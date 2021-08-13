#include "Popup.h"
#include "../TSC_Menu.h"


#define BUTTON_NUM 1

BUTTON bottomSingleBtn = {
  //button location                       color before pressed   color after pressed
  POPUP_RECT_SINGLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK
};

BUTTON bottomDoubleBtn[] = {
  {POPUP_RECT_DOUBLE_CONFIRM, NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
  {POPUP_RECT_DOUBLE_CANCEL,  NULL, 5, 1, GREEN, BLACK, WHITE,   GREEN, WHITE, BLACK},
};

GUI_RECT doubleBtnRect[] = {POPUP_RECT_DOUBLE_CONFIRM, POPUP_RECT_DOUBLE_CANCEL};
GUI_RECT singleBtnRect = POPUP_RECT_SINGLE_CONFIRM;

WINDOW window = {
  POPUP_RECT_WINDOW,       //
  10,                      //
  3,                       //
  0x5D7B,                  //
  {MAGENTA, 0x5D7B, POPUP_TITLE_HEIGHT},   //
  {WHITE, BLACK, POPUP_TEXT_HEIGHT},    //
  {WHITE, GRAY,  POPUP_BOTTOM_HEIGHT},  //
};

static BUTTON *windowButton =  NULL;
static uint16_t buttonNum = 0;

void windowReDrawButton(uint8_t positon, uint8_t pressed)
{
  if(positon >= buttonNum)            return;
  if(pressed >= 2)                    return;
  if(windowButton == NULL)            return;
  if(windowButton->context == NULL)   return;

  GUI_DrawButton(windowButton + positon, pressed);
}


void popupDrawPage(BUTTON *btn, const uint8_t *title, const uint8_t *context, const uint8_t *yes, const uint8_t *no)
{
  buttonNum = 0;
  windowButton = btn;
  if(yes)
  {
    windowButton[buttonNum++].context = yes;
  }
  if(no)
  {
    windowButton[buttonNum++].context = no;
  }
  
  TSC_ReDrawIcon = windowReDrawButton;
  GUI_DrawWindow(&window, title, context);
  
  for(uint8_t i = 0; i < buttonNum; i++)
    GUI_DrawButton(&windowButton[i], 0);    
}

void menuCallBackPopup(void)
{
  uint16_t key_num = KEY_GetValue(BUTTON_NUM, &singleBtnRect);
  switch(key_num)
  {            
    case KEY_POPUP_CONFIRM: 
      infoMenu.cur--; 
      break;
    
    default:
      break;            
  }
}

void menuPopup(void)
{
  menuSetFrontCallBack(menuCallBackPopup);
}

void popupReminder(uint8_t* info, uint8_t* context)
{
  popupDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);    
  if(infoMenu.menu[infoMenu.cur] != menuPopup)
  {
    infoMenu.menu[++infoMenu.cur] = menuPopup;
  }
}
