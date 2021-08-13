#include "../TSC_Menu.h"

void menuCallBackIsPause(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      if(setPrintPause(true))
        infoMenu.menu[infoMenu.cur] = menuExtrude;
      break;

    case KEY_POPUP_CANCEL:	
      infoMenu.cur--;
      break;		
  }
}

void menuIsPause()
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_IS_PAUSE), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackIsPause);
}


MENUITEMS moreItems = {
//  title
LABEL_MORE,
// icon                       label
 {{ICON_FAN,                  LABEL_FAN},
  {ICON_EXTRUDE,              LABEL_EXTRUDE},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};
 // {ICON_RGB_SETTINGS,         LABEL_RGB_SETTINGS},
 // {ICON_FEATURE_SETTINGS,     LABEL_FEATURE_SETTINGS},
void menuCallBackMore(void)
{
  KEY_VALUES  key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      infoMenu.menu[++infoMenu.cur] = menuFan;
      break;
    
    // case KEY_ICON_1:
    //   infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
    //   break;
    
    case KEY_ICON_1:
      if (!isPaused()) // need paused before extrude
        infoMenu.menu[++infoMenu.cur] = menuIsPause;
      else
        infoMenu.menu[++infoMenu.cur] = menuExtrude; 
      break;
    
    // case KEY_ICON_3:
    //   infoMenu.menu[++infoMenu.cur] = menuFeatureSettings;
    //   break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;            
  }
}


void menuMore()
{
  menuDrawPage(&moreItems);
  menuSetFrontCallBack(menuCallBackMore);
}
