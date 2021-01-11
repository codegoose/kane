#include <kane/entity_mud_summoner.h>

kane::pc::mud_summoner_entity::mud_summoner_entity() {
	current_anim = "mud_summoner_idle";
	anims["mud_summoner_idle"];
	anims["mud_summoner_run"];
	anims["mud_summoner_summon"];
}

kane::pc::mud_summoner_entity::~mud_summoner_entity() {
	
}

void kane::pc::mud_summoner_entity::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "mud_summoner_idle") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x,
			anim.sheet_size.y / 4
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
			{ 0, 3 }
		};
	} else if (anim_name == "mud_summoner_run") {
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
		anim.num_wait_steps = 3;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 }
		};
	} else if (anim_name == "mud_summoner_summon") {
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
		anim.num_wait_steps = 6;
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
		anim.loop = false;
		anim.next_anim = "mud_summoner_idle";
	}
}

void kane::pc::mud_summoner_entity::anim_frame_cb(int frame) {

}