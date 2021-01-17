#include <kane/npc/mud_minion.h>
#include <kane/local_player.h>

#include <glm/glm.hpp>

kane::npc::mud_minion::mud_minion() : game::entity("mud_minion_npc") {
	current_anim = "mud_minion_move";
	anims["mud_minion_move"];
	anims["mud_minion_attack"];
	anims["mud_minion_damage"];
	team = game::alliance::hoard;
}

kane::npc::mud_minion::~mud_minion() {
	
}

void kane::npc::mud_minion::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "mud_minion_move") {
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
		anim.num_wait_steps = 3;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 }
		};
	} else if (anim_name == "mud_minion_attack") {
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
		anim.num_wait_steps = 3;
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
		anim.next_anim = "mud_minion_move";
	} else if (anim_name == "mud_minion_damage") {
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
		anim.num_wait_steps = 3;
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
	}
}

void kane::npc::mud_minion::anim_frame_cb(int frame) {
	if (current_anim == "mud_minion_damage" && frame == 7) remove_from_game = true;
}

void kane::npc::mud_minion::update_cb(double secs) {
	if (current_anim == "mud_minion_damage") return;
	if (current_anim == "mud_minion_attack") return;
	current_anim = "mud_minion_move";
	auto distance = glm::distance(location, lp::entity->location);
	if (distance > 20.f + follow_variance) {
		sprite_flipped = location.x > lp::entity->location.x ? true : false;
		location.x += (sprite_flipped ? -40.f : 40.f) * secs;
	} else current_anim = "mud_minion_attack";
}

void kane::npc::mud_minion::receive_damage(const std::string_view description, int amount) {
	health -= amount;
	current_anim = "mud_minion_damage";
}

void kane::npc::mud_minion::render_location_cb(glm::vec2 &location_out) {
	location_out = location;
}