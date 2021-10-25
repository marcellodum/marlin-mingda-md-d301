/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include "../../../gcode/queue.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_PT_DEPLOY = 1,
  ID_PT_STOW,
  ID_PT_MANUAL_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_PT_DEPLOY:
      queue.inject_P(PSTR("M280 P0 S10"));
      break;
    case ID_PT_STOW:
      queue.inject_P(PSTR("M280 P0 S90"));
      break;
    case ID_PT_MANUAL_RETURN:
      lv_clear_probe_tool();
      lv_draw_auto_level();
      break;
  }
}

void lv_draw_probe_tool() {
  scr = lv_screen_create(PROBE_TOOL_UI);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_PT_MANUAL_RETURN);
  lv_big_button_create(scr, "F:/bmp_custom1.bin", probe_tool_menu.stow, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_PT_STOW);
  lv_big_button_create(scr, "F:/bmp_custom2.bin", probe_tool_menu.deploy, INTERVAL_V, titleHeight, event_handler, ID_PT_DEPLOY);
}

void lv_clear_probe_tool() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif