#include <kane/game.h>
#include <kane/audio.h>
#include <kane/timing.h>
#include <kane/traits.h>
#include <kane/local_player.h>
#include <kane/entity_merchant.h>
#include <kane/entity_mud_summoner.h>
#include <kane/entity_mud_minion.h>
#include <kane/logging.h>

#include <glm/glm.hpp>

#include <memory>

namespace kane::game {

	std::vector<entity *> entities;
}

kane::game::entity::entity() {
	entities.push_back(this);
}

kane::game::entity::~entity() {
	entities.erase(std::find(entities.begin(), entities.end(), this));
}

namespace kane::game {

	struct merchant_instance : entity, pc::merchant_entity, traits::mortal, signals::emitter {

		float state_timing = 0;
		bool running_from_player = false;

		merchant_instance() {
			signalling_id = "merchant_npc";
		}

		virtual ~merchant_instance() {

		}

		void receive_damage(const signals::source &src, int amount) override {
			if (src.id == signalling_id) return;
		}

		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override {
			if (src.id == signalling_id) return;
			if (!(location.x >= info.min.x && location.x <= info.max.x && location.y >= info.min.y && location.y <= info.max.y)) return;
			running_from_player = true;
			state_timing = 3.f;
		}

		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override {
			if (src.id == signalling_id) return;
			if (glm::distance(location, info.location) > info.radius) return;
			running_from_player = true;
			state_timing = 3.f;
		}

		void update_cb(double secs) override {
			if (running_from_player) {
				if (lp::entity) {
					if (state_timing > 0) {
						sprite_flipped = location.x > lp::entity->location.x ? false : true;
						current_anim = "merchant_walk";
						location.x += (sprite_flipped ? -40.f : 40.f) * secs;
						state_timing -= secs;
						return;
					}
				}
				running_from_player = false;
			}
			state_timing += secs;
			if (state_timing < 3.f) current_anim = "merchant_idle";
			else if (state_timing < 6.f) {
				current_anim = "merchant_walk";
				location.x += (sprite_flipped ? -20.f : 20.f) * secs;
			} else {
				state_timing = 0;
				current_anim = "merchant_idle";
			}
			if (location.x > 30) sprite_flipped = true;
			if (location.x < -30) sprite_flipped = false;
		}

		void render_location_cb(glm::vec2 &location_out) override {
			location_out = location;
		}
	};

	struct mud_minion_instance : entity, pc::mud_minion_entity, traits::mortal, signals::emitter {

		float follow_variance = -10 + rand() % 10;

		mud_minion_instance() {
			signalling_id = "mud_minion_npc";
		}

		virtual ~mud_minion_instance() {

		}

		void receive_damage(const signals::source &src, int amount) override {
			if (src.id == signalling_id) return;
		}

		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override {
			if (src.id == signalling_id) return;
			if (!(location.x >= info.min.x && location.x <= info.max.x && location.y >= info.min.y && location.y <= info.max.y)) return;
			current_anim = "mud_minion_damage";
		}

		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override {
			if (src.id == signalling_id) return;
			if (glm::distance(location, info.location) > info.radius) return;
			current_anim = "mud_minion_damage";
		}

		void update_cb(double secs) override {
			if (current_anim == "mud_minion_damage") return;
			if (current_anim == "mud_minion_attack") return;
			current_anim = "mud_minion_move";
			auto distance = glm::distance(location, lp::entity->location);
			if (distance > 20.f + follow_variance) {
				sprite_flipped = location.x > lp::entity->location.x ? true : false;
				location.x += (sprite_flipped ? -40.f : 40.f) * secs;
			} else current_anim = "mud_minion_attack";
		}

		void render_location_cb(glm::vec2 &location_out) override {
			location_out = location;
		}
	};

	struct mud_summoner_instance : entity, pc::mud_summoner_entity, traits::mortal, signals::emitter {

		bool summoning = false;
		float since_last_summon = 0;
		float since_last_misc = 0;

		mud_summoner_instance() {
			signalling_id = "mud_summoner_npc";
		}

		virtual ~mud_summoner_instance() {

		}

		void receive_damage(const signals::source &src, int amount) override {
			if (src.id == signalling_id) return;
		}

		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override {

		}

		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override {

		}

		void update_cb(double secs) override {
			if (summoning && current_anim == "summoner_idle") {
				summoning = false;
				since_last_summon = 0;
			}
			if (current_anim != "mud_summoner_summon") since_last_summon += secs;
			auto distance = glm::distance(location, lp::entity->location);
			if (distance < 200.f) {
				if (location.x > lp::entity->location.x) sprite_flipped = false;
				if (location.x < lp::entity->location.x) sprite_flipped = true;
				current_anim = "mud_summoner_run";
				location.x += (sprite_flipped ? -40.f : 40.f) * secs;
			} else {
				if (since_last_summon >= 4.f) {
					current_anim = "mud_summoner_summon";
					summoning = true;
					since_last_summon = 0;
				} else if (!summoning) {
					current_anim = "mud_summoner_idle";
					since_last_misc += secs;
				}
			}
		}

		void render_location_cb(glm::vec2 &location_out) override {
			location_out = location;
		}
	};

	void update_varying(double secs) {
		auto copies = entities;
		for (auto &cent : copies) cent->update_cb(secs);
	}
}

void kane::game::initialize() {
	if (static bool first = true; first) {
		timing::subscribe(update_varying, true);
		first = false;
	}
	audio::play_music("low_fog");
	new merchant_instance;
	new mud_summoner_instance;
}

void kane::game::shutdown() {

}