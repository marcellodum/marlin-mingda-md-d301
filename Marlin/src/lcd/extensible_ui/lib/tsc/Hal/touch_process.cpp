#include "touch_process.h"
#include "xpt2046.h"
#include "../LCD_Init.h"
#include "../Language/Language.h"
#include "../../../ui_api.h"
#include "../Menu/menu.h"

#define XL1 LCD_X[0]
#define XL2 LCD_X[1]
#define XL3 LCD_X[2]

#define YL1 LCD_Y[0]
#define YL2 LCD_Y[1]
#define YL3 LCD_Y[2]

#define X1  TP_X[0]
#define X2  TP_X[1]
#define X3  TP_X[2]

#define Y1  TP_Y[0]
#define Y2  TP_Y[1]
#define Y3  TP_Y[2]

#define T_A TSC_Para[0]
#define T_B TSC_Para[1]
#define T_C TSC_Para[2]
#define T_D TSC_Para[3]
#define T_E TSC_Para[4]
#define T_F TSC_Para[5]
#define T_K TSC_Para[6]


uint32_t TSC_Para[7] = {5200, 513600, 4247661336, 4294598656, 2160, 687023640, 1972341};

void TS_Get_Coordinates(uint16_t *x, uint16_t *y)
{
  uint16_t tp_x = XPT2046_Repeated_Compare_AD(CMD_RDX);
  uint16_t tp_y = XPT2046_Repeated_Compare_AD(CMD_RDY);

  *x = (T_A*tp_x+T_B*tp_y+T_C)/T_K;
  *y = (T_D*tp_x+T_E*tp_y+T_F)/T_K;
}

#define TS_ERR_RANGE 10
uint8_t calibrationEnsure(uint16_t x,uint16_t y)
{
  uint32_t i;
  uint16_t tp_x,tp_y,x_offset;
  int lcd_x,lcd_y;
  GUI_SetColor(BLACK);
  GUI_FillCircle(x,y,5);
  for(i=0;i<10;i++)
  {
    GUI_DrawPoint(x+i,y);
    GUI_DrawPoint(x-i,y);
    GUI_DrawPoint(x,y+i);
    GUI_DrawPoint(x,y-i);
  }
  while(!isPress());
  tp_x = XPT2046_Repeated_Compare_AD(CMD_RDX);
  tp_y = XPT2046_Repeated_Compare_AD(CMD_RDY);

  //	
  lcd_x = (T_A*tp_x+T_B*tp_y+T_C)/T_K;
  lcd_y = (T_D*tp_x+T_E*tp_y+T_F)/T_K;


  if(lcd_x < x+TS_ERR_RANGE && lcd_x>x-TS_ERR_RANGE  && lcd_y > y-TS_ERR_RANGE && lcd_y<y+TS_ERR_RANGE)
  {		
    x_offset=(LCD_WIDTH_PIXEL - GUI_StrPixelWidth(textSelect(LABEL_ADJUST_OK))) >> 1;
    GUI_DispString(x_offset, LCD_HEIGHT_PIXEL-40, textSelect(LABEL_ADJUST_OK));
    /*GUI_DispDec(0, 0, T_A, 10, RIGHT);
    GUI_DispDec(0, 30, T_B, 10, RIGHT);
    GUI_DispDec(0, 60, T_C, 10, RIGHT);
    GUI_DispDec(0, 90, T_D, 10, RIGHT);
    GUI_DispDec(0, 120, T_E, 10, RIGHT);
    GUI_DispDec(0, 150, T_F, 10, RIGHT);
    GUI_DispDec(0, 180, T_K, 10, RIGHT);
    while(1);*/
    ExtUI::delay_ms(1000);
  }
  else
  {
    while(isPress());
    GUI_SetColor(RED);
    x_offset=(LCD_WIDTH_PIXEL - GUI_StrPixelWidth(textSelect(LABEL_ADJUST_FAILED))) >> 1;
    GUI_DispString(x_offset, LCD_HEIGHT_PIXEL-40, textSelect(LABEL_ADJUST_FAILED));
    GUI_DispDec(0,0,lcd_x,3,0);
    GUI_DispDec(0,20,lcd_y,3,0);
    ExtUI::delay_ms(1000);
    return 0;
  }
  return 1;
}

void TSC_Calibration(void)
{
  uint16_t x_offset;
  uint32_t LCD_X[3] = {40, LCD_WIDTH_PIXEL-40, LCD_WIDTH_PIXEL-40};
  uint32_t LCD_Y[3] = {40, 40, LCD_HEIGHT_PIXEL-40};
  uint16_t TP_X[3],TP_Y[3];
  uint32_t tp_num = 0;
  int i;

  do
  {
    GUI_Clear(WHITE);
    GUI_SetColor(BLACK);
    GUI_SetBkColor(WHITE);
    x_offset=(LCD_WIDTH_PIXEL - GUI_StrPixelWidth(textSelect(LABEL_ADJUST_TITLE))) >> 1;
    GUI_DispString(x_offset, 5, textSelect(LABEL_ADJUST_TITLE));
    x_offset=(LCD_WIDTH_PIXEL - GUI_StrPixelWidth(textSelect(LABEL_ADJUST_INFO))) >> 1;
    GUI_DispString(x_offset, 25, textSelect(LABEL_ADJUST_INFO));
    GUI_SetColor(RED);
    for(tp_num = 0;tp_num<3;tp_num++)
    {
      GUI_FillCircle(LCD_X[tp_num],LCD_Y[tp_num],3);
      for(i=0;i<10;i++)
      {
        GUI_DrawPoint(LCD_X[tp_num]+i,LCD_Y[tp_num]);
        GUI_DrawPoint(LCD_X[tp_num]-i,LCD_Y[tp_num]);
        GUI_DrawPoint(LCD_X[tp_num],LCD_Y[tp_num]+i);
        GUI_DrawPoint(LCD_X[tp_num],LCD_Y[tp_num]-i);
      }
      while(isPress() == false);
      TP_X[tp_num] = XPT2046_Repeated_Compare_AD(CMD_RDX);
      TP_Y[tp_num] = XPT2046_Repeated_Compare_AD(CMD_RDY);
      while(isPress() != false);
    }
    T_K = (X1 - X3)*(Y2 - Y3) - (X2 - X3)*(Y1 - Y3);
    T_A = ((XL1 - XL3)*(Y2 - Y3) - (XL2 - XL3)*(Y1 - Y3));
    T_B = (( X1 - X3 )*( XL2 - XL3) - (XL1 - XL3)*( X2 - X3));
    T_C = (Y1*( X3*XL2 - X2*XL3) + Y2*(X1*XL3 - X3*XL1) + Y3*(X2*XL1 - X1*XL2));
    T_D = ((YL1 - YL3)*(Y2 - Y3) - (YL2 - YL3)*(Y1 - Y3));
    T_E = ((X1 - X3)*(YL2 - YL3) - (YL1 - YL3)*(X2 - X3));
    T_F = (Y1*(X3*YL2 - X2*YL3) + Y2*(X1*YL3 - X3*YL1) + Y3*(X2*YL1 - X1*YL2));
  }while(calibrationEnsure(LCD_WIDTH_PIXEL/2, LCD_HEIGHT_PIXEL/2)==0);

  GUI_RestoreColorDefault();
}

#include "../../../src/libs/buzzer.h"
#include "../Menu/Settings.h"
uint16_t Key_value(uint8_t total_rect,const GUI_RECT* menuRect)
{
  uint8_t i = 0;
  uint16_t x, y;

  TS_Get_Coordinates(&x,&y);
  for(i=0;i<total_rect;i++)
  {
    if((x>menuRect[i].x0)&&(x<menuRect[i].x1)&&(y>menuRect[i].y0)&&(y<menuRect[i].y1))
    {
      if (!infoSettings.silent)
        BUZZ(40, 1500);
      return i;
    }
  }
  return IDLE_TOUCH;
}

uint8_t isPress(void)
{
  static bool pressed = false;
  static uint32_t nextTime = 0;

  bool nowPressed = !XPT2046_Read_Pen();

  if (nowPressed) {
    if (nextTime <= millis()) {
      pressed = true;
    }
  } else {
    pressed = false;
    nextTime = millis() + 10;
  }

  return pressed;
}


void (*TSC_ReDrawIcon)(uint8_t positon, uint8_t is_press) = NULL;

uint16_t KEY_GetValue(uint8_t total_rect,const GUI_RECT* menuRect)
{
  static uint16_t key_num = IDLE_TOUCH;
  static bool firstPress = true;

  uint16_t key_return=IDLE_TOUCH;

  if (isPress())        
  {
    if(firstPress)
    {
      key_num = Key_value(total_rect, menuRect);
      firstPress = false;
      if(TSC_ReDrawIcon)
        TSC_ReDrawIcon(key_num, 1);
    }
  }
  else
  {
    if (firstPress == false )
    {
      if(TSC_ReDrawIcon)
        TSC_ReDrawIcon(key_num, 0);
      key_return = key_num;
      key_num = IDLE_TOUCH;
      firstPress = true;
    }
  }
  return key_return;
}

typedef enum
{
  NO_CLICK,
  FIRST_CLICK,
  FIRST_RELEASE,
  SECOND_CLICK,
  LONG_PRESS,
}KEY_STATUS;

#define KEY_DOUOBLE_SPACE        15     //
#define KEY_LONG_PRESS_START     200     //

#define KEY_LONG_PRESS_SPACE_MAX 10     //
#define KEY_LONG_PRESS_SPACE_MIN 2      //

//u16 KEY_GetValue(u8 total_rect,const GUI_RECT* menuRect)
//{
//  u16 key_return=NO_TOUCH;

//  static u16  first_key  = NO_TOUCH;
//  static u32  first_time = 0;
//  static u8   long_press_space = KEY_LONG_PRESS_SPACE_MAX;

//  static KEY_STATUS nowStatus = NO_CLICK;    //������ǰ��״̬

//  if(touchScreenIsPress)        
//  {
//    switch(nowStatus)
//    {
//      case NO_CLICK: 
//        nowStatus=FIRST_CLICK;
//        first_key=Key_value(total_rect,menuRect);
//        first_time=OS_GetTime();
//        break;

//      case FIRST_CLICK:
//        if(OS_GetTime()-first_time>KEY_LONG_PRESS_START)
//        {
//          nowStatus  = LONG_PRESS;
//          first_key |= KEY_LONG_CLICK;
//        }
//        break;

//      case FIRST_RELEASE:
//        if(first_key == Key_value(total_rect,menuRect))
//        {
//          nowStatus  = SECOND_CLICK;   
//          first_key |= KEY_DOUBLE_CLICK;  
//        }
//        else                  
//        {
//          nowStatus=NO_CLICK;
//        }
//        break;

//      case SECOND_CLICK:
//        if(OS_GetTime()-first_time>KEY_LONG_PRESS_START)
//        {
//          nowStatus  = LONG_PRESS;
//          first_key |= KEY_LONG_CLICK;
//        }
//        break;

//      case LONG_PRESS:
//        if(OS_GetTime()-first_time>long_press_space)
//        {
//          if(long_press_space>KEY_LONG_PRESS_SPACE_MIN)
//            long_press_space--;
//          first_time=OS_GetTime();
//          key_return = first_key;		
//        }
//        break;

//      default:
//        break;
//    }
//  }
//  else
//  {
//    switch(nowStatus)
//    {
//      case FIRST_CLICK: 
//        nowStatus=FIRST_RELEASE;
//        break;

//      case FIRST_RELEASE:
//        if(OS_GetTime()-first_time>KEY_DOUOBLE_SPACE)
//        {
//        nowStatus  = NO_CLICK;
//        key_return = first_key;
//        }
//        break;

//      case SECOND_CLICK:
//        nowStatus  = NO_CLICK;
//        key_return = first_key;
//        break;

//      case LONG_PRESS:
//        nowStatus  = NO_CLICK;
//        key_return = first_key | KEY_LONG_RELEASE;
//        break;

//      default:
//        break;
//    }
//  }
//  return key_return;
//}
