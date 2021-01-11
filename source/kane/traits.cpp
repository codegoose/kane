#include <kane/traits.h>
#include <kane/logging.h>

#include <tuple>

void kane::traits::mortal::receive_signal(const signals::source &src, const std::any &info) {
	if (info.type() == typeid(damage_signal)) receive_damage(src, std::any_cast<damage_signal>(info).amount);
	else if (info.type() == typeid(damage_zone_rect_signal)) receive_damage_zone_rect(src, std::any_cast<damage_zone_rect_signal>(info));
	else if (info.type() == typeid(damage_zone_radius_signal)) receive_damage_zone_radius(src, std::any_cast<damage_zone_radius_signal>(info));
}