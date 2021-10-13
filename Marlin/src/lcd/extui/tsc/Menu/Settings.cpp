#include "Settings.h"
#include "../TSC_Menu.h"

SETTINGS infoSettings, // Settings para
         lastSettings; // Last Settings para
extern uint32_t TSC_Para[7]; 
uint32_t lastTSC_Para[7];


#ifdef FIL_RUNOUT_PIN
  #define ITEM_RUNOUT_NUM 2
  const ITEM itemRunout[ITEM_RUNOUT_NUM] = {
  // icon                       label
    {ICON_RUNOUT,               LABEL_RUNOUT_OFF},
    {ICON_RUNOUT,               LABEL_RUNOUT_ON},
  };
  const  uint8_t item_runout[ITEM_RUNOUT_NUM] = {FILAMENT_RUNOUT_OFF, FILAMENT_RUNOUT_ON};
  static uint8_t item_runout_i = 0;
#endif


// Reset settings data
void infoSettingsReset(void)
{
  infoSettings.baudrate = 115200;
  infoSettings.language = TFT_ENGLISH;
  infoSettings.mode = SERIAL_TSC;
  infoSettings.runout = 1;
  infoSettings.rotate_ui = 0;
 // infoSettings.bg_color = ST7920_BKCOLOR;
 // infoSettings.font_color = ST7920_FNCOLOR;
  infoSettings.silent = 0;
  infoSettings.auto_off = 0;
}

void loopCheckSettings(void)
{
  static uint32_t lastTime = 0;
  if (millis() - lastTime > 1000)
  {
    lastTime = millis();
    if (memcmp(&infoSettings, &lastSettings, sizeof(SETTINGS))
     || memcmp(lastTSC_Para, TSC_Para, sizeof(TSC_Para)))
    {
      memcpy(&lastSettings, &infoSettings, sizeof(SETTINGS));
      memcpy(lastTSC_Para, TSC_Para, sizeof(TSC_Para));
      storePara();
      //MYSERIAL1.println("Settings stored!");
    }
  }
}

// Version infomation
GUI_RECT lcdFullRect = {0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL};
void menuCallBackInfo(void)
{
  KEY_VALUES key_num = (KEY_VALUES)KEY_GetValue(1, &lcdFullRect);
  if (key_num != KEY_IDLE)
  {
   infoMenu.cur--;
  }
}

void menuInfo(void)
{
  const char* hardware = "Board   : " BOARD_INFO_NAME;
  const char* firmware = "Firmware: " SHORT_BUILD_VERSION " " __DATE__;
  
  uint16_t HW_X = (LCD_WIDTH_PIXEL - GUI_StrPixelWidth((uint8_t *)hardware))/2;
  uint16_t FW_X = (LCD_WIDTH_PIXEL - GUI_StrPixelWidth((uint8_t *)firmware))/2;
  uint16_t centerY = LCD_HEIGHT_PIXEL/2;
  uint16_t startX = min(HW_X, FW_X);  
  GUI_Clear(BK_COLOR);
  GUI_DispString(startX, centerY - BYTE_HEIGHT, (uint8_t *)hardware);
  GUI_DispString(startX, centerY, (uint8_t *)firmware);
  GUI_DispStringInRect(20, LCD_HEIGHT_PIXEL - (BYTE_HEIGHT*2), LCD_WIDTH_PIXEL-20, LCD_HEIGHT_PIXEL, textSelect(LABEL_TOUCH_TO_EXIT));

  TSC_ReDrawIcon = 0; // invalid icon redraw function
  menuSetFrontCallBack(menuCallBackInfo);
}


// MENUITEMS settingsItems = {
// // title
// LABEL_SETTINGS,
// // icon                       label
//  {{ICON_SCREEN_SETTINGS,      LABEL_SCREEN_SETTINGS},
//   {ICON_MACHINE_SETTINGS,     LABEL_MACHINE_SETTINGS},
//   {ICON_FEATURE_SETTINGS,     LABEL_FEATURE_SETTINGS},
//   {ICON_SCREEN_INFO,          LABEL_SCREEN_INFO},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACK,                 LABEL_BACK},}
// };

MENUITEMS settingsItems = {
// title
LABEL_SETTINGS,
// icon                       label
 {{ICON_SCREEN_SETTINGS,      LABEL_SCREEN_SETTINGS},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_SCREEN_INFO,          LABEL_SCREEN_INFO},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};



void menuCallBackSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      infoMenu.menu[++infoMenu.cur] = menuScreenSettings;
      break;
    
    // case KEY_ICON_1: 
    //   infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
    //   break;
    
    // case KEY_ICON_2:
    //   infoMenu.menu[++infoMenu.cur] = menuFeatureSettings;
    //   break;

    #ifdef FIL_RUNOUT_PIN
    case KEY_ICON_1:
      item_runout_i = (item_runout_i + 1) % ITEM_RUNOUT_NUM;                
      settingsItems.items[key_num] = itemRunout[item_runout_i];
      menuDrawItem(&settingsItems.items[key_num], key_num);
      infoSettings.runout = item_runout[item_runout_i];
      ExtUI::setFilamentRunoutEnabled(infoSettings.runout);
      break;
    #endif
    
    case KEY_ICON_2:
      infoMenu.menu[++infoMenu.cur] = menuInfo;
      break;

    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuSettings(void)
{

  #ifdef FIL_RUNOUT_PIN
  for(uint8_t i=0; i<ITEM_RUNOUT_NUM; i++)
  {
    if(infoSettings.runout == item_runout[i])
    {
      item_runout_i = i;
      settingsItems.items[KEY_ICON_1] = itemRunout[item_runout_i];
    }
  }
  #endif  

  menuDrawPage(&settingsItems);
  menuSetFrontCallBack(menuCallBackSettings);
}
