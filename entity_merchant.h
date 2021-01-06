#pragma once

#include "rendering.h"

namespace kane::pc {
	struct merchant_entity : kane::rendering::entity {
		merchant_entity();
		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
	};
}