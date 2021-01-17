#pragma once

#include <kane/game.h>

namespace kane::pc {
	struct shadow_entity : game::entity {

		int sound_slot_charging = -1;

		shadow_entity();
		virtual ~shadow_entity();

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
		void render_location_cb(glm::vec2 &location_out) override;
	};
}