#pragma once

#include <kane/game.h>

namespace kane::npc {
	struct mud_minion : game::entity {

		float follow_variance = -10 + rand() % 10;

		mud_minion();
		virtual ~mud_minion();

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
		void update_cb(double secs) override;
		void render_location_cb(glm::vec2 &location_out) override;
	};
}