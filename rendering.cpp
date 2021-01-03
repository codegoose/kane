#include "rendering.h"
#include "logging.h"
#include "nanovg.h"
#include "settings.h"
#include "assets.h"

kane::rendering::sprite merchant_idle;

bool kane::rendering::initialize(NVGcontext *nvg) {
	merchant_idle.sheet_img = assets::sprite_sheet_gl_handles["merchant-idle"];
	nvgImageSize(nvg, merchant_idle.sheet_img, &merchant_idle.sheet_size.x, &merchant_idle.sheet_size.y);
	merchant_idle.tile_size = {
		merchant_idle.sheet_size.x / 4,
		merchant_idle.sheet_size.y
	};
	merchant_idle.tile_off = {
		-merchant_idle.tile_size.x / 2,
		-merchant_idle.tile_size.y / 2
	};
	settings::persist["rendering-init-num"] = settings::persist.value("rendering-init-num", 0) + 1;
	return true;
}

void kane::rendering::render(NVGcontext *nvg) {
	nvgBeginPath(nvg);
	nvgRect(nvg, merchant_idle.tile_pos.x + merchant_idle.tile_off.x, merchant_idle.tile_pos.y + merchant_idle.tile_off.y, merchant_idle.tile_size.x, merchant_idle.tile_size.y);
	nvgFillPaint(nvg,
		nvgImagePattern(
			nvg,
			merchant_idle.tile_pos.x + merchant_idle.tile_off.x + merchant_idle.sheet_off.x,
			merchant_idle.tile_pos.y + merchant_idle.tile_off.y + merchant_idle.sheet_off.y,
			merchant_idle.sheet_size.x,
			merchant_idle.sheet_size.y,
			0,
			merchant_idle.sheet_img,
			1
		)
	);
	nvgFill(nvg);
}

void kane::rendering::shutdown(NVGcontext *nvg) {

}