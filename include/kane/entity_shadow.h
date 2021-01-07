#pragma once

#include <kane/rendering.h>

namespace kane::pc {
	struct shadow_entity : kane::rendering::entity {
		shadow_entity();
		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
	};
}