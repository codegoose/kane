#include <kane/rendering.h>
#include <kane/logging.h>
#include <kane/settings.h>
#include <kane/assets.h>
#include <kane/timing.h>
#include <kane/camera.h>
#include <kane/local_player.h>

#include <nanovg.h>
#include <glm/glm.hpp>

namespace kane::rendering {

	std::vector<entity *> entities;

	void set_animation_frame(animation &anim, int frame) {
		anim.steps_left_to_wait = anim.num_wait_steps;
		anim.current_frame = frame;
	}

	void step_animation(entity &ent, animation &anim) {
		anim.steps_left_to_wait--;
		if (anim.steps_left_to_wait > 0) return;
		anim.current_frame++;
		if (anim.current_frame >= anim.frame_xy_list.size()) {
			if (anim.loop) anim.current_frame = 0;
			else {
				if (anim.next_anim == "") anim.current_frame = anim.frame_xy_list.size() - 1;
				else {
					ent.current_anim = anim.next_anim;
					set_animation_frame(ent.anims[ent.current_anim], 0);
					return;
				}
			}
		}
		set_animation_frame(anim, anim.current_frame);
	}

	void render_sprite(NVGcontext *nvg, sprite &spr) {
		nvgBeginPath(nvg);
		nvgRect(nvg, spr.tile_pos.x + spr.tile_off.x, spr.tile_pos.y + spr.tile_off.y, spr.tile_size.x, spr.tile_size.y);
		nvgFillPaint(nvg,
			nvgImagePattern(
				nvg,
				spr.tile_pos.x + spr.tile_off.x + spr.sheet_off.x,
				spr.tile_pos.y + spr.tile_off.y + spr.sheet_off.y,
				spr.sheet_size.x,
				spr.sheet_size.y,
				0,
				spr.sheet_img,
				spr.alpha
			)
		);
		nvgFill(nvg);
	}

	void render_entity(NVGcontext *nvg, entity &ent) {
		auto anim_i = ent.anims.find(ent.current_anim);
		if (anim_i == ent.anims.end()) {
			sl::warn("Assigned animation ({}) doesn't appear to be valid.", ent.current_anim);
			return;
		}
		if (anim_i->second.sheet_img == 0) {
			auto sheet_i = assets::sprite_sheet_gl_handles.find(ent.current_anim);
			if (sheet_i == assets::sprite_sheet_gl_handles.end()) {
				sl::warn("Unable to find sprite for current animation: ", ent.current_anim);
				return;
			}
			glm::ivec2 sheet_size;
			nvgImageSize(nvg, sheet_i->second, &sheet_size.x, &sheet_size.y);
			sl::debug("Linking sprite sheet to entity: {}, GL texture #{}. ({}, {})", ent.current_anim, sheet_i->second, sheet_size.x, sheet_size.y);
			ent.anim_sheet_assign_cb(ent.current_anim, sheet_i->second, sheet_size);
			if (anim_i->second.sheet_img == 0) {
				sl::warn("Sprite sheet proposal wasn't absorbed: {}, GL texture #{}.", ent.current_anim, sheet_i->second);
				return;
			}
		}
		if (ent.last_rendered_anim != ent.current_anim) set_animation_frame(anim_i->second, 0);
		anim_i->second.sheet_off = {
			anim_i->second.tile_size.x * -anim_i->second.frame_xy_list[anim_i->second.current_frame].x,
			anim_i->second.tile_size.y * -anim_i->second.frame_xy_list[anim_i->second.current_frame].y
		};
		float transform[6], translation[6], scale[6];
		nvgTransformIdentity(transform);
		nvgTransformTranslate(translation, ent.pos.x, -ent.pos.y); // Y-axis is heading down by default, flip it
		nvgTransformScale(scale, ent.flipped ? -1 : 1, 1);
		nvgTransformMultiply(transform, scale);
		nvgTransformMultiply(transform, translation);
		nvgTransform(nvg, transform[0], transform[1], transform[2], transform[3], transform[4], transform[5]);
		render_sprite(nvg, anim_i->second);
		ent.last_rendered_anim = ent.current_anim;
	}

	void update_entity(entity &ent, double secs) {
		auto anim = ent.anims.find(ent.current_anim);
		if (anim == ent.anims.end() || anim->second.sheet_img == 0) return;
		step_animation(ent, ent.anims[ent.current_anim]);
	}

	void update(double secs) {
		for (auto ent : entities) update_entity(*ent, secs);
	}

	void render_bg_asset(NVGcontext *nvg, int x, int y, int w, int h, float tx_scale, int gl_tex_id, glm::ivec2 framebuffer_size) {
		nvgResetTransform(nvg);
		nvgTranslate(
			nvg,
			glm::round((framebuffer_size.x * .5f) - (camera::pos.x * camera::scale * tx_scale)),
			glm::round((framebuffer_size.y * .5f) + (camera::pos.y * camera::scale * tx_scale))
		);
		nvgScale(nvg, camera::scale, camera::scale);
		nvgBeginPath(nvg);
		nvgRect(nvg, x, y, w, h);
		nvgFillPaint(nvg, nvgImagePattern(nvg, x, y, w, h, 0, gl_tex_id, 1));
		nvgFill(nvg);
	}

	void render_bg_parallax(NVGcontext *nvg, glm::ivec2 framebuffer_size) {
		// auto scene_view_size = glm::fvec2(framebuffer_size) / camera::scale;
		// sl::info("{} <-> {}", camera::pos.x - (scene_view_size.x * .5f), camera::pos.x + (scene_view_size.x * .5f));
		{
			auto img_glid = assets::sprite_sheet_gl_handles["bg_city_2_far"];
			int img_w, img_h;
			nvgImageSize(nvg, img_glid, &img_w, &img_h);
			for (int i = -8; i < 8; i++) render_bg_asset(nvg, img_w * i, -img_h, img_w, img_h, .1f, img_glid, framebuffer_size);
		}
		{
			auto img_glid = assets::sprite_sheet_gl_handles["bg_city_2_med"];
			int img_w, img_h;
			nvgImageSize(nvg, img_glid, &img_w, &img_h);
			for (int i = -12; i < 12; i++) render_bg_asset(nvg, img_w * i, -img_h, img_w, img_h, .35f, img_glid, framebuffer_size);
		}
		{
			auto img_glid = assets::sprite_sheet_gl_handles["bg_city_2_close"];
			int img_w, img_h;
			nvgImageSize(nvg, img_glid, &img_w, &img_h);
			for (int i = -16; i < 16; i++) render_bg_asset(nvg, img_w * i, -img_h, img_w, img_h, .8f, img_glid, framebuffer_size);
		}
	}

	void render_ui(NVGcontext *nvg, glm::ivec2 framebuffer_size) {
		nvgResetTransform(nvg);
		nvgFontFace(nvg, "comfortaa_bold");
		nvgFontSize(nvg, glm::round(10 * camera::scale));
		nvgFillColor(nvg, nvgRGBA(255, 255, 255, 128));
		nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);
		nvgText(nvg, glm::round(10 * camera::scale), framebuffer_size.y - glm::round(10 * camera::scale), "Project Kane - Alpha", 0);
		nvgFontSize(nvg, glm::round(8 * camera::scale));
		nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgFontFace(nvg, "comfortaa");
		float adv = 0;
		for (auto text : { "Q - charge", "F - Special Attack", "Space - Attack", "WASD - move" }) {
			nvgText(nvg, glm::round(10 * camera::scale), glm::round(10 * camera::scale) + adv, text, 0);
			adv += glm::round(10 * camera::scale);
		}
	}
}

bool kane::rendering::initialize(NVGcontext *nvg) {
	if (static bool first = true; first) {
		timing::subscribe(update);
		first = false;
	}
	settings::persist["rendering-init-num"] = settings::persist.value("rendering-init-num", 0) + 1;
	return true;
}

void kane::rendering::render(NVGcontext *nvg, glm::ivec2 framebuffer_size) {
	camera::pos = lp::entity->pos;
	camera::scale = glm::round(glm::max(1.f, framebuffer_size.x / 600.f));
	render_bg_parallax(nvg, framebuffer_size);
	for (auto ent : entities) {
		nvgResetTransform(nvg);
		nvgTranslate(
			nvg,
			glm::round((framebuffer_size.x * .5f) - (camera::pos.x * camera::scale)),
			glm::round((framebuffer_size.y * .5f) + (camera::pos.y * camera::scale))
		);
		nvgScale(nvg, camera::scale, camera::scale);
		render_entity(nvg, *ent);
	}
	render_ui(nvg, framebuffer_size);
}

void kane::rendering::shutdown(NVGcontext *nvg) {

}

kane::rendering::entity::entity() {
	entities.push_back(this);
}

kane::rendering::entity::~entity() {
	entities.erase(std::find(entities.begin(), entities.end(), this));
}