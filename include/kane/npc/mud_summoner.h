#pragma once

#include <kane/game.h>

namespace kane::npc {
	struct mud_summoner : game::entity {

		bool summoning = false;
		float since_last_summon = 0;
		float since_last_misc = 0;

		mud_summoner();
		virtual ~mud_summoner();

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
		void update_cb(double secs) override;
		void render_location_cb(glm::vec2 &location_out) override;
	};
}