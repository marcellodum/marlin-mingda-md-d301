#include "../TSC_Menu.h"

//1 title, ITEM_PER_PAGE items
MENUITEMS percentageItems = {
// title
LABEL_PERCENTAGE_SPEED,
// icon                       label
 {{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_MOVE,                 LABEL_PERCENTAGE_SPEED},
  {ICON_E_5_MM,               LABEL_5_PERCENT},
  {ICON_NORMAL_SPEED,         LABEL_NORMAL_SPEED},
  {ICON_BACK,                 LABEL_BACK},}
};

#define ITEM_PERCENTAGE_NUM 2
const ITEM itemPercentage[ITEM_PERCENTAGE_NUM] = {
// icon                       label
  {ICON_MOVE,                 LABEL_PERCENTAGE_SPEED},
  {ICON_EXTRUDE,              LABEL_PERCENTAGE_FLOW},
};
static int16_t itemPercentageTitle[ITEM_PERCENTAGE_NUM] = {
  LABEL_PERCENTAGE_SPEED,     LABEL_PERCENTAGE_FLOW
};


#define ITEM_PERCENT_UNIT_NUM 3
const ITEM itemPercentUnit[ITEM_PERCENT_UNIT_NUM] = {
// icon                       label
  {ICON_E_1_MM,               LABEL_1_PERCENT},
  {ICON_E_5_MM,               LABEL_5_PERCENT},
  {ICON_E_10_MM,              LABEL_10_PERCENT},
};
const  uint8_t item_percent_unit[ITEM_PERCENT_UNIT_NUM] = {1, 5, 10};
static uint8_t item_percent_unit_i = 1;

//0:Speed  1:Flow
static uint8_t item_percentage_i = 0;
uint16_t getGercentageValue(uint8_t flowOrSpeed)
{
  if (item_percentage_i == 0) return ExtUI::getFeedrate_percent();
  else return ExtUI::getFlowPercentage(ExtUI::getActiveTool());
}

void setGercentageValue(uint8_t flowOrSpeed, uint16_t value)
{
  if (item_percentage_i == 0) return  ExtUI::setFeedrate_percent(value);
  else return ExtUI::setFlow_percent(value, ExtUI::getActiveTool());
}

void showPercentage(void)
{
  GUI_DispDec(CENTER_X - 3*BYTE_WIDTH/2, CENTER_Y, getGercentageValue(item_percentage_i), 3, LEFT);
  GUI_DispString(CENTER_X - 3*BYTE_WIDTH/2 + 3* BYTE_WIDTH , CENTER_Y, (uint8_t *)"%");
}
void percentageReDraw(void)
{
  GUI_DispDec(CENTER_X - 3*BYTE_WIDTH/2, CENTER_Y, getGercentageValue(item_percentage_i), 3, LEFT);
}

static uint16_t lastPercentage = 0;
void menuCallBackSpeed(void)
{
  KEY_VALUES  key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setGercentageValue(item_percentage_i, getGercentageValue(item_percentage_i) - item_percent_unit[item_percent_unit_i]);
      break;
      
    case KEY_ICON_3:
      setGercentageValue(item_percentage_i, getGercentageValue(item_percentage_i) + item_percent_unit[item_percent_unit_i]);
      break;
      
    case KEY_ICON_4:
      item_percentage_i = (item_percentage_i+1) % ITEM_PERCENTAGE_NUM;
      percentageItems.items[key_num] = itemPercentage[item_percentage_i];
      menuDrawItem(&percentageItems.items[key_num], key_num);      
      percentageItems.title.index = itemPercentageTitle[item_percentage_i];
      menuDrawTitle(textSelect(percentageItems.title.index));
      showPercentage();
      break;
    
    case KEY_ICON_5:
      item_percent_unit_i = (item_percent_unit_i+1) % ITEM_PERCENT_UNIT_NUM;
      percentageItems.items[key_num] = itemPercentUnit[item_percent_unit_i];
      menuDrawItem(&percentageItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      setGercentageValue(item_percentage_i, 100);
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default:break;
  }
  if(lastPercentage != getGercentageValue(item_percentage_i))
  {
    lastPercentage = getGercentageValue(item_percentage_i);
    percentageReDraw();
  }
}

void menuSpeed(void)
{
  menuDrawPage(&percentageItems);
  showPercentage();
  menuSetFrontCallBack(menuCallBackSpeed);
}
