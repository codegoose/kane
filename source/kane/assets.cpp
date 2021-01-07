#include <kane/assets.h>
#include <kane/logging.h>

#include <nanovg.h>
#include <stb_image.h>
#include <GL/glew.h>

#include <fstream>
#include <filesystem>

std::map<std::string, int> kane::assets::sprite_sheet_gl_handles;
std::map<std::string, std::pair<glm::ivec2, std::vector<uint8_t>>> kane::assets::sprite_sheet_pixel_data;

bool kane::assets::load(NVGcontext *nvg) {
	for (auto &file : std::filesystem::directory_iterator("C:\\Users\\Brandon\\Documents\\Assets\\")) {
		if (file.path().extension() != ".png") continue;
		std::ifstream in(file.path().string().c_str(), std::ios::binary);
		if (!in.is_open()) {
			sl::warn("Unable to open image file: {}", file.path().string());
			continue;
		}
		auto name = file.path().stem().string();
		std::vector<char> contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		int w, h, num_chan;
		auto pixels = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(contents.data()), contents.size(), &w, &h, &num_chan, 4);
		if (!pixels) {
			sl::warn("Unable to process file data as image: {}", file.path().string());
			continue;
		}
		int texture = 0;
		{
			auto &convenient_img_copy = sprite_sheet_pixel_data[name];
			convenient_img_copy.first = { w, h };
			convenient_img_copy.second.resize(w * h * 4);
			memcpy(convenient_img_copy.second.data(), pixels, convenient_img_copy.second.size());
			texture = nvgCreateImageRGBA(nvg, w, h, 0, pixels);
			stbi_image_free(pixels);
		}
		if (!texture) {
			sl::warn("Unable to create texture from image pixels: {}", file.path().string());
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