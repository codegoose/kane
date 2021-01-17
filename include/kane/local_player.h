#pragma once

#include <kane/game.h>

#include <memory>

namespace kane::lp {

	extern std::shared_ptr<game::entity> entity;

	void initialize();
	void shutdown();
}