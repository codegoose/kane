#pragma once

#include <map>
#include <string>

#include "nanovg.h"

namespace kane::assets {

	extern std::map<std::string, int> sprite_sheet_gl_handles;

	bool load(NVGcontext *nvg);
	void destroy(NVGcontext *nvg);
}