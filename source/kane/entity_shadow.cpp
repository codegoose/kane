#include <kane/entity_shadow.h>
#include <kane/logging.h>
#include <kane/audio.h>

kane::pc::shadow_entity::shadow_entity() {
	current_anim = "shadow_idle";
	anims["shadow_idle"];
	anims["shadow_run"];
	anims["shadow_charge"];
	anims["shadow_damage"];
	anims["shadow_attack_1"];
	anims["shadow_attack_2"];
}

void kane::pc::shadow_entity::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "shadow_idle") {
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
	} else if (anim_name == "shadow_run") {
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
	} else if (anim_name == "shadow_charge") {
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
	} else if (anim_name == "shadow_damage") {
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
	} else if (anim_name == "shadow_attack_1") {
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
		anim.loop = false;
		anim.next_anim = "shadow_idle";
	} else if (anim_name == "shadow_attack_2") {
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
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 },
			{ 0, 8 },
			{ 0, 9 }
		};
		anim.loop = false;
		anim.next_anim = "shadow_idle";
	}
}

void kane::pc::shadow_entity::anim_frame_cb(int frame) {
	if (current_anim == "shadow_attack_1") {
		switch (frame) {
			case 0: audio::play_sound("electric_impact_long_1", 50); break;
			case 8: audio::play_sound("electric_short_1", 80); break;
		}
	}
	if (current_anim == "shadow_attack_2") {
		switch (frame) {
			case 2: audio::play_sound("electric_impact_3", 80); break;
		}
	}
	if (current_anim == "shadow_charge") {
		if (sound_slot_charging == -1) sound_slot_charging = audio::play_sound("electric_2_loop", 30, true);
	} else if (sound_slot_charging >= 0) {
		audio::stop_sound(sound_slot_charging);
		sound_slot_charging = -1;
	}
}