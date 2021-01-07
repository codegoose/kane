#define SPDLOG_FMT_EXTERNAL

#include <map>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include <kane/logging.h>
#include <kane/rendering.h>
#include <kane/settings.h>
#include <kane/assets.h>
#include <kane/timing.h>
#include <kane/cursor.h>

namespace kane {
	GLFWwindow *glfw_window = 0;
}

namespace kane {
	void run(NVGcontext *nvg) {
		if (!assets::load(nvg)) return;
		cursor::initialize();
		if (rendering::initialize(nvg)) {
			timing::reset();
			while (!glfwWindowShouldClose(glfw_window)) {
				glfwPollEvents();
				timing::tick();
				glClearColor(0.5, 0.4, 0.3, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				int w, h;
				glfwGetFramebufferSize(glfw_window, &w, &h);
				glViewport(0, 0, w, h);
				nvgBeginFrame(nvg, w, h, 1);
				rendering::render(nvg, { w, h });
				nvgEndFrame(nvg);
				glfwSwapBuffers(glfw_window);
			}
			rendering::shutdown(nvg);
		} else sl::error("Unable to initialize the rendering subsystem.");
		cursor::shutdown();
		assets::destroy(nvg);
	}
}

int main() {
	{
		std::vector<sl::sink_ptr> sinks;
		sinks.push_back(std::make_shared<sl::sinks::stdout_color_sink_mt>());
		sinks.push_back(std::make_shared<sl::sinks::daily_file_sink_mt>("log", 0, 0));
		auto combined_sink = std::make_shared<sl::logger>("main", begin(sinks), end(sinks));
		combined_sink->set_level(sl::level::debug);
		sl::set_default_logger(combined_sink);
	}
	kane::settings::load();
	glfwSetErrorCallback([](int code, const char *what) { sl::error("{}.", what); });
	if (glfwInit() == GLFW_TRUE) {
		glfwWindowHint(GLFW_VISIBLE, 0);
		if (kane::glfw_window = glfwCreateWindow(1024, 768, "GLFW", 0, 0); kane::glfw_window) {
			glewExperimental = GL_TRUE;
			glfwMakeContextCurrent(kane::glfw_window);
			if (glewInit() == GLEW_OK) {
				sl::debug("OpenGL {} via {}.", glGetString(GL_VERSION), glGetString(GL_RENDERER));
				if (auto nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES); nvg) {
					glfwSwapInterval(1);
					glfwShowWindow(kane::glfw_window);
					kane::run(nvg);
					glfwHideWindow(kane::glfw_window);
					nvgDeleteGL3(nvg);
				} else sl::error("Unable to initialize NanoVG.");
			} else sl::error("Failed to load OpenGL functioanlity.");
			glfwDestroyWindow(kane::glfw_window);
		} else sl::error("Unable to create window.");
		glfwTerminate();
	} else sl::error("Unable to initialize GLFW.");
	kane::settings::save();
	return 0;
}