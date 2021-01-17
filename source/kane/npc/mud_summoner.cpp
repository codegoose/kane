#include <kane/npc/mud_summoner.h>
#include <kane/npc/mud_minion.h>
#include <kane/local_player.h>
#include <kane/logging.h>

#include <glm/glm.hpp>

kane::npc::mud_summoner::mud_summoner() : game::entity("mud_summoner_npc") {
	current_anim = "mud_summoner_idle";
	anims["mud_summoner_idle"];
	anims["mud_summoner_run"];
	anims["mud_summoner_summon"];
	team = game::alliance::hoard;
}

kane::npc::mud_summoner::~mud_summoner() {
	
}

void kane::npc::mud_summoner::anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) {
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
		anim.next_anim = "mud_summoner_idle";
	}
}

void kane::npc::mud_summoner::anim_frame_cb(int frame) {

}

void kane::npc::mud_summoner::update_cb(double secs) {
	if (summoning && current_anim == "mud_summoner_idle") {
		summoning = false;
		since_last_summon = 0;
		auto minion_1 = std::make_shared<npc::mud_minion>();
		auto minion_2 = std::make_shared<npc::mud_minion>();
		minion_1->location = location + glm::vec2(21, 2);
		minion_2->location = location - glm::vec2(22, -2);
		game::add_entity(minion_1);
		game::add_entity(minion_2);
	}
	if (current_anim != "mud_summoner_summon" && lp::entity) since_last_summon += secs;
	auto distance = lp::entity ? glm::distance(location, lp::entity->location) : 10000.f;
	if (distance < 200.f && !summoning) {
		if (location.x > lp::entity->location.x) sprite_flipped = false;
		if (location.x < lp::entity->location.x) sprite_flipped = true;
		current_anim = "mud_summoner_run";
		location.x += (sprite_flipped ? -40.f : 40.f) * secs;
	} else {
		if (since_last_summon >= 4.f) {
			current_anim = "mud_summoner_summon";
			summoning = true;
			since_last_summon = 0;
		} else if (!summoning) {
			current_anim = "mud_summoner_idle";
			since_last_misc += secs;
		}
	}
}

void kane::npc::mud_summoner::render_location_cb(glm::vec2 &location_out) {
	location_out = location;
}