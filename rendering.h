#pragma once

#include <glm/vec2.hpp>

#include "nanovg.h"

namespace kane::rendering {

	struct sprite {
		int sheet_img;
		glm::ivec2 tile_pos { 0, 0 };
		glm::ivec2 tile_size { 0, 0 };
		glm::ivec2 tile_off { 0, 0 };
		glm::ivec2 sheet_off { 0, 0 };
		glm::ivec2 sheet_size { 0, 0 };
	};

	bool initialize(NVGcontext *nvg);
	void render(NVGcontext *nvg);
	void shutdown(NVGcontext *nvg);
}