#include <kane/game.h>
#include <kane/audio.h>
#include <kane/timing.h>
#include <kane/local_player.h>
#include <kane/entity_merchant.h>
#include <kane/entity_mud_summoner.h>

#include <glm/glm.hpp>

namespace kane::game {

	pc::merchant_entity *merchant = 0;
	pc::mud_summoner_entity *summoner = 0;

	void update_varying(double secs) {
		if (lp::entity && lp::entity->update) lp::entity->update(secs);
		if (merchant && merchant->update) merchant->update(secs);
		if (summoner && summoner->update) summoner->update(secs);
	}

	void make_random_merchant() {
		merchant = new pc::merchant_entity;
		merchant->pos.x = 100;
		merchant->update = [](double secs) {
			static float tmp = 0;
			tmp += secs;
			if (tmp < 3.f) merchant->current_anim = "merchant_idle";
			else if (tmp < 6.f) {
				merchant->current_anim = "merchant_walk";
				merchant->pos.x += (merchant->flipped ? -20.f : 20.f) * secs;
			} else {
				tmp = 0;
				merchant->current_anim = "merchant_idle";
			}
			if (merchant->pos.x > 30) merchant->flipped = true;
			if (merchant->pos.x < -30) merchant->flipped = false;
		};
	}

	void make_random_mud_summoner() {
		summoner = new pc::mud_summoner_entity;
		summoner->pos.x = -100;
		summoner->update = [](double secs) {
			static bool summoning = false;
			static float since_last_summon = 0.f;
			static float since_last_misc = 0.f;
			if (summoning && summoner->current_anim == "summoner_idle") {
				summoning = false;
				since_last_summon = 0;
			}
			if (summoner->current_anim != "mud_summoner_summon") since_last_summon += secs;
			auto distance = glm::distance(summoner->pos, lp::entity->pos);
			if (distance < 200.f) {
				if (summoner->pos.x > lp::entity->pos.x) summoner->flipped = false;
				if (summoner->pos.x < lp::entity->pos.x) summoner->flipped = true;
				summoner->current_anim = "mud_summoner_run";
				summoner->pos.x += (summoner->flipped ? -40.f : 40.f) * secs;
			} else {
				if (since_last_summon >= 4.f) {
					summoner->current_anim = "mud_summoner_summon";
					summoning = true;
					since_last_summon = 0;
				} else if (!summoning) {
					summoner->current_anim = "mud_summoner_idle";
					since_last_misc += secs;
				}
			}
		};
	}
}

void kane::game::initialize() {
	if (static bool first = true; first) {
		timing::subscribe(update_varying, true);
		first = false;
	}
	make_random_merchant();
	make_random_mud_summoner();
	audio::play_music("low_fog");
}

void kane::game::shutdown() {
	delete merchant;
	delete summoner;
}