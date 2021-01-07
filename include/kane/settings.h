#pragma once

#include <nlohmann/json.hpp>

namespace kane::settings {

	extern nlohmann::json persist;

	void load();
	void save();
}