#include <kane/local_player.h>
#include <kane/logging.h>
#include <kane/input.h>
#include <kane/entity_shadow.h>

#include <glm/glm.hpp>

std::shared_ptr<kane::game::entity> kane::lp::entity;

namespace kane::lp {

	struct shadow_instance : pc::shadow_entity {

		double idle_time = 0;

		shadow_instance() {
			max_health = 1000;
			health = max_health;
			invulnerable = false;
		}

		virtual ~shadow_instance() {

		}

		void update_cb(double secs) override {
			if (current_anim == "shadow_attack_1" || current_anim == "shadow_attack_2") {
				for (auto &anim : anims) anim.second.alpha = 1;
				idle_time = 0;
				return;
			}
			if (input::lp_util_1) {
				current_anim = "shadow_charge";
				for (auto &anim : anims) anim.second.alpha = 1;
				idle_time = 0;
				return;
			}
			if (input::lp_attack_1) {
				current_anim = "shadow_attack_2";
				return;
			}
			if (input::lp_attack_2) {
				current_anim = "shadow_attack_1";
				return;
			}
			if (input::lp_movement.x != 0) {
				current_anim = "shadow_run";
				sprite_flipped = input::lp_movement.x >= 0 ? false : true;
				location.x += input::lp_movement.x * 50.f * static_cast<float>(secs);
				idle_time = 0;
			} else {
				idle_time += secs;
				if (idle_time > 1.0 / 10.0) current_anim = "shadow_idle";
			}
			if (idle_time > 3) for (auto &anim : anims) anim.second.alpha -= .65f * secs;
			else for (auto &anim : anims) anim.second.alpha += 6.f * secs;
			for (auto &anim : anims) anim.second.alpha = glm::max(0.4f, glm::min(1.f, anim.second.alpha));
		}
	};
}

void kane::lp::initialize() {
	entity = std::make_shared<shadow_instance>();
	entity->team = game::alliance::player;
	game::add_entity(entity);
}

void kane::lp::shutdown() {
	game::remove_entity(entity);
	entity.reset();
}