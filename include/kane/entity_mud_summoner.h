#pragma once

#include <kane/rendering.h>

namespace kane::pc {
	struct mud_summoner_entity : kane::rendering::entity {
		mud_summoner_entity();
		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
	};
}