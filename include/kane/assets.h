#pragma once

#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <utility>

#include <glm/vec2.hpp>

struct NVGcontext;

namespace kane::assets {

	extern std::map<std::string, int> sprite_sheet_gl_handles;
	extern std::map<std::string, std::pair<glm::ivec2, std::vector<uint8_t>>> sprite_sheet_pixel_data;

	bool load(NVGcontext *nvg);
	void destroy(NVGcontext *nvg);
}