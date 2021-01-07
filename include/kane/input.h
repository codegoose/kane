#pragma once

#include <glm/vec2.hpp>

namespace kane::input {

	enum class type {
		keyboard,
		gamepad
	};

	extern type active_device_type;
	extern glm::vec2 lp_movement;

	void update(double secs);
}