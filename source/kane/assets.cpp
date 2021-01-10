#include <kane/assets.h>
#include <kane/logging.h>

#include <nanovg.h>
#include <stb_image.h>
#include <GL/glew.h>

#include <emico.h>

#include <fstream>
#include <filesystem>

std::map<std::string, int> kane::assets::sprite_sheet_gl_handles;
std::map<std::string, std::pair<glm::ivec2, std::vector<uint8_t>>> kane::assets::sprite_sheet_pixel_data;

namespace kane::assets {
	void prepare_fonts(NVGcontext *nvg) {
		for (auto &emb : emico::assets) {
			if (emb.first.substr(0, 6) != "fonts.") continue;
			auto name = emb.first.substr(6, emb.first.size() - 4 - 6);
			auto fnt = nvgCreateFontMem(nvg, name.c_str(), (unsigned char *)emb.second.first, emb.second.second, 0);
			if (fnt == -1) sl::warn("Failed to prepare font from embedded assets: {} -> {}", emb.first, name);
			else sl::debug("Prepared font from embedded assets: {} -> {} (font #{})", emb.first, name, fnt);
		}
	}
}

bool kane::assets::load(NVGcontext *nvg) {
	prepare_fonts(nvg);
	for (auto &emb : emico::assets) {
		if (emb.first.substr(0, strlen("sprites.")) != "sprites.") continue;
		int w, h, num_chan;
		auto pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(emb.second.first), emb.second.second, &w, &h, &num_chan, 4);
		assert(pixels);
		int texture = 0;
		auto name = emb.first;
		name = name.substr(strlen("sprites."));
		name = name.substr(0, name.size() - strlen(".png"));
		sl::debug("Referencing embedded asset as sprite: {} -> {}", emb.first, name);
		{
			auto &convenient_img_copy = sprite_sheet_pixel_data[name];
			convenient_img_copy.first = { w, h };
			convenient_img_copy.second.resize(w * h * 4);
			memcpy(convenient_img_copy.second.data(), pixels, convenient_img_copy.second.size());
			texture = nvgCreateImageRGBA(nvg, w, h, 0, pixels);
			stbi_image_free(pixels);
		}
		if (!texture) {
			sl::warn("Unable to create texture from image pixels: {}", emb.first);
			continue;
		}
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