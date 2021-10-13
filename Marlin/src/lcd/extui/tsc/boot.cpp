#include "TSC_Menu.h"
#include "boot.h"
#include "../../../core/macros.h"

const GUI_RECT iconUpdateRect = {(LCD_WIDTH_PIXEL - ICON_WIDTH)/2,              (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2, 
                                 (LCD_WIDTH_PIXEL - ICON_WIDTH)/2 + ICON_WIDTH, (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT};
const GUI_RECT labelUpdateRect = {0,              (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT, 
                                 LCD_WIDTH_PIXEL, (LCD_HEIGHT_PIXEL - ICON_HEIGHT)/2 + ICON_HEIGHT + BYTE_HEIGHT};

const char iconBmpName[][32]={
"Heat", "Move", "Home", "Print", "Extrude", "Fan", "Settings", "Leveling", "Inc", "Dec",
"Nozzle", "bed", "1_deg", "5_deg", "10_deg", "Stop", "Back", "x_inc", "y_inc", "z_inc",
"01_mm", "1_mm", "10_mm", "100_mm", "x_dec", "y_dec","z_dec", "x_home", "y_home", "z_home", "Folder",
"file", "Page_up", "Page_down", "Pause", "Resume", "Load", "Unload", "slow_s", "normal_s", "fast_s",  
"e_1_mm", "e_5_mm", "e_10_mm", "e_100_mm", "e_200_mm", "e_1_per", "e_5_per", "e_10_per", "fan_fs", "fan_hs", "rotateui", "Language", "ts_adj", "More", "scr_info",
"BackGroundColor", "FontColor", "Disconn", "baudrate", "Percent", "BabyStep", "001_mm", "onb_sd", "ontft_sd", "U_Disk",
"Runout", "Point_1", "Point_2", "Point_3", "Point_4", "Point_5", "marlin", "BTT", "gcode", "BLTouch", "BLT_dep",
"BLT_stw", "BLT_test", "BLT_rpt", "BLT_rst", "scr_set", "mac_set", "feat_set", "probeofs", "eepromsv", "eepromrs", "eepromrt", "SilentOn", "shutdown",
"RGBSet", "RGBRed", "RGBGreen", "RGBBlue", "RGBWhite", "RGBOff", "PH_Both", "Preheat_PLA", "Preheat_PETG", "Preheat_ABS",
"PowerSup", "Custom", "Custom0", "Custom1", "Custom2", "Custom3", "Custom4", "Custom5", "Custom6", "HomeMove", "Heat_Fan",
"manual_lev", "CoolDown", "Silent~1","STNozzle","ST_Bed","ST_Fan","MainMenu","st_speed","st_flow", "InfoBox_part1", "InfoBox_part2", "InfoBox", "Stop",
"apply", "brightn", "c_light", "chamber", "conn_set", "dis_step", "g_bed", "g_chamb", "g_nozzle",
"lev_corn", "levedged", "level_off", "level_on", "marlin_m", "meshedit", "Mesh_OK", "nozzle_d", "nozzle_u",
"param", "pid", "preheat", "p_bed", "p_fan", "p_flow", "p_nozzle", "p_speed", "p_timer", "p_zlayer",
"reset_v", "sound", "tune_ext", "tuning", "z_align", "z_fade"
};

#define LCD_DMA_MAX_TRANS	65535		// DMA 65535 bytes one frame
void lcd_frame_segment_display(uint16_t size, uint32_t addr)
{
  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_FAST_READ_DATA);
  W25Qxx_SPI_Read_Write_Byte((uint8_t)((addr)>>16));
  W25Qxx_SPI_Read_Write_Byte((uint8_t)((addr)>>8));
  W25Qxx_SPI_Read_Write_Byte((uint8_t)addr);
  W25Qxx_SPI_Read_Write_Byte(0XFF);  //8 dummy clock
  W25qxxSPI.setDataSize(DATA_SIZE_16BIT);
  W25qxxSPI.dmaTransfer(0, (void *)&TFTLCD->RAM, size);
  W25qxxSPI.setDataSize(DATA_SIZE_8BIT);
  W25qxxSPI.begin();
  W25Qxx_SPI_CS_Set(1);
}

void lcd_frame_display(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint32_t addr)
{
  uint32_t cur=0;
  uint32_t segmentSize;
  uint32_t totalSize = w*h;

  LCD_SetWindow(sx,sy,sx+w-1,sy+h-1);

  for(cur = 0; cur < totalSize; cur += LCD_DMA_MAX_TRANS)
  {
    segmentSize = (cur+LCD_DMA_MAX_TRANS)<=totalSize ? LCD_DMA_MAX_TRANS : totalSize-cur;
    lcd_frame_segment_display(segmentSize, addr+cur*(2));
  }
}

void LOGO_ReadDisplay(void)
{
  lcd_frame_display(0, 0, LCD_WIDTH_PIXEL, LCD_HEIGHT_PIXEL, LOGO_ADDR);
}

void ICON_ReadDisplay(uint16_t sx, uint16_t sy, uint8_t icon)
{
  lcd_frame_display(sx, sy, ICON_WIDTH, ICON_HEIGHT, ICON_ADDR(icon));
}

void ICON_PressedDisplay(uint16_t sx, uint16_t sy, uint8_t icon)
{
  uint16_t mode = 0x0FF0;
  uint16_t x, y;
  uint16_t color = 0;
  uint32_t address = ICON_ADDR(icon);

  LCD_SetWindow(sx, sy, sx+ICON_WIDTH-1, sy+ICON_HEIGHT-1);

  W25Qxx_SPI_CS_Set(0);
  W25Qxx_SPI_Read_Write_Byte(CMD_READ_DATA);
  W25Qxx_SPI_Read_Write_Byte((address & 0xFF0000) >> 16);
  W25Qxx_SPI_Read_Write_Byte((address& 0xFF00) >> 8);
  W25Qxx_SPI_Read_Write_Byte(address & 0xFF);

  for(y=sy; y<sy+ICON_WIDTH; y++)
  {	
    for(x=sx; x<sx+ICON_HEIGHT; x++)
    {
      color  = (W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE)<<8);
      color |= W25Qxx_SPI_Read_Write_Byte(W25QXX_DUMMY_BYTE);
      LCD_WR_16BITS_DATA(color & mode);
    }
  }		
  W25Qxx_SPI_CS_Set(1);		
}


#include "../../sd/cardreader.h"
#if ENABLED(SDSUPPORT)
  extern CardReader card;
#endif

uint8_t TSCBoot::  scanUpdateFile(void) {
  uint8_t rst = 0;
  
  if (!IS_SD_INSERTED()) return rst;
  if (!card.isMounted()) card.mount();
  if (card.isMounted()) {
    if (card.getroot().exists(BMP_ROOT_DIR)) rst |= BMP;
    if (card.getroot().exists(FONT_ROOT_DIR)) rst |= FONT;
  }
  return rst;
}

bool TSCBoot::bmpDecode(char *bmp, uint32_t addr) {
  SdFile file;
  char  magic[2];  
  int   w, h, bytePerLine;  
  short bitDepth; 
  int   offset;

  if (!file.open(card.getrootp(), bmp, O_READ)) {return false;}
  if (!file.read(magic, 2) || memcmp(magic, "BM", 2)) return false;

  file.seekSet(10);
  file.read(&offset, sizeof(int));
  file.seekSet(18);
  file.read(&w, sizeof(int));
  file.read(&h, sizeof(int));
  file.seekSet(28);
  file.read(&bitDepth, sizeof(short));

  if (bitDepth < 24) return false;
  bytePerLine = w * bitDepth / 8;     
  if (bytePerLine % 4 !=0) bytePerLine = (bytePerLine / 4 + 1) * 4;  // BMP format 4 bytes alignment
  
  // Erase w25qxx sector before write
  for (int sector=0; sector < (w * h * 2 + W25QXX_SECTOR_SIZE - 1) / W25QXX_SECTOR_SIZE; sector++) {
    W25Qxx_EraseSector(addr + sector * W25QXX_SECTOR_SIZE);
  }

  uint16_t pointIndex=0;
  uint8_t  lcdcolor[4];
  uint8_t  buf[256];
  GUI_COLOR point;
  for(int y = 0; y < h; y++) {
    file.seekSet(offset + (h - y - 1) * bytePerLine);
    for(int x = 0; x < w; x++) {
      file.read((char *)&lcdcolor, bitDepth / 8);

      point.RGB.r=lcdcolor[2]>>3; // 24bits or 32bits convert to 16bits "565" format
      point.RGB.g=lcdcolor[1]>>2;        
      point.RGB.b=lcdcolor[0]>>3;

      //GUI_DrawPixel(x, y, point.color);
      
      buf[pointIndex++]=(uint8_t)(point.color>>8);
      buf[pointIndex++]=(uint8_t)(point.color&0xFF);
      
      if(pointIndex == W25QXX_PAGE_SIZE) {
        W25Qxx_WritePage(buf, addr, W25QXX_PAGE_SIZE);
        addr += W25QXX_PAGE_SIZE;
        pointIndex = 0;
      }
    }
  }    

  W25Qxx_WritePage(buf, addr, pointIndex);
  addr += pointIndex;
  file.close();

  return true;  
}  

void TSCBoot::updateIcon(void) {
  char nowBmp[64];  
  GUI_Clear(BK_COLOR);
  GUI_DispString(100, 5, (uint8_t*)"Icon Updating...!");

  if(bmpDecode((char *)BMP_ROOT_DIR "/Logo.bmp", LOGO_ADDR))
  {
    LOGO_ReadDisplay();
  }
  GUI_Clear(BK_COLOR);
  for(uint32_t i=0; i < COUNT(iconBmpName); i++)
  {
    sprintf(nowBmp, (char *)BMP_ROOT_DIR "/%s.bmp", iconBmpName[i]);
    if(bmpDecode(nowBmp, ICON_ADDR(i)))
    {
      GUI_ClearRect(labelUpdateRect.x0, labelUpdateRect.y0, labelUpdateRect.x1, labelUpdateRect.y1);
      GUI_DispStringInPrect(&labelUpdateRect, (uint8_t *)nowBmp);
      ICON_ReadDisplay(iconUpdateRect.x0, iconUpdateRect.y0, i);
    }
  }
}

void TSCBoot::updateFont(char *font, uint32_t addr) {
  SdFile file;
  char buffer[128];
  uint32_t  progress = 0;
  uint32_t  offset = 0;
  uint8_t*  tempbuf = NULL;
  
  if (!file.open(card.getrootp(), font, O_READ)) return;

  tempbuf = (uint8_t*)malloc(W25QXX_SECTOR_SIZE);
  if (tempbuf == NULL)  return;
  GUI_Clear(BK_COLOR);
  sprintf(buffer, "%s Size: %luKB", font, file.fileSize()>>10);
  GUI_DispString(0, 100, (uint8_t*)buffer);
  GUI_DispString(0, 140, (uint8_t*)"Updating:   %");
  
  while (file.curPosition() < file.fileSize()) {
    uint16_t readBytes = file.read(tempbuf, W25QXX_SECTOR_SIZE);
    if (!readBytes) break;
    
    W25Qxx_EraseSector(addr + offset);
    W25Qxx_WriteBuffer(tempbuf, addr + offset, W25QXX_SECTOR_SIZE);
    offset += readBytes;
    if (progress != offset * 100 / file.fileSize()) {
      progress =  offset * 100 / file.fileSize();
      GUI_DispDec(0 + BYTE_WIDTH*9, 140, progress, 3, RIGHT);
    }
    if(readBytes !=W25QXX_SECTOR_SIZE) break;
  }
  
  file.close();
  free(tempbuf);
}

/*
void scanResetDir(void)
{
  FIL resetfile;
  if (f_open(&resetfile, TFT_RESET_FILE, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    f_close(&resetfile);
    f_rename(TFT_RESET_FILE, TFT_RESET_FILE ".DONE");
    //infoSettingsReset();
    //TSC_Calibration();
  }
}*/

void TSCBoot::scanUpdates(void) {
  volatile uint8_t result = 0;   //must volatile

  result = scanUpdateFile();

  if (result & FONT) {
    updateFont((char *)FONT_ROOT_DIR "/byte_a~1.fon", BYTE_ASCII_ADDR);
    updateFont((char *)FONT_ROOT_DIR "/word_u~1.fon", WORD_UNICODE);
  }
  if (result & BMP) {
    updateIcon();
  }
 // if (result) f_rename(ROOT_DIR, ROOT_DIR".CUR");
 // scanResetDir();
}
