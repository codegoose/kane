#include "entity_shadow.h"
#include "logging.h"

kane::entity::shadow_entity::shadow_entity() {
	current_anim = "shadow-idle";
	anims["shadow-idle"];
	anims["shadow-run"];
	anims["shadow-charge"];
	anims["shadow-damage"];
	anims["shadow-attack-1"];
	anims["shadow-attack-2"];
}

void kane::entity::shadow_entity::update(double secs) {
	state += secs;
	sl::info("{}", state);
	if (state < 3.f) {
		current_anim = "shadow-run";
		pos.x += (flipped ? -10 : 10) * secs;
		if (pos.x > 50) flipped = true;
		if (pos.x < -50) flipped = false;
	} else if (state < 6.f) {
		current_anim = "shadow-charge";
	} else if (state < 12.f) {
		current_anim = "shadow-idle";
	} else if (state < 16.f) {
		current_anim = "shadow-attack-1";
	} else if (state < 20.f) {
		current_anim = "shadow-attack-2";
	} else {
		current_anim = "shadow-idle";
	}
}

void kane::entity::shadow_entity::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "shadow-idle") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 1;
		anim.frame_xy_list = {
			{ 0, 0 }
		};
	} else if (anim_name == "shadow-run") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 6
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 4;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 }
		};
	} else if (anim_name == "shadow-charge") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 8
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 2;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 }
		};
	} else if (anim_name == "shadow-damage") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 9
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 4;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 },
			{ 0, 8 }
		};
	} else if (anim_name == "shadow-attack-1") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 22
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 3;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 },
			{ 0, 8 },
			{ 0, 9 },
			{ 0, 10 },
			{ 0, 11 },
			{ 0, 12 },
			{ 0, 13 },
			{ 0, 14 },
			{ 0, 15 },
			{ 0, 16 },
			{ 0, 17 },
			{ 0, 18 },
			{ 0, 19 },
			{ 0, 20 },
			{ 0, 21 }
		};
	} else if (anim_name == "shadow-attack-2") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 10
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 3;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 },
			{ 0, 8 },
			{ 0, 9 }
		};
	}
}