#include <kane/game.h>
#include <kane/audio.h>
#include <kane/timing.h>
#include <kane/traits.h>
#include <kane/local_player.h>
#include <kane/logging.h>

#include <kane/npc/merchant.h>
#include <kane/npc/mud_summoner.h>
#include <kane/npc/mud_minion.h>

#include <glm/glm.hpp>

#include <memory>

namespace kane::game {

	std::vector<entity *> entities;
}

kane::game::entity::entity(const std::string_view &name) : name(name) {
	entities.push_back(this);
	sl::debug("New game entity created: {} ({})", name, reinterpret_cast<void *>(this));
}

kane::game::entity::~entity() {
	entities.erase(std::find(entities.begin(), entities.end(), this));
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
	new npc::merchant;
	new npc::mud_summoner;
}

void kane::game::shutdown() {

}