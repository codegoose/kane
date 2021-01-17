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
	sl::warn("New game entity created: {} ({})", name, reinterpret_cast<void *>(this));
}

kane::game::entity::~entity() {
	sl::warn("Destroying game entity: {} ({})", name, reinterpret_cast<void *>(this));
}

namespace kane::game {

	void update_varying(double secs) {
		auto copies = entities;
		for (auto &cent : copies) cent->update_cb(secs);
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

void kane::game::shutdown() {
	for (auto &ent : entities) {
		if (ent.use_count() > 1) {
			sl::warn("Entity has many references on game shutdown: {} ({}), {} references", ent->name, reinterpret_cast<void *>(ent.get()), ent.use_count());
		}
	}
	entities.clear();
}