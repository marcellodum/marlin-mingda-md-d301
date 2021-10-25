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
  ID_AL_LEVEL = 1,
  ID_AL_PROBE,
  ID_AL_MANUAL_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_AL_LEVEL:
      get_gcode_command(AUTO_LEVELING_COMMAND_ADDR, (uint8_t *)public_buf_m);
      public_buf_m[sizeof(public_buf_m) - 1] = 0;
      queue.inject_P(PSTR(public_buf_m));
      break;
    case ID_AL_PROBE:
      // TODO
      break;
    case ID_AL_MANUAL_RETURN:
      lv_clear_auto_level();
      lv_draw_tool();
      break;
  }
}

void lv_draw_auto_level() {
  scr = lv_screen_create(AUTO_LEVELING_UI);
  lv_big_button_create(scr, "F:/bmp_custom1.bin", auto_leveling_menu.probe, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_AL_PROBE);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_AL_MANUAL_RETURN);
  lv_big_button_create(scr, "F:/bmp_leveling.bin", auto_leveling_menu.leveling, INTERVAL_V, titleHeight, event_handler, ID_AL_LEVEL);
}

void lv_clear_auto_level() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif