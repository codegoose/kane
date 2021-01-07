#include <kane/input.h>

#include <GLFW/glfw3.h>

kane::input::type kane::input::active_device_type = kane::input::type::keyboard;
glm::vec2 kane::input::lp_movement;

void kane::input::initialize(GLFWwindow *window) {
	glfwSetKeyCallback(window, 0);
}

void kane::input::shutdown() {

}

void kane::input::update(double secs) {
	
}