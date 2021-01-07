#include <kane/local_player.h>
#include <kane/logging.h>
#include <kane/input.h>
#include <kane/timing.h>
#include <kane/entity_shadow.h>

#include <glm/glm.hpp>

std::unique_ptr<kane::rendering::entity> kane::lp::entity;

namespace kane::lp {

	double idle_time = 0;

	void update(double secs) {
		
	}

	void update_varying(double secs) {
		if (entity && entity->update) entity->update(secs);
	}

	void become_shadow_entity() {
		entity = std::make_unique<pc::shadow_entity>();
		entity->update = [](double secs) {
			if (input::lp_movement.x != 0) {
				entity->current_anim = "shadow-run";
				entity->flipped = input::lp_movement.x > 0 ? false : true;
				entity->pos += input::lp_movement * 50.f * static_cast<float>(secs);
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
}

void kane::lp::shutdown() {
	entity.release();
}