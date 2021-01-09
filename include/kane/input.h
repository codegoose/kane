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
	extern bool lp_attack_1;
	extern bool lp_attack_2;
	extern bool lp_util_1;
	extern bool lp_util_2;

	void initialize(GLFWwindow *window);
	void shutdown();
	void update(GLFWwindow *window);
}