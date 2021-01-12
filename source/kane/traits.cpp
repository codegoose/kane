#include <kane/traits.h>
#include <kane/logging.h>
#include <kane/rendering.h>

#include <tuple>

void kane::traits::mortal::receive_signal(const signals::source &src, const std::any &info) {
	if (info.type() == typeid(damage_signal)) {
		receive_damage(src, std::any_cast<damage_signal>(info).amount);
	} else if (info.type() == typeid(damage_zone_rect_signal)) {
		auto i = std::any_cast<damage_zone_rect_signal>(info);
		receive_damage_zone_rect(src, i);
		rendering::spawn_debug_box(fmt::format("{}, {}", src.id, src.what), i.min, i.max, { 0, 1, 1, .3f }, 2);
	} else if (info.type() == typeid(damage_zone_radius_signal)) {
		auto i = std::any_cast<damage_zone_radius_signal>(info);
		receive_damage_zone_radius(src, i);
		rendering::spawn_debug_sphere(fmt::format("{}, {}", src.id, src.what), i.location, i.radius, { 0, 1, 1, .3f }, 2);
	}
}