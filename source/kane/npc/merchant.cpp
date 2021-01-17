#include <kane/npc/merchant.h>
#include <kane/local_player.h>

#include <glm/glm.hpp>

kane::npc::merchant::merchant() : game::entity("merchant_npc") {
	current_anim = "merchant_idle";
	anims["merchant_idle"];
	anims["merchant_walk"];
	team = game::alliance::civilian;
}

kane::npc::merchant::~merchant() {
	
}

void kane::npc::merchant::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
	auto &anim = anims[anim_name];
	if (anim_name == "merchant_idle") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x / 4,
			anim.sheet_size.y
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
		};
		anim.num_wait_steps = 6;
		anim.frame_xy_list = {
			{ 0, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 }
		};
	} else if (anim_name == "merchant_walk") {
		anim.sheet_img = sprite_sheet_gl_handle;
		anim.sheet_size = sheet_size;
		anim.tile_size = {
			anim.sheet_size.x / 5,
			anim.sheet_size.y
		};
		anim.tile_off = {
			-anim.tile_size.x / 2,
			-anim.tile_size.y
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

void kane::npc::merchant::anim_frame_cb(int frame) {

}

void kane::npc::merchant::update_cb(double secs) {
	if (running_from_player) {
		if (lp::entity) {
			if (state_timing > 0) {
				sprite_flipped = location.x > lp::entity->location.x ? false : true;
				current_anim = "merchant_walk";
				location.x += (sprite_flipped ? -40.f : 40.f) * secs;
				state_timing -= secs;
				return;
			}
		}
		running_from_player = false;
	}
	state_timing += secs;
	if (state_timing < 3.f) current_anim = "merchant_idle";
	else if (state_timing < 6.f) {
		current_anim = "merchant_walk";
		location.x += (sprite_flipped ? -20.f : 20.f) * secs;
	} else {
		state_timing = 0;
		current_anim = "merchant_idle";
	}
	if (location.x > 30) sprite_flipped = true;
	if (location.x < -30) sprite_flipped = false;
}

void kane::npc::merchant::receive_damage(const std::string_view description, int amount) {
	running_from_player = true;
	state_timing = 4.f;
}

void kane::npc::merchant::render_location_cb(glm::vec2 &location_out) {
	location_out = location;
}