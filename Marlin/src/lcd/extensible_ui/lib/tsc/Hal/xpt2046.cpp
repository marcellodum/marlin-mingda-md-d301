#include "../../../../../inc/MarlinConfig.h"
#include "xpt2046.h"

#define TOUCH_CS_PIN     PB6
#define TOUCH_SCK_PIN    PB3
#define TOUCH_MOSI_PIN   PB5
#define TOUCH_MISO_PIN   PB4 
#define TOUCH_INT_PIN    PB7 // PenIRQ coming from XPT2046

/***************************************** XPT2046 SPI ģʽ�ײ���ֲ�Ľӿ�********************************************/
//XPT2046 SPI��� - ʹ��ģ��SPI
_SW_SPI xpt2046;

//Ƭѡ
void XPT2046_CS_Set(uint8_t level)
{
  SW_SPI_CS_Set(&xpt2046, level);
}

//��д����
uint8_t XPT2046_ReadWriteByte(uint8_t TxData)
{		
  return SW_SPI_Read_Write(&xpt2046, TxData);			    
}

//XPT2046 SPI�ͱ��жϳ�ʼ��
void XPT2046_Init(void)
{
  //PA15-TPEN
  SET_INPUT(TOUCH_INT_PIN);

  SW_SPI_Config(&xpt2046, _SPI_MODE3, 8, // 8bit
  TOUCH_CS_PIN,     //CS
  TOUCH_SCK_PIN,    //SCK
  TOUCH_MISO_PIN,   //MISO
  TOUCH_MOSI_PIN    //MOSI
  );
  XPT2046_CS_Set(1);
}


uint8_t XPT2046_Read_Pen(void)
{
  return READ(TOUCH_INT_PIN);
}

uint16_t XPT2046_Read_AD(uint8_t CMD)
{
  uint16_t ADNum;
  XPT2046_CS_Set(0);

  XPT2046_ReadWriteByte(CMD);
  ADNum=XPT2046_ReadWriteByte(0xff);
  ADNum= ((ADNum)<<8) | XPT2046_ReadWriteByte(0xff);
  ADNum >>= 4;

  XPT2046_CS_Set(1);
  return ADNum;
}

#define READ_TIMES 5
#define LOST_VAL 1
uint16_t XPT2046_Average_AD(uint8_t CMD)
{
  uint16_t i, j;
  uint16_t buf[READ_TIMES];
  uint16_t sum=0;
  uint16_t temp;
  for(i=0; i<READ_TIMES; i++) buf[i] = XPT2046_Read_AD(CMD);		 		    
  for(i=0; i<READ_TIMES-1; i++)
  {
    for(j=i+1; j<READ_TIMES; j++)
    {
      if(buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  for(i=LOST_VAL; i < READ_TIMES-LOST_VAL; i++) sum += buf[i];
  temp = sum/(READ_TIMES-2*LOST_VAL);
  return temp;   
} 


#define ERR_RANGE 50
uint16_t XPT2046_Repeated_Compare_AD(uint8_t CMD) 
{
  uint16_t ad1, ad2;
  ad1 = XPT2046_Average_AD(CMD);
  ad2 = XPT2046_Average_AD(CMD);

  if((ad2 <= ad1 && ad1 < ad2 + ERR_RANGE) 
  || (ad1 <= ad2 && ad2 < ad1 + ERR_RANGE))
  {
    return (ad1+ad2)/2;
  }
  else return 0;	  
} 

