#pragma once

#include <string>

#include <glm/vec2.hpp>

namespace kane::game {

	struct entity {

		const std::string name;

		entity(const std::string_view &name);
		virtual ~entity();

		glm::vec2 bb_min { 0, 0 }, bb_max { 0, 0 };
		glm::vec2 abs_bb_min { 0, 0 }, abs_bb_max { 0, 0 };
		glm::vec2 location { 0, 0 };

		virtual void update_cb(const double secs) = 0;
	};

	void initialize();
	void shutdown();
}