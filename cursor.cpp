
#include "cursor.h"
#include "assets.h"
#include "logging.h"
#include "stb_image_resize.h"

#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

namespace kane {
	extern GLFWwindow *glfw_window;
}

namespace kane::cursor {
	std::map<std::string, GLFWcursor *> named_cells;
}

// https://stackoverflow.com/questions/21572460/resizing-a-picture-vc
static int *make_resized_image(int* pixels,int w1,int h1,int w2,int h2)  {
	int* retval = new int[w2*h2] ;
	int x_ratio = (int)((w1<<16)/w2) +1;
	int y_ratio = (int)((h1<<16)/h2) +1;
	int x2, y2 ;
	for (int i=0;i<h2;i++) {
		for (int j=0;j<w2;j++) {
			x2 = ((j*x_ratio)>>16) ;
			y2 = ((i*y_ratio)>>16) ;
			retval[(i*w2)+j] = pixels[(y2*w1)+x2] ;
		}
	}
	return retval;
}

void kane::cursor::initialize() {
	auto pixels = assets::sprite_sheet_pixel_data.find("cursors");
	if (pixels == assets::sprite_sheet_pixel_data.end()) {
		sl::warn("Unable to apply cursor theme due to missing sprite sheet.");
		return;
	}
	const int scale = 3;
	auto upscaled_sheet = make_resized_image(reinterpret_cast<int *>(pixels->second.second.data()), pixels->second.first.x, pixels->second.first.y, pixels->second.first.x * scale, pixels->second.first.y * scale);
	for (int y = 0; y < (pixels->second.first.y * scale) / (32 * scale); y++) {
		for (int x = 0; x < (pixels->second.first.x * scale) / (32 * scale); x++) {
			std::vector<uint32_t> sprite_data;
			sprite_data.reserve(32 * 32 * scale);
			for (int py = y * 32 * scale; py < (y + 1) * 32 * scale; py++) {
				for (int px = x * 32 * scale; px < (x + 1) * 32 * scale; px++) {
					int pi = (py * (pixels->second.first.x * scale)) + px;
					sprite_data.push_back(upscaled_sheet[pi]);
				}
			}
			GLFWimage img;
			img.width = 32 * scale;
			img.height = 32 * scale;
			img.pixels = reinterpret_cast<uint8_t *>(sprite_data.data());
			auto cursor = glfwCreateCursor(&img, 16 * scale, 16 * scale);
			if (!cursor) {
				sl::warn("Unable to create cursor from cursor sheet cell {},{}.", x, y);
			}
			named_cells[fmt::format("{}-{}", x, y)] = cursor;
		}
	}
	glfwSetCursor(glfw_window, named_cells["0-0"]);
	delete[] upscaled_sheet;
}

void kane::cursor::shutdown() {

}