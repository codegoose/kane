#include <kane/input.h>
#include <kane/logging.h>

#include <GLFW/glfw3.h>

kane::input::type kane::input::active_device_type = kane::input::type::keyboard;
glm::vec2 kane::input::lp_movement;

namespace kane::input {

	bool fullscreen = false;

	void on_glfw_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
			if (fullscreen) {
				int x, y, w, h;
				glfwGetMonitorWorkarea(glfwGetWindowMonitor(window), &x, &y, &w, &h);
				glfwSetWindowMonitor(window, 0, x + (w / 2) - (1024 / 2), y + (h / 2) - (768 / 2), 1024, 768, GLFW_DONT_CARE);
			} else {
				auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			fullscreen = !fullscreen;
		}
	}
}

void kane::input::initialize(GLFWwindow *window) {
	if (static bool first = true; first) {
		glfwSetKeyCallback(window, on_glfw_key);
		first = false;
	}
}

void kane::input::shutdown() {

}

void kane::input::update(GLFWwindow *window) {
	switch (active_device_type) {
		case type::keyboard:
			lp_movement = { 0, 0 };
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) lp_movement.y += 1;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) lp_movement.x -= 1;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) lp_movement.y -= 1;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) lp_movement.x += 1;
			break;
		case type::gamepad:
			break;
	}
}