#include "../TSC_Menu.h"

//1 title, ITEM_PER_PAGE items(icon+label) 
const MENUITEMS fanItems = {
//   title
LABEL_FAN,
// icon                       label
 {{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_FAN ,                 LABEL_FAN},
  {ICON_FAN_FULL_SPEED,       LABEL_FAN_FULL_SPEED},
  {ICON_STOP,                 LABEL_STOP},
  {ICON_BACK,                 LABEL_BACK},}
};

const char* fanID[] = FAN_ID;
static ExtUI::fan_t curIndex = ExtUI::FAN0;
static uint8_t fanPercent = 0;

void showFanSpeed(void)
{
  const GUI_RECT rect = {exhibitRect.x0, (int16_t)(CENTER_Y-BYTE_HEIGHT), exhibitRect.x1, CENTER_Y};
  GUI_ClearPrect(&rect);
  GUI_DispStringInPrect(&rect, (uint8_t *)fanID[curIndex]);
  GUI_DispDec(CENTER_X-2*BYTE_WIDTH, CENTER_Y, fanPercent, 3, RIGHT);
  GUI_DispString(CENTER_X+1*BYTE_WIDTH, CENTER_Y, (uint8_t *)"%");
}

void fanSpeedReDraw(void)
{
  GUI_DispDec(CENTER_X-2*BYTE_WIDTH, CENTER_Y, fanPercent, 3, RIGHT);
}

void menuCallBackFan(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      if (fanPercent > 0)
        ExtUI::setTargetFan_percent(fanPercent-1, curIndex);
      break;
      
    case KEY_ICON_3:
      if (fanPercent < 100)
        ExtUI::setTargetFan_percent(fanPercent+1, curIndex);
      break;
      
    case KEY_ICON_4:
      curIndex = (ExtUI::fan_t)((curIndex + 1) % FAN_COUNT);
      showFanSpeed();
      break;
    
    case KEY_ICON_5:
      ExtUI::setTargetFan_percent(100, curIndex);
      break;
    
    case KEY_ICON_6:
      ExtUI::setTargetFan_percent(0, curIndex);
      break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }

  if(fanPercent != ExtUI::getTargetFan_percent(curIndex))
  {
    fanPercent = ExtUI::getTargetFan_percent(curIndex);
    fanSpeedReDraw();
  }
}

void menuFan()
{
  menuDrawPage(&fanItems);
  showFanSpeed();
  menuSetFrontCallBack(menuCallBackFan);
}
