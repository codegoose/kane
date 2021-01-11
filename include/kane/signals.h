#pragma once

#include <any>
#include <string>
#include <functional>

#include <glm/vec2.hpp>

namespace kane::signals {

	struct source {
		std::string id, what;
	};

	struct receiver {
		receiver();
		~receiver();
		virtual void receive_signal(const source &src, const std::any &info) = 0;
	};

	struct emitter {
		std::string signalling_id;
		void emit_signal(const std::string &what, std::any info);
	};

	using signal_cb_type = std::function<void(const source &src, const std::any &info)>;
}