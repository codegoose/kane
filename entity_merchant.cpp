#include "entity_merchant.h"

kane::entity::merchant_entity::merchant_entity() {
	current_anim = "merchant-idle";
	anims["merchant-idle"];
	anims["merchant-walk"];
}

void kane::entity::merchant_entity::update(double secs) {
	current_anim = "merchant-walk";
	pos.x += (flipped ? -15 : 15) * secs;
	if (pos.x > 50) flipped = true;
	if (pos.x < -50) flipped = false;
}

void kane::entity::merchant_entity::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "merchant-idle") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x / 4,
			anim.sheet_size.y
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y / 2
		};
		anim.num_wait_steps = 6;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 }
		};
	} else if (anim_name == "merchant-walk") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x / 5,
			anim.sheet_size.y
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y / 2
		};
		anim.num_wait_steps = 3;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 },
			{ 4, 0 }
		};
	}
}