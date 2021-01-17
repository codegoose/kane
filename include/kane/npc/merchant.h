#pragma once

#include <kane/game.h>

namespace kane::npc {
	struct merchant : game::entity {

		float state_timing = 0;
		bool running_from_player = false;

		merchant();
		virtual ~merchant();

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
		void update_cb(double secs) override;
		void receive_damage(const std::string_view description, int amount) override;
		void render_location_cb(glm::vec2 &location_out) override;
	};
}