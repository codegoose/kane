#include <kane/game.h>
#include <kane/audio.h>
#include <kane/timing.h>
#include <kane/local_player.h>
#include <kane/logging.h>
#include <kane/npc/merchant.h>
#include <kane/npc/mud_summoner.h>

#include <glm/glm.hpp>

#include <memory>

namespace kane::game {

	std::vector<std::shared_ptr<entity>> entities;
}

kane::game::entity::entity(const std::string_view &name) : name(name) {
	sl::warn("Game entity constructed: {} ({})", name, reinterpret_cast<void *>(this));
}

kane::game::entity::~entity() {
	sl::warn("Game entity destructed: {} ({})", name, reinterpret_cast<void *>(this));
}

void kane::game::entity::update_cb(const double secs) {

}

void kane::game::entity::receive_damage(const std::string_view description, int amount) {

}

bool kane::game::entity::exists_in_game() {
	for (auto &ent : entities) {
		if (ent.get() != this) continue;
		return true;
	}
	return false;
}

namespace kane::game {

	void update_varying(double secs) {
		auto entities_snapshot = entities;
		for (auto &ent : entities_snapshot) {
			ent->update_cb(secs);
			if (!ent->remove_from_game) continue;
			sl::warn("Entity marked for removal: {} ({})", ent->name, reinterpret_cast<void *>(ent.get()));
			remove_entity(ent);
		}
	}
}

void kane::game::initialize() {
	if (static bool first = true; first) {
		timing::subscribe(update_varying, true);
		first = false;
	}
	audio::play_music("low_fog");
	{
		auto guy = std::make_shared<npc::merchant>();
		guy->location = { 100, 0 };
		add_entity(guy);
	}
	{
		auto guy = std::make_shared<npc::mud_summoner>();
		guy->location = { -200, 0 };
		add_entity(guy);
	}
	{
		auto guy = std::make_shared<npc::mud_summoner>();
		guy->location = { -100, 0 };
		add_entity(guy);
	}
}

void kane::game::add_entity(std::shared_ptr<entity> new_entity) {
	sl::warn("Adding entity to game world: {} ({})", new_entity->name, reinterpret_cast<void *>(new_entity.get()));
	remove_entity(new_entity);
	entities.push_back(new_entity);
}

void kane::game::remove_entity(std::shared_ptr<entity> existing_entity) {
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i].get() != existing_entity.get()) continue;
		sl::warn("Removing entity from game world: {} ({})", entities[i]->name, reinterpret_cast<void *>(entities[i].get()));
		entities.erase(entities.begin() + i);
		i--;
	}
}

void kane::game::emit_damage(int affected_alliances, glm::vec2 location, float radius, int amount, const std::string_view &description) {
	auto entities_snapshot = entities;
	for (auto &ent : entities_snapshot) {
		if ((affected_alliances & ent->team) == 0) continue;
		if (glm::distance(location, ent->location) > radius) continue;
		sl::warn("Emitted damage affects {} ({}, {}).", ent->name, reinterpret_cast<void *>(ent.get()), description);
		ent->receive_damage(description, amount);
	}
}

void kane::game::emit_damage(int affected_alliances, glm::vec2 min_location, glm::vec2 max_location, int amount, const std::string_view &description) {
	auto entities_snapshot = entities;
}

void kane::game::shutdown() {
	for (auto &ent : entities) {
		if (ent.use_count() > 1) {
			sl::warn("Entity has many references on game shutdown: {} ({}), {} references", ent->name, reinterpret_cast<void *>(ent.get()), ent.use_count());
		}
	}
	entities.clear();
}