#include <kane/audio.h>
#include <kane/logging.h>

#include <SFML/Audio.hpp>

#include <emico.h>

#include <map>
#include <array>
#include <memory>

namespace kane::audio {
	std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sound_buffers;
	std::map<std::string, std::shared_ptr<sf::Music>> music_instances;
	std::array<std::shared_ptr<sf::Sound>, 200> active_sounds;
}

void kane::audio::initialize() {
	for (auto &emb : emico::assets) {
		if (emb.first.substr(0, 7) != "sounds.") continue;
		auto name = emb.first;
		name = name.substr(7, name.size() - 7 - 4);
		sl::debug("Referencing embedded asset as sound buffer: {} -> {}", emb.first, name);
		auto buffer = std::make_shared<sf::SoundBuffer>();
		if (!buffer->loadFromMemory(emb.second.first, emb.second.second)) {
			sl::warn("Unable to process embedded asset as sound: {}", emb.first);
			continue;
		}
		sound_buffers[name] = buffer;
	}
	for (auto &emb : emico::assets) {
		if (emb.first.substr(0, 6) != "music.") continue;
		auto name = emb.first;
		name = name.substr(6, name.size() - 6 - 4);
		sl::debug("Referencing embedded asset as music stream: {} -> {}", emb.first, name);
		auto music = std::make_shared<sf::Music>();
		if (!music->openFromMemory(emb.second.first, emb.second.second)) {
			sl::warn("Unable to process embedded asset as music stream: {}", emb.first);
			continue;
		}
		music_instances[name] = music;
	}
}

void kane::audio::garbage_collect() {
	for (int i = 0; i < active_sounds.size(); i++) {
		if (!active_sounds[i] || active_sounds[i]->getStatus() == sf::Sound::Playing) continue;
		sl::debug("Clearing sound slot: {}", i);
		active_sounds[i].reset();
	}
}

void kane::audio::shutdown() {
	for (auto &sound_slot : active_sounds) {
		if (!sound_slot) continue;
		sound_slot->stop();
		sound_slot.reset();
	}
	sound_buffers.clear();
	music_instances.clear();
}

int kane::audio::play_sound(const std::string_view &name, float volume, bool looping) {
	auto buffer_i = sound_buffers.find(name.data());
	if (buffer_i == sound_buffers.end()) {
		sl::warn("Instructed to play sound that doesn't exist: {}", name);
		return -1;
	}
	for (int i = 0; i < active_sounds.size(); i++) {
		auto &sound_slot = active_sounds[i];
		if (sound_slot) continue;
		sound_slot = std::make_shared<sf::Sound>(*buffer_i->second.get());
		sound_slot->play();
		sound_slot->setVolume(volume);
		sound_slot->setLoop(looping);
		sl::debug("Playing sound: {} (Slot #{})", name, i);
		if (looping) return i;
		else return -1;
	}
	sl::warn("Unable to play sound: {} (Too many sounds playing.)", name);
	return -1;
}

void kane::audio::stop_sound(int slot) {
	if (slot < 0 || slot >= active_sounds.size()) {
		sl::warn("Referencing invalid slot when trying to stop a sound: {}", slot);
		return;
	}
	if (active_sounds[slot] && active_sounds[slot]->getStatus() == sf::Sound::Playing) {
		sl::debug("Stopping sound in slot: {}", slot);
		active_sounds[slot]->stop();
		active_sounds[slot].reset();
		return;
	}
}

void kane::audio::play_music(const std::string_view &name) {
	if (music_instances.find(name.data()) == music_instances.end()) {
		sl::warn("Instructed to play music that doesn't exist: {}");
		return;
	}
	for (auto &music : music_instances) {
		if (music.first != name) {
			if (music.second->getStatus() == sf::Music::Playing) {
				sl::warn("Stopping music: {}", name);
				music.second->stop();
			}
			continue;
		}
		if (music.second->getStatus() == sf::Music::Playing) sl::warn("Instructed to play music that's already playing: {}", name);
		else sl::debug("Playing music: {}", name);
		music.second->play();
	}
}