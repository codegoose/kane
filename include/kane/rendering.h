#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

#include <glm/vec2.hpp>

struct NVGcontext;

namespace kane::rendering {

	struct sprite {
		int sheet_img = 0;
		float alpha = 1;
		glm::ivec2 tile_pos { 0, 0 };
		glm::ivec2 tile_size { 0, 0 };
		glm::ivec2 tile_off { 0, 0 };
		glm::ivec2 sheet_off { 0, 0 };
		glm::ivec2 sheet_size { 0, 0 };
	};

	struct animation : sprite {
		int steps_left_to_wait { 0 };
		int num_wait_steps { 1 };
		std::vector<glm::ivec2> frame_xy_list;
		int current_frame { 0 };
	};

	struct entity {
		glm::vec2 pos;
		bool flipped { false };
		std::map<std::string, animation> anims;
		std::map<std::string, std::string> anim_sheet_names;
		std::string current_anim;
		std::string last_rendered_anim;
		std::function<void(double)> update;
		entity();
		~entity();
		virtual void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) = 0;
	};

	bool initialize(NVGcontext *nvg);
	void render(NVGcontext *nvg, glm::ivec2 framebuffer_size);
	void shutdown(NVGcontext *nvg);
}