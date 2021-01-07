#pragma once

#include <glm/vec2.hpp>

struct GLFWwindow;

namespace kane::input {

	enum class type {
		keyboard,
		gamepad
	};

	extern type active_device_type;
	extern glm::vec2 lp_movement;

	void initialize(GLFWwindow *window);
	void shutdown();
	void update(double secs);
}