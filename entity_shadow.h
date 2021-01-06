#pragma once

#include "rendering.h"

namespace kane::entity {
	struct shadow_entity : kane::rendering::entity {
		float state = 0.f;
		shadow_entity();
		void update(double secs) override;
		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
	};
}