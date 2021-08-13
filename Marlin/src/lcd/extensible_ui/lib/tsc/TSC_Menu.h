#pragma once

#include "../../../../inc/MarlinConfigPre.h"

#include "../../ui_api.h"

#include "../../../../Marlin.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"
// Hardware
#include "Hal/stm32f1_fsmc.h"
#include "Hal/sw_spi.h"
#include "Hal/w25qxx.h"
#include "Hal/xpt2046.h"
#include "Hal/touch_process.h"
#include "Hal/STM32_Flash.h"

// API
#include "Language/utf8_decode.h"
#include "Language/Language.h"
#include "LCD_Init.h"
#include "GUI.h"
#include "boot.h"
#include "flashStore.h"

// Menu
#include "Menu/Popup.h"
#include "Menu/PowerFailed.h"
#include "Menu/UsrConfirm.h"
#include "Menu/menu.h"
#include "Menu/StatusScreen.h"
#include "Menu/MainPage.h"
#include "Menu/PreheatMenu.h"
#include "Menu/Heat.h"
#include "Menu/Move.h"
#include "Menu/Home.h"
#include "Menu/Print.h"
#include "Menu/Printing.h"
#include "Menu/Speed.h"
#include "Menu/BabyStep.h"
#include "Menu/More.h"
#include "Menu/Extrude.h"
#include "Menu/Fan.h"
#include "Menu/Settings.h"
#include "Menu/ScreenSettings.h"
#include "Menu/TSCAdjust.h"
#include "Menu/MachineSettings.h"
#include "Menu/FeatureSettings.h"
#include "Menu/leveling.h"
#include "Menu/ProbeOffset.h"

typedef enum
{
  sound_success,
  sound_error,
  sound_ok,
  sound_cancel,
  sound_notify,
  sound_keypress,
}SOUND;
void Buzzer_play(SOUND sound);

#define MAX_MENU_DEPTH 10       // max sub menu depth
typedef void (*FP_MENU)(void); 

typedef struct
{
  FP_MENU menu[MAX_MENU_DEPTH];  // Menu function buffer
  uint8_t cur;                   // Current menu index in buffer
}MENU;

extern MENU  infoMenu;  // Menu structure

void menuUpdate(void);
void menuSetFrontCallBack(FP_MENU cb);
void LCD_Setup();
bool storeCmd(const char *cmd);
void mustStoreCmd(const char *cmd);
