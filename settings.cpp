#include "settings.h"
#include "logging.h"

#include <fstream>

#define KANE_PERSIST_FILE "settings.json"

nlohmann::json kane::settings::persist;

void kane::settings::load() {
	try {
		std::ifstream ifs(KANE_PERSIST_FILE);
		persist = nlohmann::json::parse(ifs);
		if (!persist.is_object()) throw "Configuration file isn't formatted correctly.";
		sl::debug("Loaded configurations.");
		return;
	} catch (const nlohmann::detail::parse_error &pe) {
		sl::warn("Unable to load configurations.");
	} catch (const char *err) {
		sl::error(err);
	}
	sl::warn("Using default configurations.");
	persist = nlohmann::json::parse("{}");
}

void kane::settings::save() {
	std::ofstream ost(KANE_PERSIST_FILE, std::ios::binary);
	ost << persist.dump(1, '\t');
	if (!ost.bad()) sl::debug("Saved configuration data.");
	else sl::warn("Error saving configuration data.");
}