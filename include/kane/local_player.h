#pragma once

#include <kane/game.h>
#include <kane/rendering.h>

#include <memory>

namespace kane::lp {

	extern std::unique_ptr<game::entity> entity;

	void initialize();
	void shutdown();
}