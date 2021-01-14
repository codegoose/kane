#pragma once

#include <kane/game.h>
#include <kane/traits.h>
#include <kane/rendering.h>

namespace kane::npc {
	struct mud_summoner : game::entity, rendering::entity, traits::mortal, signals::emitter {

		bool summoning = false;
		float since_last_summon = 0;
		float since_last_misc = 0;

		mud_summoner();
		virtual ~mud_summoner();

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
		void receive_damage(const signals::source &src, int amount) override;
		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override;
		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override;
		void update_cb(double secs) override;
		void render_location_cb(glm::vec2 &location_out) override;
	};
}