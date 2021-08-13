#include "Heat.h"
#include "../TSC_Menu.h"

#define ZREO_COLOR LIME

//1 title, ITEM_PER_PAGE items (icon + label) 
MENUITEMS heatItems = {
// title
LABEL_HEAT,
// icon                       label
 {{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_5_DEGREE,             LABEL_5_DEGREE},
  {ICON_STOP,                 LABEL_STOP},
  {ICON_BACK,                 LABEL_BACK},}
};


const ITEM itemTool[] = {
// icon                       label
  {ICON_BED,                  LABEL_BED},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
  {ICON_NOZZLE,               LABEL_NOZZLE},
};    

#define ITEM_DEGREE_NUM 3
const ITEM itemDegree[ITEM_DEGREE_NUM] = {
// icon                       label
  {ICON_1_DEGREE,             LABEL_1_DEGREE},
  {ICON_5_DEGREE,             LABEL_5_DEGREE},
  {ICON_10_DEGREE,            LABEL_10_DEGREE},
};
const  uint8_t item_degree[ITEM_DEGREE_NUM] = {1, 5, 10};
static uint8_t item_degree_i = 1;

const char* const heatDisplayID[] = HEAT_DISPLAY_ID;
static uint8_t heaterIndex = 1; // 0: Bed, 1: T0, 2: T1, etc

void heatSetCurHeaterIndex(uint8_t index)
{
  heaterIndex = index;
}

int16_t getCurrentTemp(void) {
  int16_t curTemp = heaterIndex ? thermalManager.degHotend(heaterIndex-1) : thermalManager.degBed();
  return curTemp;
}

int16_t getRealTargetTemp(void) {
  int16_t tarTemp = heaterIndex ? thermalManager.degTargetHotend(heaterIndex-1) : thermalManager.degTargetBed();
  return tarTemp;
}

int16_t getTargetTemp(void) {
  int16_t tarTemp = heaterIndex ? thermalManager.degTargetHotend(heaterIndex-1) : thermalManager.degTargetBed();
  if (tarTemp == 0) {
    tarTemp = heaterIndex ? 180 :50;
  }
  return tarTemp;
}

void setTargetTemp(int16_t tmp) {
  if (tmp < 0) return;
  if (heaterIndex) {    
    thermalManager.setTargetHotend(tmp, heaterIndex - 1);
  } else {
    thermalManager.setTargetBed(tmp);
  }
}

void showTemperature(void)
{
  const GUI_RECT rect = {exhibitRect.x0, (int16_t)(CENTER_Y-BYTE_HEIGHT), exhibitRect.x1, CENTER_Y};
  GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);
  GUI_DispStringInPrect(&rect, (uint8_t*)heatDisplayID[heaterIndex]);
  GUI_DispDec(CENTER_X-BYTE_WIDTH*3, CENTER_Y, getCurrentTemp(), 3, RIGHT);
  GUI_DispString(CENTER_X, CENTER_Y, (uint8_t*)"/");
  if (getRealTargetTemp() == 0) GUI_SetColor(ZREO_COLOR);
  GUI_DispDec(CENTER_X+BYTE_WIDTH*1, CENTER_Y, getTargetTemp(), 3, LEFT);
  GUI_SetColor(FK_COLOR);
}

void currentReDraw(void)
{
  GUI_DispDec(CENTER_X-BYTE_WIDTH*3, CENTER_Y, getCurrentTemp(), 3, RIGHT);
}

void targetReDraw(void)
{
  if (getRealTargetTemp() == 0) GUI_SetColor(ZREO_COLOR);
  GUI_DispDec(CENTER_X+BYTE_WIDTH*1, CENTER_Y, getTargetTemp(), 3, LEFT);
  GUI_SetColor(FK_COLOR);
}


void menuCallBackHeat(void)
{
  static int16_t curTemp = 0, tarTemp = 0;
  KEY_VALUES  key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
        setTargetTemp(getTargetTemp() - item_degree[item_degree_i]);
      break;
    
    case KEY_ICON_3:
        setTargetTemp(getTargetTemp() + item_degree[item_degree_i]);
      break;
      
    case KEY_ICON_4:
      heaterIndex = (heaterIndex+1) % 2;
      heatItems.items[key_num] = itemTool[heaterIndex];
      menuDrawItem(&heatItems.items[key_num], key_num);
      showTemperature();
      break;
    
    case KEY_ICON_5:
      item_degree_i = (item_degree_i+1) % ITEM_DEGREE_NUM;
      heatItems.items[key_num] = itemDegree[item_degree_i];
      menuDrawItem(&heatItems.items[key_num], key_num);
      break;
    
    case KEY_ICON_6:
      setTargetTemp(0);
      break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default :
      break;
  }
  static uint32_t nowTime_ms = 0;
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    if(curTemp != getCurrentTemp())
    {      
      curTemp = getCurrentTemp();
      currentReDraw();
    }
  }
  if(tarTemp != getRealTargetTemp())
  {
    tarTemp = getRealTargetTemp();
    targetReDraw();
  }
}

void menuHeat(void)
{
  heatItems.items[KEY_ICON_4] = itemTool[heaterIndex];
  menuDrawPage(&heatItems);
  showTemperature();
  menuSetFrontCallBack(menuCallBackHeat);
}
