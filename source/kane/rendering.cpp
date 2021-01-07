#include <kane/rendering.h>
#include <kane/logging.h>
#include <kane/settings.h>
#include <kane/assets.h>
#include <kane/timing.h>

#include <nanovg.h>
#include <glm/glm.hpp>

namespace kane::rendering {

	std::vector<entity *> entities;

	void set_animation_frame(animation &anim, int frame) {
		anim.steps_left_to_wait = anim.num_wait_steps;
		anim.current_frame = frame;
	}

	void step_animation(animation &anim) {
		anim.steps_left_to_wait--;
		if (anim.steps_left_to_wait > 0) return;
		anim.current_frame++;
		if (anim.current_frame >= anim.frame_xy_list.size()) anim.current_frame = 0;
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
				1
			)
		);
		nvgFill(nvg);
	}

	void render_entity(NVGcontext *nvg, entity &ent) {
		auto anim_i = ent.anims.find(ent.current_anim);
		if (anim_i == ent.anims.end()) return;
		if (anim_i->second.sheet_img == 0) {
			auto sheet_i = assets::sprite_sheet_gl_handles.find(ent.current_anim);
			if (sheet_i == assets::sprite_sheet_gl_handles.end()) return;
			glm::ivec2 sheet_size;
			nvgImageSize(nvg, sheet_i->second, &sheet_size.x, &sheet_size.y);
			ent.anim_sheet_assign_cb(ent.current_anim, sheet_i->second, sheet_size);
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
		if (ent.update) ent.update(secs);
		step_animation(ent.anims[ent.current_anim]);
	}

	void update(double secs) {
		for (auto ent : entities) update_entity(*ent, secs);
	}
}

bool kane::rendering::initialize(NVGcontext *nvg) {
	if (static bool first = true; first) {
		timing::subscribe(update);
		first = false;
	}
	settings::persist["rendering-init-num"] = settings::persist.value("rendering-init-num", 0) + 1;;
	return true;
}

void kane::rendering::render(NVGcontext *nvg, glm::ivec2 framebuffer_size) {
	const int scale = glm::round(glm::max(1.f, framebuffer_size.y / 300.f));
	for (auto ent : entities) {
		nvgResetTransform(nvg);
		nvgTranslate(nvg, framebuffer_size.x / 2, framebuffer_size.y / 2);
		nvgScale(nvg, scale, scale);
		render_entity(nvg, *ent);
	}
}

void kane::rendering::shutdown(NVGcontext *nvg) {

}

kane::rendering::entity::entity() {
	entities.push_back(this);
}

kane::rendering::entity::~entity() {
	entities.erase(std::find(entities.begin(), entities.end(), this));
}