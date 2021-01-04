#include "timing.h"
#include "logging.h"

#include <vector>

namespace kane::timing {
	const int ticks_per_sec = 30;
	std::chrono::high_resolution_clock::time_point last_tick;
	std::chrono::nanoseconds leftover_time;
	std::vector<std::function<void(double secs)>> subs;
	std::vector<std::function<void(double secs)>> subs_varying;
}

void kane::timing::reset() {
	last_tick = std::chrono::high_resolution_clock::now();
	leftover_time.zero();
	sl::warn("Timing subsystem reset issued.");
}

void kane::timing::tick() {
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_tick);
	last_tick = now;
	for (auto &F : subs_varying) F(std::chrono::duration<double>(elapsed).count());
	auto ns_per_step = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count() / ticks_per_sec;
	leftover_time += elapsed;
	int num_steps = 0;
	bool capped_steps = false;
	while (leftover_time.count() >= ns_per_step) {
		for (auto &F : subs) F(1.0 / static_cast<double>(ticks_per_sec));
		leftover_time -= std::chrono::nanoseconds(ns_per_step);
		num_steps++;
		if (num_steps >= 8) {
			capped_steps = true;
			break;
		}
	}
	if (num_steps > 1) sl::debug("Loss of time. {} fixed-steps taken that frame. {}ms leftover.", num_steps, std::chrono::duration_cast<std::chrono::milliseconds>(leftover_time).count());
	if (capped_steps) sl::debug("Per-frame fixed-step cap reached. Leftovers for the next frame.");
}

void kane::timing::subscribe(std::function<void(double secs)> callback, bool varying) {
	if (varying) subs_varying.push_back(callback);
	else subs.push_back(callback);
}