#ifndef _STM32_FLASH_H_
#define _STM32_FLASH_H_

#include <stdint.h>

void STM32_FlashRead(uint8_t *data, uint32_t len);
void STM32_FlashWrite(uint8_t *data, uint32_t len);
  
#endif
