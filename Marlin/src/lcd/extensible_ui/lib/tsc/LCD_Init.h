#pragma once

#include "./Hal/stm32f1_fsmc.h"

#define WHITE                 0xFFFF
#define BLACK                 0x0000
#define BLUE                  0x001F
#define GBLUE                 0X07FF
#define RED                   0xF800
#define MAGENTA               0xF81F
#define GREEN                 0x07E0
#define CYAN                  0x7FFF
#define YELLOW                0xFFE0
#define BROWN                 0XBC40
#define BRRED                 0XFC07
#define GRAY                  0X8430
#define ORANGE                0xFB23
#define PURPLE                0x7817
#define LIME                  0xBFE0
#define DARKBLUE              0X0030
#define DARKGREEN             0x0340
#define DARKGRAY              0x2124

#define LCD_WIDTH_PIXEL 480
#define LCD_HEIGHT_PIXEL 320
#define BYTE_HEIGHT 16
#define BYTE_WIDTH  (BYTE_HEIGHT/2)
#define ICON_WIDTH  95
#define ICON_HEIGHT 95
#define TITLE_END_Y 50
  
#define SPACE_X ((LCD_WIDTH_PIXEL - ICON_WIDTH*4)/4)
#define START_X (SPACE_X/2)
#define SPACE_X_PER_ICON (LCD_WIDTH_PIXEL/4)
#define SPACE_Y ((LCD_HEIGHT_PIXEL - TITLE_END_Y - ICON_HEIGHT*2)/2)


#define WORD_UNICODE            0x280000 // unicode (+0x480000 4.5M)
#define BYTE_ASCII_ADDR         0x700000 // ascii (+0x1000 4K)

#define LCD_WR_REG(reg) LCD_WriteReg(reg)
#define LCD_WR_DATA(data) LCD_WriteData(data)
#define LCD_WR_16BITS_DATA(data) LCD_WriteData(data)

void LCD_RefreshDirection(void);
void LCD_Init(void);          //
void LCD_init_Reg(void);
