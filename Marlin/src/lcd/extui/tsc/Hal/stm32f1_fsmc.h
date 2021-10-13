#pragma once

#include <stdint.h>

typedef struct {
  volatile uint16_t REG;
  volatile uint16_t RAM;
} TFTLCD_TypeDef;

extern TFTLCD_TypeDef *TFTLCD;  // LCD FSMC Control Address

void LCD_FSMCInit(uint8_t cs, uint8_t rs);
void LCD_WriteData(uint16_t RegValue);
void LCD_WriteReg(uint16_t Reg);
uint32_t LCD_ReadData(uint16_t RegValue, uint8_t ReadSize);

void LCD_WriteMultiple(uint16_t data, uint32_t count);
void LCD_WriteSequence(uint16_t *data, uint16_t length);
void LCD_WriteSequence_Async(uint16_t *data, uint16_t length);
void LCD_WaitSequence_Async();

