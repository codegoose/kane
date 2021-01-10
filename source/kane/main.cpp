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

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include <kane/logging.h>
#include <kane/rendering.h>
#include <kane/settings.h>
#include <kane/assets.h>
#include <kane/timing.h>
#include <kane/cursor.h>
#include <kane/input.h>
#include <kane/local_player.h>
#include <kane/audio.h>

#include <emico.h>

namespace kane {
	void run(GLFWwindow *window, NVGcontext *nvg) {
		if (!assets::load(nvg)) return;
		cursor::initialize(window);
		if (rendering::initialize(nvg)) {
			input::initialize(window);
			audio::initialize();
			lp::initialize();
			timing::reset();
			glfwShowWindow(window);
			while (!glfwWindowShouldClose(window)) {
				glfwPollEvents();
				input::update(window);
				timing::tick();
				glClearColor(75 * (1.f / 255.f), 74 * (1.f / 255.f), 80.f * (1.f / 255.f), 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				int w, h;
				glfwGetFramebufferSize(window, &w, &h);
				glViewport(0, 0, w, h);
				nvgBeginFrame(nvg, w, h, 1);
				rendering::render(nvg, { w, h });
				nvgEndFrame(nvg);
				glfwSwapBuffers(window);
				glfwSwapInterval(1);
			}
			glfwHideWindow(window);
			lp::shutdown();
			audio::shutdown();
			input::shutdown();
			rendering::shutdown(nvg);
		} else sl::error("Unable to initialize the rendering subsystem.");
		cursor::shutdown(window);
		assets::destroy(nvg);
	}
}

static void center_window_on_primary(GLFWwindow *window) {
	int x, y, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
	int ww, wh;
	glfwGetWindowSize(window, &ww, &wh);
	glfwSetWindowPos(window, x + (w / 2) - (ww / 2), y + (h / 2) - (wh / 2));
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
	for (auto &entry : emico::assets) sl::debug("{} @ {} ({} bytes)", entry.first, entry.second.first, entry.second.second);
	kane::settings::load();
	glfwSetErrorCallback([](int code, const char *what) { sl::error("{}.", what); });
	if (glfwInit() == GLFW_TRUE) {
		glfwWindowHint(GLFW_VISIBLE, 0);
		if (auto window = glfwCreateWindow(1024, 768, "Kane", 0, 0); window) {
			glewExperimental = GL_TRUE;
			glfwMakeContextCurrent(window);
			if (glewInit() == GLEW_OK) {
				sl::debug("OpenGL {} via {}.", glGetString(GL_VERSION), glGetString(GL_RENDERER));
				if (auto nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES); nvg) {
					center_window_on_primary(window);
					kane::run(window, nvg);
					nvgDeleteGL3(nvg);
				} else sl::error("Unable to initialize NanoVG.");
			} else sl::error("Failed to load OpenGL functioanlity.");
			glfwDestroyWindow(window);
		} else sl::error("Unable to create window.");
		glfwTerminate();
	} else sl::error("Unable to initialize GLFW.");
	kane::settings::save();
	return 0;
}