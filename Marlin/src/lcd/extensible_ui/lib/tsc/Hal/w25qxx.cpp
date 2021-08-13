#include "../../../../../inc/MarlinConfig.h"
#include "w25qxx.h"

/*************************** W25Qxx SPI ģʽ�ײ���ֲ�Ľӿ� ***************************/
//#define W25Qxx_SPI     _SPI3
//#define W25Qxx_SPEED   0
#define W25Qxx_CS_PIN PB12
SPIClass W25qxxSPI(2);


void W25Qxx_SPI_CS_Set(uint8_t level)
{
  WRITE(W25Qxx_CS_PIN, level);
}

uint8_t W25Qxx_SPI_Read_Write_Byte(uint8_t data)
{
  return W25qxxSPI.transfer(data);
}

void W25Qxx_SPI_Read_Buf(uint8_t* buf, uint16_t nbyte) 
{
  W25qxxSPI.dmaTransfer(0, const_cast<uint8_t*>(buf), nbyte);
}

void W25Qxx_SPI_Write_Buf(uint8_t* pBuffer, uint32_t len)
{
  W25qxxSPI.send(pBuffer, len);
}

void W25Qxx_Init(void)
{
  W25qxxSPI.setClockDivider(SPI_BAUD_PCLK_DIV_2);
  W25qxxSPI.begin();
  SET_OUTPUT(W25Qxx_CS_PIN);
  W25Qxx_SPI_CS_Set(1);
}
/*************************************************************************************/


// дʹ��
void W25Qxx_WriteEnable(void)
{
  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_WRITE_ENABLE);
  W25Qxx_SPI_CS_Set(1);
}
//�ȴ�W25Qxx����
void W25Qxx_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_READ_REGISTER1);
  do
  {
    flashstatus = W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  }
  while (flashstatus & 0x01); 
  W25Qxx_SPI_CS_Set(1);
}

//��ҳд
void W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  W25Qxx_WriteEnable();
  W25Qxx_SPI_CS_Set(0);
	
  W25Qxx_SPI_Read_Write_Byte(CMD_PAGE_PROGRAM);
  W25Qxx_SPI_Read_Write_Byte((WriteAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((WriteAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(WriteAddr & 0xFF);
	
  W25Qxx_SPI_Write_Buf(pBuffer, NumByteToWrite);
  
  W25Qxx_SPI_CS_Set(1);
  W25Qxx_WaitForWriteEnd();
}

//��FLASH�з���buffer������
void W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % W25QXX_SPI_PAGESIZE;//������һҳ���ĸ���ַ��ʼд����
  count = W25QXX_SPI_PAGESIZE - Addr;//������ҳ������д��������
  NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;//����Ҫ��������ݳ��ȿ���д����ҳ
  NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;//����������ҳ�����⣬������д�����ݳ���

  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      while(NumOfPage--)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }
      W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        W25Qxx_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25Qxx_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

      W25Qxx_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/*��FLASH�ж�����*/
void W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  W25Qxx_SPI_CS_Set(0);

  W25Qxx_SPI_Read_Write_Byte(CMD_READ_DATA);

  W25Qxx_SPI_Read_Write_Byte((ReadAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((ReadAddr& 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(ReadAddr & 0xFF);

  W25Qxx_SPI_Read_Buf(pBuffer, NumByteToRead);
	
  W25Qxx_SPI_CS_Set(1);
}

//��ID
uint32_t W25Qxx_ReadID(void)
{
  uint32_t Temp = 0;

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_READ_ID);

  Temp = W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  Temp = (Temp<<8) | W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
  Temp = (Temp<<8) | W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);

  W25Qxx_SPI_CS_Set(1);

  return Temp;
}

//��������
void W25Qxx_EraseSector(uint32_t SectorAddr)
{
  W25Qxx_WriteEnable();

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_SECTOR_ERASE);
  W25Qxx_SPI_Read_Write_Byte((SectorAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((SectorAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(SectorAddr & 0xFF);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}

//�����
void W25Qxx_EraseBlock(uint32_t BlockAddr)
{
  W25Qxx_WriteEnable();
	
  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_BLOCK_ERASE);
  W25Qxx_SPI_Read_Write_Byte((BlockAddr & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((BlockAddr & 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(BlockAddr & 0xFF);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}

//ȫƬ����
void W25Qxx_EraseBulk(void)
{
  W25Qxx_WriteEnable();

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_FLASH__BE);
  W25Qxx_SPI_CS_Set(1);

  W25Qxx_WaitForWriteEnd();
}


