#include "assets.h"
#include "nanovg.h"

#include <GL/glew.h>

std::map<std::string, int> kane::assets::sprite_sheet_gl_handles;

bool kane::assets::load(NVGcontext *nvg) {
	int texture = nvgCreateImage(nvg, "C:\\Users\\Brandon\\Documents\\Assets\\merchant-idle.png", 0);
	sprite_sheet_gl_handles["merchant-idle"] = texture;
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void kane::assets::destroy(NVGcontext *nvg) {
	for (auto &E : sprite_sheet_gl_handles) nvgDeleteImage(nvg, E.second);
	sprite_sheet_gl_handles.clear();
}