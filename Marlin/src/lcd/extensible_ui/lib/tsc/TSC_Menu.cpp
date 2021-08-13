/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(BTT_FSMC_LCD)

#include "../../ui_api.h"

#include "../../../../Marlin.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../libs/buzzer.h"
#include "../../../../module/printcounter.h"
#include "TSC_Menu.h"


MENU  infoMenu = {.menu = {menuStatus}, .cur = 0};  // Menu structure
FP_MENU menuFrontCallBack = 0; 

TSCBoot boot;


#define Buzzer_TurnOn(fre, ms) BUZZ(ms, fre)
void Buzzer_play(SOUND sound){
switch (sound)
{
case sound_ok:
  Buzzer_TurnOn(3800,40);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(5500,50);
  break;
case sound_success:

  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  break;
case sound_cancel:
  Buzzer_TurnOn(5500,50);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(3800,40);
  break;
  case sound_notify:
  Buzzer_TurnOn(3090,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(4190,50);
  break;
case sound_error:
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
  break;
case sound_keypress:
default:
  Buzzer_TurnOn(LCD_FEEDBACK_FREQUENCY_HZ, LCD_FEEDBACK_FREQUENCY_DURATION_MS);
  break;
}
}

// Store gcode cmd to gcode queue
// If the gcode queue is full, reminde in title bar.
bool storeCmd(const char *cmd)
{  
  if (queue.length >= BUFSIZE) {  
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }
  queue.enqueue_one_now(cmd);
  return true;
}

// Store gcode cmd to gcode queue
// If the gcode queue is full, reminde in title bar,  waiting for available queue and store the command.
void mustStoreCmd(const char *cmd)
{  
  if (queue.length >= BUFSIZE) reminderMessage(LABEL_BUSY, STATUS_BUSY);
  queue.enqueue_one_now(cmd);
}

extern SETTINGS lastSettings; // Last Settings para
extern uint32_t TSC_Para[7],
                lastTSC_Para[7];
void LCD_Setup() {
  XPT2046_Init();
  W25Qxx_Init();
  readStoredPara();
  LCD_Init();
  LOGO_ReadDisplay();
  boot.scanUpdates();
  if(readStoredPara() == false) // Read settings parameter
  {
    TSC_Calibration();
    storePara();
    LOGO_ReadDisplay();
  }
 
  memcpy(&lastSettings, &infoSettings, sizeof(SETTINGS));
  memcpy(lastTSC_Para, TSC_Para, sizeof(TSC_Para));
  ExtUI::setFilamentRunoutEnabled(infoSettings.runout);
}


static bool hasPrintingMenu = false;
void menuUpdate(void) {
  static FP_MENU lastMenu = 0;
  if (lastMenu != infoMenu.menu[infoMenu.cur]) { // Menu switch
    lastMenu = infoMenu.menu[infoMenu.cur];
    lastMenu();
  }

  if (menuFrontCallBack) { // Call back is vaild
    menuFrontCallBack();
  }

  loopFrontEnd();

  if (isPrinting()) {
    if (!hasPrintingMenu) {
      hasPrintingMenu = true;
      infoMenu.menu[++infoMenu.cur] = menuPrinting;
    }
  } else {
    if (hasPrintingMenu) {
      hasPrintingMenu = false;
      while (infoMenu.menu[infoMenu.cur] != menuPrinting && infoMenu.cur) {
        infoMenu.cur--;
      }
      
      if (infoMenu.cur) infoMenu.cur--;
      // Abort print not print completed, need not popup
      if (!card.flag.abort_sd_printing) {
        if(infoSettings.auto_off) {
          // Auto shut down after printing
          infoMenu.menu[++infoMenu.cur] = menuShutDown;
        } else {
          // popup print completed
          Buzzer_play(sound_success);
          popupReminder(textSelect(LABEL_PRINT), textSelect(LABEL_PRINTING_COMPLETED));
        }
      }
    }
  }
}

void menuSetFrontCallBack(FP_MENU cb) {
  menuFrontCallBack = cb;
}
#endif