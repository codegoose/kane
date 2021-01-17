#include <kane/local_player.h>
#include <kane/logging.h>
#include <kane/input.h>
#include <kane/audio.h>

#include <glm/glm.hpp>

std::shared_ptr<kane::game::entity> kane::lp::entity;

namespace kane::lp {

	struct shadow_instance : game::entity {

		double idle_time = 0;
		int sound_slot_charging = -1;

		shadow_instance() : game::entity("local_player") {
			max_health = 300;
			health = max_health;
			invulnerable = false;
			current_anim = "shadow_idle";
			anims["shadow_idle"];
			anims["shadow_run"];
			anims["shadow_charge"];
			anims["shadow_damage"];
			anims["shadow_attack_1"];
			anims["shadow_attack_2"];
		}

		virtual ~shadow_instance() {

		}

		void anim_sheet_assign_cb(std::string anim_name, int sprite_sheet_gl_handle, glm::ivec2 sheet_size) override {
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
				anim.loop = false;
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

		void anim_frame_cb(int frame) override {
			if (current_anim == "shadow_attack_1") {
				switch (frame) {
					case 0:
						audio::play_sound("electric_impact_long_1", 50);
						break;
					case 8:
						audio::play_sound("electric_short_1", 80);
						game::emit_damage(game::alliance::hoard | game::alliance::civilian | game::alliance::inanimate, location, 50.f, 100, fmt::format("{}:{}", name, current_anim));
						break;
				}
			}
			if (current_anim == "shadow_attack_2") {
				switch (frame) {
					case 2:
						audio::play_sound("electric_impact_3", 80);
						if (sprite_flipped) game::emit_damage(game::alliance::hoard | game::alliance::civilian | game::alliance::inanimate, { location.x - 100,  location.y - 3 }, { location.x, location.y + 20}, 100, fmt::format("{}:{}", name, current_anim));
						else game::emit_damage(game::alliance::hoard | game::alliance::civilian | game::alliance::inanimate, { location.x,  location.y - 3 }, { location.x + 100, location.y + 20}, 100, fmt::format("{}:{}", name, current_anim));
						break;
				}
			}
			if (current_anim == "shadow_charge") {
				if (sound_slot_charging == -1) sound_slot_charging = audio::play_sound("electric_2_loop", 30, true);
			} else if (sound_slot_charging >= 0) {
				audio::stop_sound(sound_slot_charging);
				sound_slot_charging = -1;
			}
			if (current_anim == "shadow_damage" && frame == 8) {
				remove_from_game = true;
				lp::entity.reset();
			}
		}

		void receive_damage(const std::string_view description, int amount) override {
			if (current_anim == "shadow_attack_1") return;
			health -= amount;
			sl::critical("Player health: {}", health);
			if (health <= 0) current_anim = "shadow_damage";
		}

		void render_location_cb(glm::vec2 &location_out) override {
			location_out = location;
		}

		void update_cb(double secs) override {
			if (current_anim == "shadow_damage") return;
			if (current_anim == "shadow_attack_1" || current_anim == "shadow_attack_2") {
				idle_time = 0;
				return;
			}
			if (input::lp_util_1) {
				current_anim = "shadow_charge";
				for (auto &anim : anims) anim.second.alpha = 1;
				// idle_time = 0;
				return;
			}
			if (input::lp_attack_1) {
				current_anim = "shadow_attack_2";
				return;
			}
			if (input::lp_attack_2) {
				current_anim = "shadow_attack_1";
				return;
			}
			if (input::lp_movement.x != 0) {
				current_anim = "shadow_run";
				sprite_flipped = input::lp_movement.x >= 0 ? false : true;
				location.x += input::lp_movement.x * 50.f * static_cast<float>(secs);
				idle_time = 0;
			} else {
				idle_time += secs;
				if (idle_time > 1.0 / 10.0) current_anim = "shadow_idle";
			}
		}
	};
}

void kane::lp::initialize() {
	entity = std::make_shared<shadow_instance>();
	entity->team = game::alliance::player;
	game::add_entity(entity);
}

void kane::lp::shutdown() {
	game::remove_entity(entity);
	entity.reset();
}