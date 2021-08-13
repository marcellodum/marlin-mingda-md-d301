#include "../TSC_Menu.h"
#include "Extrude.h"

//1title, ITEM_PER_PAGE items
MENUITEMS extrudeItems = {
// title
LABEL_EXTRUDE,
// icon                       label
 {{ICON_UNLOAD,               LABEL_UNLOAD},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_LOAD,                 LABEL_LOAD},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_BACK,                 LABEL_BACK},}
};

#define ITEM_EXTRUDER_NUM EXTRUDERS
static ExtUI::extruder_t item_extruder_i = ExtUI::E0;

#define ITEM_SPEED_NUM 3
const ITEM itemSpeed[ITEM_SPEED_NUM] = {
// icon                       label
  {ICON_SLOW_SPEED,           LABEL_SLOW_SPEED},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_FAST_SPEED,           LABEL_FAST_SPEED},
};
const  uint32_t item_speed[ITEM_SPEED_NUM] = {EXTRUDE_SLOW_SPEED, EXTRUDE_NORMAL_SPEED, EXTRUDE_FAST_SPEED};
static uint8_t  item_speed_i = 1;

#define ITEM_LEN_NUM 3
const ITEM itemLen[ITEM_LEN_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_MM},
  {ICON_E_5_MM,               LABEL_5_MM},
  {ICON_E_10_MM,              LABEL_10_MM},
};
const  uint8_t item_len[ITEM_LEN_NUM] = {1, 5, 10};
static uint8_t item_len_i = 1;
static float extrudeCoordinate = 0.0f;

void extrudeCoordinateReDraw(void)
{
  const GUI_RECT rect = {exhibitRect.x0, CENTER_Y, exhibitRect.x1, (int16_t)(CENTER_Y+BYTE_HEIGHT)};
  char buf[36];
  sprintf_P(buf, PSTR("%.2f"), extrudeCoordinate);
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)buf);
}

const char* extruderDisplayID[] = EXTRUDER_ID;

void showExtrudeCoordinate(void)
{
  const GUI_RECT rect = {exhibitRect.x0, (int16_t)(CENTER_Y-BYTE_HEIGHT), exhibitRect.x1, CENTER_Y};
  item_extruder_i = ExtUI::getActiveTool();
  extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)extruderDisplayID[item_extruder_i]);
  extrudeCoordinateReDraw();
}

void menuCallBackExtrude(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) - item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
      break;
    
    case KEY_ICON_3:
      ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(item_extruder_i) + item_len[item_len_i], item_extruder_i, item_speed[item_speed_i]);
      break;
    
    case KEY_ICON_4:
      item_extruder_i = (ExtUI::extruder_t)((item_extruder_i + 1) % ITEM_EXTRUDER_NUM);
      showExtrudeCoordinate();
      break;

    case KEY_ICON_5:
      item_speed_i = (item_speed_i+1) % ITEM_SPEED_NUM;
      extrudeItems.items[key_num] = itemSpeed[item_speed_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;
    
    case KEY_ICON_6:
      item_len_i = (item_len_i+1) % ITEM_LEN_NUM;
      extrudeItems.items[key_num] = itemLen[item_len_i];
      menuDrawItem(&extrudeItems.items[key_num], key_num);
      break;

    case KEY_ICON_7: 
      infoMenu.cur--; 
      break;
    
    default:
      break;
  }
  
  if (extrudeCoordinate != ExtUI::getAxisPosition_mm(item_extruder_i)){
    extrudeCoordinate = ExtUI::getAxisPosition_mm(item_extruder_i);
    extrudeCoordinateReDraw();
  }
}

void menuExtrude()
{
  menuDrawPage(&extrudeItems);
  showExtrudeCoordinate();
  menuSetFrontCallBack(menuCallBackExtrude);
}
