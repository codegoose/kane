#include "assets.h"
#include "nanovg.h"
#include "logging.h"

#include <filesystem>
#include <GL/glew.h>

std::map<std::string, int> kane::assets::sprite_sheet_gl_handles;

bool kane::assets::load(NVGcontext *nvg) {
	for (auto &file : std::filesystem::directory_iterator("C:\\Users\\Brandon\\Documents\\Assets\\")) {
		if (file.path().extension() != ".png") continue;
		int texture = nvgCreateImage(nvg, file.path().string().c_str(), 0);
		if (texture == 0) {
			sl::warn("Unable to load file as image: {}", file.path().string());
			continue;
		}
		auto name = file.path().stem().string();
		sprite_sheet_gl_handles[name] = texture;
		sl::debug("Loaded {} as GL texture #{}.", name, texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return true;
}

void kane::assets::destroy(NVGcontext *nvg) {
	for (auto &E : sprite_sheet_gl_handles) {
		sl::debug("Destroyed GL texture #{}. ({})", E.second, E.first);
		nvgDeleteImage(nvg, E.second);
	}
	sprite_sheet_gl_handles.clear();
}