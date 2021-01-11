#pragma once

#include <kane/signals.h>

namespace kane::traits {

	struct mortal : signals::receiver {

		struct damage_signal {
			int amount;
		};

		struct damage_zone_rect_signal {
			glm::vec2 min, max;
			int amount;
		};

		struct damage_zone_radius_signal {
			glm::vec2 pos;
			float radius;
			int amount;
		};

		int max_health = 100;
		int health = 100;
		bool invulnerable = true;

		void receive_signal(const signals::source &src, const std::any &info) override;

		virtual void receive_damage(const signals::source &src, int amount) = 0;
		virtual void receive_damage_zone_rect(const signals::source &src, damage_zone_rect_signal info) = 0;
		virtual void receive_damage_zone_radius(const signals::source &src, damage_zone_radius_signal info) = 0;
	};
}