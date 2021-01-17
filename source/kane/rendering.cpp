#include <kane/rendering.h>
#include <kane/logging.h>
#include <kane/settings.h>
#include <kane/assets.h>
#include <kane/timing.h>
#include <kane/camera.h>
#include <kane/game.h>
#include <kane/local_player.h>

#include <nanovg.h>
#include <glm/glm.hpp>

namespace kane::game {

	extern std::vector<std::shared_ptr<entity>> entities;
}

namespace kane::rendering {

	struct debug_box_info {
		std::string name;
		glm::vec2 min, max;
		glm::vec4 color;
		float secs;
	};

	struct debug_sphere_info {
		std::string name;
		glm::vec2 location;
		float radius;
		glm::vec4 color;
		float secs;
	};

	std::vector<debug_box_info> debug_boxes;
	std::vector<debug_sphere_info> debug_spheres;

	void set_animation_frame(std::shared_ptr<game::entity> ent, animation &anim, int frame) {
		anim.steps_left_to_wait = anim.num_wait_steps;
		anim.current_frame = frame;
		ent->last_stepped_animation = ent->current_anim;
		ent->anim_frame_cb(frame);
	}

	void step_animation(std::shared_ptr<game::entity> ent, animation &anim) {
		anim.steps_left_to_wait--;
		if (ent->last_stepped_animation != ent->current_anim) {
			sl::debug("Swapping animation: {} -> {}", ent->last_stepped_animation, ent->current_anim);
			set_animation_frame(ent, anim, 0);
		}
		if (anim.steps_left_to_wait > 0) return;
		anim.current_frame++;
		if (anim.current_frame >= anim.frame_xy_list.size()) {
			if (anim.loop) anim.current_frame = 0;
			else {
				if (anim.next_anim == "") anim.current_frame = anim.frame_xy_list.size() - 1;
				else {
					ent->current_anim = anim.next_anim;
					set_animation_frame(ent, ent->anims[ent->current_anim], 0);
					return;
				}
			}
		}
		set_animation_frame(ent, anim, anim.current_frame);
	}

	void render_sprite(NVGcontext *nvg, std::shared_ptr<game::entity> ent, sprite &spr) {
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
		nvgStrokeColor(nvg, nvgRGBAf(.5f, 1, .5f, .1f));
		nvgStrokeWidth(nvg, .5f);
		nvgStroke(nvg);
		nvgBeginPath(nvg);
		nvgCircle(
			nvg,
			static_cast<float>(spr.tile_pos.x + spr.tile_off.x) + (static_cast<float>(spr.tile_size.x) * .5f),
			static_cast<float>(spr.tile_pos.y + spr.tile_off.y) + (static_cast<float>(spr.tile_size.y) * .5f),
			1.f
		);
		nvgStrokeWidth(nvg, .5f);
		nvgStrokeColor(nvg, nvgRGBAf(0, 1, 1, .4f));
		nvgStroke(nvg);
		nvgBeginPath(nvg);
		nvgMoveTo(
			nvg,
			static_cast<float>(spr.tile_pos.x) + (static_cast<float>(spr.tile_size.x) * .5f) + 1,
			static_cast<float>(spr.tile_pos.y + spr.tile_off.y) + (static_cast<float>(spr.tile_size.y) * .5f)
		);
		nvgLineTo(
			nvg,
			static_cast<float>(spr.tile_pos.x) + (static_cast<float>(spr.tile_size.x) * .5f) - 1,
			static_cast<float>(spr.tile_pos.y + spr.tile_off.y) + (static_cast<float>(spr.tile_size.y) * .5f)
		);
		nvgStrokeWidth(nvg, 1);
		nvgStroke(nvg);
	}

	void render_entity(NVGcontext *nvg, std::shared_ptr<game::entity> ent) {
		auto anim_i = ent->anims.find(ent->current_anim);
		if (anim_i == ent->anims.end()) {
			sl::warn("Assigned animation ({}) doesn't appear to be valid.", ent->current_anim);
			return;
		}
		if (anim_i->second.sheet_img == 0) {
			auto sheet_i = assets::sprite_sheet_gl_handles.find(ent->current_anim);
			if (sheet_i == assets::sprite_sheet_gl_handles.end()) {
				sl::warn("Unable to find sprite for current animation: ", ent->current_anim);
				return;
			}
			glm::ivec2 sheet_size;
			nvgImageSize(nvg, sheet_i->second, &sheet_size.x, &sheet_size.y);
			sl::debug("Linking sprite sheet to entity: {}, GL texture #{}. ({}, {})", ent->current_anim, sheet_i->second, sheet_size.x, sheet_size.y);
			ent->anim_sheet_assign_cb(ent->current_anim, sheet_i->second, sheet_size);
			if (anim_i->second.sheet_img == 0) {
				sl::warn("Sprite sheet proposal wasn't absorbed: {}, GL texture #{}.", ent->current_anim, sheet_i->second);
				return;
			}
		}
		anim_i->second.sheet_off = {
			anim_i->second.tile_size.x * -anim_i->second.frame_xy_list[anim_i->second.current_frame].x,
			anim_i->second.tile_size.y * -anim_i->second.frame_xy_list[anim_i->second.current_frame].y
		};
		float transform[6], translation[6], scale[6];
		nvgTransformIdentity(transform);
		glm::vec2 location;
		ent->render_location_cb(location);
		nvgTransformTranslate(translation, location.x, -location.y); // Y-axis is heading down by default, flip it
		nvgTransformScale(scale, ent->sprite_flipped ? -1 : 1, 1);
		nvgTransformMultiply(transform, scale);
		nvgTransformMultiply(transform, translation);
		nvgTransform(nvg, transform[0], transform[1], transform[2], transform[3], transform[4], transform[5]);
		render_sprite(nvg, ent, anim_i->second);
	}

	void update_entity(std::shared_ptr<game::entity> ent, double secs) {
		auto anim = ent->anims.find(ent->current_anim);
		if (anim == ent->anims.end() || anim->second.sheet_img == 0) return;
		step_animation(ent, ent->anims[ent->current_anim]);
	}

	void update(double secs) {
		{
			auto entities_snapshot = game::entities;
			for (auto ent : entities_snapshot) update_entity(ent, secs);
		}
		for (auto &dbg : debug_boxes) dbg.secs -= secs;
		for (auto &dbg : debug_spheres) dbg.secs -= secs;
		for (size_t i = 0; i < debug_boxes.size(); i++) {
			if (debug_boxes[i].secs > 0) continue;
			debug_boxes.erase(debug_boxes.begin() + i);
			i--;
		}
		for (size_t i = 0; i < debug_spheres.size(); i++) {
			if (debug_spheres[i].secs > 0) continue;
			debug_spheres.erase(debug_spheres.begin() + i);
			i--;
		}
	}

	void render_bg_asset(NVGcontext *nvg, int x, int y, int w, int h, float tx_scale, int gl_tex_id, glm::ivec2 framebuffer_size) {
		nvgResetTransform(nvg);
		nvgTranslate(
			nvg,
			glm::round((framebuffer_size.x * .5f) - (camera::location.x * camera::scale * tx_scale)),
			glm::round((framebuffer_size.y * .5f) + (camera::location.y * camera::scale * tx_scale))
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

kane::rendering::entity::~entity() {

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
	if (lp::entity) camera::location = lp::entity->location;
	camera::scale = glm::round(glm::max(1.f, framebuffer_size.x / 600.f));
	render_bg_parallax(nvg, framebuffer_size);
	{
		auto entities_snapshot = game::entities;
		for (auto ent : entities_snapshot) {
			nvgResetTransform(nvg);
			nvgTranslate(
				nvg,
				glm::round((framebuffer_size.x * .5f) - (camera::location.x * camera::scale)),
				glm::round((framebuffer_size.y * .5f) + (camera::location.y * camera::scale))
			);
			nvgScale(nvg, camera::scale, camera::scale);
			render_entity(nvg, ent);
		}
	}
	for (auto &dbg : debug_boxes) {
		nvgResetTransform(nvg);
		nvgTranslate(
			nvg,
			glm::round((framebuffer_size.x * .5f) - (camera::location.x * camera::scale)),
			glm::round((framebuffer_size.y * .5f) + (camera::location.y * camera::scale))
		);
		nvgScale(nvg, camera::scale, camera::scale);
		nvgBeginPath(nvg);
		nvgRect(nvg, dbg.min.x, -dbg.max.y, dbg.max.x - dbg.min.x, dbg.max.y - dbg.min.y);
		nvgStrokeColor(nvg, nvgRGBAf(1, 1, 1, .1f));
		nvgStrokeWidth(nvg, .5f);
		nvgStroke(nvg);
		nvgFontFace(nvg, "comfortaa_bold");
		nvgFontSize(nvg, 4);
		nvgFillColor(nvg, nvgRGBAf(1, 0, 0, .3f));
		nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);
		nvgText(nvg, dbg.min.x, -dbg.max.y - 1, dbg.name.data(), 0);
	}
	for (auto &dbg : debug_spheres) {
		nvgResetTransform(nvg);
		nvgTranslate(
			nvg,
			glm::round((framebuffer_size.x * .5f) - (camera::location.x * camera::scale)),
			glm::round((framebuffer_size.y * .5f) + (camera::location.y * camera::scale))
		);
		nvgScale(nvg, camera::scale, camera::scale);
		nvgBeginPath(nvg);
		nvgCircle(nvg, dbg.location.x, -dbg.location.y, dbg.radius);
		nvgStrokeColor(nvg, nvgRGBAf(1, 1, 1, .1f));
		nvgStrokeWidth(nvg, .5f);
		nvgStroke(nvg);
		nvgFontFace(nvg, "comfortaa_bold");
		nvgFontSize(nvg, 4);
		nvgFillColor(nvg, nvgRGBAf(1, 0, 0, .3f));
		nvgTextAlign(nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgText(nvg, dbg.location.x, dbg.location.y, dbg.name.data(), 0);
	}
	render_ui(nvg, framebuffer_size);
}

void kane::rendering::shutdown(NVGcontext *nvg) {
	debug_boxes.clear();
	debug_spheres.clear();
}

void kane::rendering::spawn_debug_box(const std::string &name, const glm::vec2 &min, const glm::vec2 &max, const glm::vec4 &color, float secs) {
	debug_boxes.push_back({ name, min, max, color, secs });
}

void kane::rendering::spawn_debug_sphere(const std::string &name, const glm::vec2 &location, float radius, const glm::vec4 &color, float secs) {
	debug_spheres.push_back({ name, location, radius, color, secs });
}