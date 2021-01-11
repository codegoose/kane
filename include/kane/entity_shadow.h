#pragma once

#include <kane/rendering.h>
#include <kane/signals.h>

namespace kane::pc {
	struct shadow_entity : kane::rendering::entity, kane::signals::emitter {
		int sound_slot_charging = -1;
		shadow_entity();
		~shadow_entity();
		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override;
		void anim_frame_cb(int frame) override;
	};
}