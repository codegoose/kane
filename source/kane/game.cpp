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

	std::vector<std::shared_ptr<rendering::entity>> npcs;

	struct merchant_instance : pc::merchant_entity, traits::mortal, signals::emitter {

		float state_timing = 0;
		bool running_from_player = false;

		merchant_instance() {
			signalling_id = "merchant_npc";
			update = std::bind(&merchant_instance::update_cb, this, std::placeholders::_1);
		}

		virtual ~merchant_instance() {

		}

		void receive_damage(const signals::source &src, int amount) override {
			if (src.id == signalling_id) return;
		}

		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override {
			if (src.id == signalling_id) return;
			if (!(pos.x >= info.min.x && pos.x <= info.max.x && pos.y >= info.min.y && pos.y <= info.max.y)) return;
			running_from_player = true;
			state_timing = 3.f;
		}

		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override {
			if (src.id == signalling_id) return;
			if (glm::distance(pos, info.pos) > info.radius) return;
			running_from_player = true;
			state_timing = 3.f;
		}

		void update_cb(double secs) {
			if (running_from_player) {
				if (lp::entity) {
					if (state_timing > 0) {
						flipped = pos.x > lp::entity->pos.x ? false : true;
						current_anim = "merchant_walk";
						pos.x += (flipped ? -40.f : 40.f) * secs;
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
				pos.x += (flipped ? -20.f : 20.f) * secs;
			} else {
				state_timing = 0;
				current_anim = "merchant_idle";
			}
			if (pos.x > 30) flipped = true;
			if (pos.x < -30) flipped = false;
		}
	};

	struct mud_minion_instance : pc::mud_minion_entity, traits::mortal, signals::emitter {

		float follow_variance = -10 + rand() % 10;

		mud_minion_instance() {
			signalling_id = "mud_minion_npc";
			update = std::bind(&mud_minion_instance::update_cb, this, std::placeholders::_1);
		}

		virtual ~mud_minion_instance() {

		}

		void receive_damage(const signals::source &src, int amount) override {
			if (src.id == signalling_id) return;
		}

		void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) override {
			if (src.id == signalling_id) return;
			if (!(pos.x >= info.min.x && pos.x <= info.max.x && pos.y >= info.min.y && pos.y <= info.max.y)) return;
			current_anim = "mud_minion_damage";
		}

		void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) override {
			if (src.id == signalling_id) return;
			if (glm::distance(pos, info.pos) > info.radius) return;
			current_anim = "mud_minion_damage";
		}

		void update_cb(double secs) {
			if (current_anim == "mud_minion_damage") return;
			if (current_anim == "mud_minion_attack") return;
			current_anim = "mud_minion_move";
			auto distance = glm::distance(pos, lp::entity->pos);
			if (distance > 20.f + follow_variance) {
				flipped = pos.x > lp::entity->pos.x ? true : false;
				pos.x += (flipped ? -40.f : 40.f) * secs;
			} else current_anim = "mud_minion_attack";
		}
	};

	struct mud_summoner_instance : pc::mud_summoner_entity, traits::mortal, signals::emitter {

		bool summoning = false;
		float since_last_summon = 0;
		float since_last_misc = 0;

		mud_summoner_instance() {
			signalling_id = "mud_summoner_npc";
			update = std::bind(&mud_summoner_instance::update_cb, this, std::placeholders::_1);
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

		void update_cb(double secs) {
			if (summoning && current_anim == "summoner_idle") {
				summoning = false;
				since_last_summon = 0;
			}
			if (current_anim != "mud_summoner_summon") since_last_summon += secs;
			auto distance = glm::distance(pos, lp::entity->pos);
			if (distance < 200.f) {
				if (pos.x > lp::entity->pos.x) flipped = false;
				if (pos.x < lp::entity->pos.x) flipped = true;
				current_anim = "mud_summoner_run";
				pos.x += (flipped ? -40.f : 40.f) * secs;
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
	};

	void update_varying(double secs) {
		if (lp::entity && lp::entity->update) lp::entity->update(secs);
		for (auto &npc : npcs) {
			if (!npc || !npc->update) continue;
			npc->update(secs);
		}
	}

	void make_random_merchant() {
		auto merchant = std::make_shared<merchant_instance>();
		merchant->pos.x = -100 + rand() % 200;
		npcs.push_back(merchant);
	}

	void make_random_mud_summoner() {
		auto summoner = std::make_shared<mud_summoner_instance>();
		summoner->pos.x = -50 + rand() % 100;
		npcs.push_back(summoner);
	}

	void make_random_mud_minion() {
		auto minion = std::make_shared<mud_minion_instance>();
		minion->pos.x = -600 + rand() % 1200;
		npcs.push_back(minion);
	}
}

void kane::game::initialize() {
	if (static bool first = true; first) {
		timing::subscribe(update_varying, true);
		first = false;
	}
	make_random_merchant();
	make_random_mud_summoner();
	make_random_mud_minion();
	make_random_mud_minion();
	make_random_mud_minion();
	make_random_mud_minion();
	make_random_mud_minion();
	make_random_mud_minion();
	audio::play_music("low_fog");
}

void kane::game::shutdown() {
	npcs.clear();
}