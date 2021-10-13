#include "../TSC_Menu.h"

MENUITEMS moveItems = {
//title
LABEL_MOVE,
//icon
{
  {ICON_X_INC, LABEL_X_INC},
  {ICON_Y_INC, LABEL_Y_INC},
  {ICON_Z_INC, LABEL_Z_INC},
  {ICON_1_MM,  LABEL_1_MM},
  {ICON_X_DEC, LABEL_X_DEC},
  {ICON_Y_DEC, LABEL_Y_DEC},
  {ICON_Z_DEC, LABEL_Z_DEC},
  {ICON_BACK,  LABEL_BACK},
}
};

#define ITEM_MOVE_LEN_NUM 3
const ITEM itemMoveLen[ITEM_MOVE_LEN_NUM] = {
//icon          label
  {ICON_01_MM,  LABEL_01_MM},
  {ICON_1_MM,   LABEL_1_MM},
  {ICON_10_MM,  LABEL_10_MM},
};
const float item_move_len[ITEM_MOVE_LEN_NUM] = {0.1f, 1, 10};
static uint8_t item_move_len_i = 1;

static void setAxisPosition(float diff_mm, const ExtUI::axis_t axis)
{
  ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(axis) + diff_mm, axis);
}

extern GUI_RECT titleRect;
static void redrawPosition(void)
{
  GUI_SetBkColor(TITLE_COLOR);
  
  GUI_DispString(START_X+1*SPACE_X+1*ICON_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2,(uint8_t *)"X:");
  GUI_DispFloat(START_X+1*SPACE_X+1*ICON_WIDTH+2*BYTE_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2, statusMsg.x, 4, 1, LEFT);

  GUI_DispString(START_X+2*SPACE_X+2*ICON_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2,(uint8_t *)"Y:");
  GUI_DispFloat(START_X+2*SPACE_X+2*ICON_WIDTH+2*BYTE_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2, statusMsg.y, 4, 1, LEFT);

  GUI_DispString(START_X+3*SPACE_X+3*ICON_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2,(uint8_t *)"Z:");
  GUI_DispFloat(START_X+3*SPACE_X+3*ICON_WIDTH+2*BYTE_WIDTH, (TITLE_END_Y-10-BYTE_HEIGHT)/2, statusMsg.z, 4, 1, LEFT);

  GUI_SetBkColor(BK_COLOR);
}

void menuCallBackMove()
{
  STATUS_MSG tempMsg;
  statusMsg_GetCurMsg(&tempMsg);
  // Position refresh per 1 sec
  static uint32_t nowTime_ms = 0;
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    // Refresh position
    if (statusMsg.x != tempMsg.x || statusMsg.y != tempMsg.y || statusMsg.z != tempMsg.z){
      statusMsg.x = tempMsg.x;
      statusMsg.y = tempMsg.y;
      statusMsg.z = tempMsg.z;
      redrawPosition();
    }
  }

  KEY_VALUES key_num = menuKeyGetValue();
  switch (key_num)
  {
  case KEY_ICON_0: setAxisPosition(item_move_len[item_move_len_i], ExtUI::X);    break;
  case KEY_ICON_1: setAxisPosition(item_move_len[item_move_len_i], ExtUI::Y);    break;
  case KEY_ICON_2: setAxisPosition(item_move_len[item_move_len_i], ExtUI::Z);    break;
  case KEY_ICON_3: 
    item_move_len_i = (item_move_len_i+1)%ITEM_MOVE_LEN_NUM;            
    moveItems.items[key_num] = itemMoveLen[item_move_len_i];
    menuDrawItem(&moveItems.items[key_num], key_num);
    break;
  case KEY_ICON_4: setAxisPosition(-item_move_len[item_move_len_i], ExtUI::X);    break;
  case KEY_ICON_5: setAxisPosition(-item_move_len[item_move_len_i], ExtUI::Y);    break;
  case KEY_ICON_6: setAxisPosition(-item_move_len[item_move_len_i], ExtUI::Z);    break;
  case KEY_ICON_7: infoMenu.cur--; break;  
  default:
    break;
  }
}

void menuMove()
{
  statusMsg_GetCurMsg(&statusMsg);
  menuDrawPage(&moveItems);
  redrawPosition();
  menuSetFrontCallBack(menuCallBackMove);
}
