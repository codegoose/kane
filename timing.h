#pragma once

#include <chrono>
#include <functional>

namespace kane::timing {

	void reset();
	void tick();
	void subscribe(std::function<void(double secs)> callback, bool varying=false);
}