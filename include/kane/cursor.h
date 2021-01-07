#pragma once

struct GLFWwindow;

namespace kane::cursor{

	void initialize(GLFWwindow *window);
	void shutdown(GLFWwindow *window);
}