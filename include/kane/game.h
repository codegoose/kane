#pragma once

#include <kane/rendering.h>

#include <string>
#include <memory>

#include <glm/vec2.hpp>

namespace kane::game {

	struct entity : rendering::entity {

		const std::string name;

		int max_health = 100;
		int health = 100;
		bool invulnerable = true;

		entity(const std::string_view &name);
		virtual ~entity();

		glm::vec2 location { 0, 0 };

		virtual void update_cb(const double secs) = 0;
	};

	void initialize();
	void add_entity(std::shared_ptr<entity> new_entity);
	void remove_entity(std::shared_ptr<entity> existing_entity);
	void shutdown();
}