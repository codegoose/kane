#pragma once

#include <string>

namespace kane::audio {

	void initialize();
	void shutdown();
	int play_sound(const std::string_view &name, float volume = 100.f, bool looping = false);
	void play_music(const std::string_view &name);
}