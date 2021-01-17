#pragma once

#include <kane/rendering.h>

#include <string>
#include <memory>

#include <glm/vec2.hpp>

namespace kane::game {

	enum alliance : int {
		hoard = 1 << 0,
		civilian = 1 << 1,
		player = 1 << 2,
		inanimate = 1 << 3
	};

	struct entity : rendering::entity {

		const std::string name;

		alliance team = alliance::inanimate;

		int max_health = 100;
		int health = 100;
		bool invulnerable = true;
		bool remove_from_game = false;

		entity(const std::string_view &name);
		virtual ~entity();

		glm::vec2 location { 0, 0 };

		virtual void update_cb(const double secs);
		virtual void receive_damage(const std::string_view description, int amount);

		bool exists_in_game();
	};

	void initialize();
	void add_entity(std::shared_ptr<entity> new_entity);
	void remove_entity(std::shared_ptr<entity> existing_entity);
	void emit_damage(int affected_alliances, glm::vec2 location, float radius, int amount, const std::string_view &description);
	void emit_damage(int affected_alliances, glm::vec2 min_location, glm::vec2 max_location, int amount, const std::string_view &description);
	void shutdown();
}