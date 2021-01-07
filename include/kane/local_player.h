#pragma once

#include <kane/rendering.h>

#include <memory>

namespace kane::lp {

	extern std::unique_ptr<rendering::entity> entity;

	void initialize();
	void shutdown();
}