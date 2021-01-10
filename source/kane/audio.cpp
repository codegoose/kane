#include <kane/audio.h>
#include <kane/logging.h>

#include <SFML/Audio.hpp>

#include <emico.h>

#include <memory>

namespace kane::audio {
	std::unique_ptr<sf::SoundBuffer> bg_music_buf;
	std::unique_ptr<sf::Sound> bg_music;
}

void kane::audio::initialize() {
	bg_music_buf = std::make_unique<sf::SoundBuffer>();
	auto &emb = emico::assets["music.low_fog.ogg"];
	if (!bg_music_buf->loadFromMemory(emb.first, emb.second)) {
		sl::warn("Unable to load music.");
		bg_music_buf.release();
		return;
	}
	bg_music = std::make_unique<sf::Sound>();
	bg_music->setBuffer(*bg_music_buf);
	bg_music->setVolume(40);
	bg_music->setLoop(true);
	bg_music->play();
}

void kane::audio::shutdown() {
	bg_music.release();
	bg_music_buf.release();
}