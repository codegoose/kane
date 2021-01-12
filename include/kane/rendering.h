#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

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
		bool loop = true;
		std::string next_anim;
	};

	struct entity {
		bool sprite_flipped { false };
		std::map<std::string, animation> anims;
		std::map<std::string, std::string> anim_sheet_names;
		std::string current_anim, last_stepped_animation;
		entity();
		virtual ~entity();
		virtual void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) = 0;
		virtual void anim_frame_cb(int frame) = 0;
		virtual void render_location_cb(glm::vec2 &location_out) = 0;
	};

	bool initialize(NVGcontext *nvg);
	void render(NVGcontext *nvg, glm::ivec2 framebuffer_size);
	void shutdown(NVGcontext *nvg);
	void spawn_debug_box(const std::string &name, const glm::vec2 &min, const glm::vec2 &max, const glm::vec4 &color, float secs);
	void spawn_debug_sphere(const std::string &name, const glm::vec2 &location, float radius, const glm::vec4 &color, float secs);
}