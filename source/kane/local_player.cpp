#include <kane/local_player.h>
#include <kane/logging.h>
#include <kane/input.h>
#include <kane/timing.h>
#include <kane/entity_shadow.h>
#include <kane/entity_merchant.h>

#include <glm/glm.hpp>

std::unique_ptr<kane::rendering::entity> kane::lp::entity;

namespace kane::lp {

	double idle_time = 0;
	pc::merchant_entity *merchant = 0;

	void update(double secs) {
		
	}

	void update_varying(double secs) {
		if (entity && entity->update) entity->update(secs);
		if (merchant && merchant->update) merchant->update(secs);
	}

	void make_random_merchant() {
		merchant = new pc::merchant_entity;
		merchant->pos.x = 100;
		merchant->update = [](double secs) {
			static float tmp = 0;
			tmp += secs;
			if (tmp < 3.f) merchant->current_anim = "merchant-idle";
			else if (tmp < 6.f) {
				merchant->current_anim = "merchant-walk";
				merchant->pos.x += (merchant->flipped ? -20.f : 20.f) * secs;
			} else {
				tmp = 0;
				merchant->current_anim = "merchant-idle";
			}
			if (merchant->pos.x > 30) merchant->flipped = true;
			if (merchant->pos.x < -30) merchant->flipped = false;
		};
	}

	void become_shadow_entity() {
		entity = std::make_unique<pc::shadow_entity>();
		entity->update = [](double secs) {
			if (entity->current_anim == "shadow-attack-1" || entity->current_anim == "shadow-attack-2") {
				for (auto &anim : entity->anims) anim.second.alpha = 1;
				idle_time = 0;
				return;
			}
			if (input::lp_util_1) {
				entity->current_anim = "shadow-charge";
				for (auto &anim : entity->anims) anim.second.alpha = 1;
				idle_time = 0;
				return;
			}
			if (input::lp_attack_1) {
				entity->current_anim = "shadow-attack-2";
				return;
			}
			if (input::lp_attack_2) {
				entity->current_anim = "shadow-attack-1";
				return;
			}
			if (input::lp_movement.x != 0) {
				entity->current_anim = "shadow-run";
				entity->flipped = input::lp_movement.x >= 0 ? false : true;
				entity->pos.x += input::lp_movement.x * 50.f * static_cast<float>(secs);
				idle_time = 0;
			} else {
				idle_time += secs;
				if (idle_time > 1.0 / 10.0) entity->current_anim = "shadow-idle";
			}
			if (idle_time > 3) for (auto &anim : entity->anims) anim.second.alpha -= .65f * secs;
			else for (auto &anim : entity->anims) anim.second.alpha += 6.f * secs;
			for (auto &anim : entity->anims) anim.second.alpha = glm::max(0.4f, glm::min(1.f, anim.second.alpha));
		};
	}
}

void kane::lp::initialize() {
	if (static bool first = true; first) {
		timing::subscribe(update);
		timing::subscribe(update_varying, true);
		first = false;
	}
	become_shadow_entity();
	make_random_merchant();
}

void kane::lp::shutdown() {
	entity.release();
}