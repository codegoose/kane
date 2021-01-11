#include <kane/signals.h>
#include <kane/logging.h>

#include <vector>
#include <algorithm>

namespace kane::signals {
	std::vector<receiver *> index;
}

kane::signals::receiver::receiver() {
	index.push_back(this);
}

kane::signals::receiver::~receiver() {
	index.erase(std::find(index.begin(), index.end(), this));
}

void kane::signals::emitter::emit_signal(const std::string &what, std::any info) {
	if (signalling_id.size()) sl::debug("Emitting signal: {}, {}", signalling_id, what);
	else {
		sl::warn("Tried to emit signal using blank signalling id: {}", what);
		return;
	}
	for (auto rxr : index) rxr->receive_signal({ signalling_id, what }, info);
}