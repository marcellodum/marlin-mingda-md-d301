#ifndef _STATUSSCREEN_H_
#define _STATUSSCREEN_H_

#define TFT35_V2_0

//Colors for drawing the icons
#define GANTRYLBL_BKCOLOR   0x2187
#define GANTRYLBL_COLOR     WHITE
#define HEADING_COLOR       WHITE
#define VAL_COLOR           BLACK
#define INFOBOX_BKCOLOR     0x4b0d
#define INFOBOX_BORDER      0x4b0d
#define INFOBOX_ICON_COLOR  0x03BF
#define INFOMSG_BKCOLOR     WHITE
#define INFOMSG_COLOR       BLACK

typedef struct {
  float x = 0, y = 0, z = 0;
  float actHotend = 0, tagHotend = 0, actBed = 0, tagBed = 0;
  float fanPercent = 0;
  float feedRatePercent = 0;
  ExtUI::extruder_t curTool = ExtUI::E0;
}STATUS_MSG;

extern STATUS_MSG statusMsg;

void statusMsg_GetCurMsg(STATUS_MSG *msg);

void menuStatus(void);
//void set_status_icon(void);
void drawTemperature(void);
void statusScreen_setMsg(const uint8_t *title, const uint8_t *msg);
void drawStatusScreenMsg(void);






//LCD resolution, font and icon size
#if defined(TFT35_V1_0) || defined(TFT35_V1_1) || defined(TFT35_V1_2) || defined(TFT35_V2_0) || defined(TFT35_V3_0)

  #define SSICON_VAL_X0           5
  #define SSICON_VAL_Y0           67
  //#define  statusicon_val_charcount  7

  #define SSICON_NAME_X0          40
  #define SSICON_NAME_Y0          7
  //#define  statusicon_name_charcount  3

  #define STATUS_MSG_ICON_XOFFSET     5
  #define STATUS_MSG_ICON_YOFFSET     5
  #define STATUS_MSG_TITLE_XOFFSET    5
  #define STATUS_MSG_BODY_XOFFSET     0
  #define STATUS_MSG_BODY_YOFFSET     60
  #define STATUS_MSG_BODY_BOTTOM      6
  #define STATUS_TITLE_SHORT          20
  #define STATUS_GANTRY_YOFFSET       8
  #define INFOBOX_P1_WIDTH            108
  #define INFOBOX_P2_WIDTH            107


#elif defined(TFT28_V1_0) || defined(TFT24_V1_1)

  #define SSICON_VAL_X0           2
  #define SSICON_VAL_Y0           47
  //#define  statusicon_val_charcount  7

  #define SSICON_NAME_X0          44
  #define SSICON_NAME_Y0          5
  //#define  statusicon_name_charcount  3

  #define STATUS_MSG_ICON_XOFFSET     3
  #define STATUS_MSG_ICON_YOFFSET     3
  #define STATUS_MSG_TITLE_XOFFSET    3
  #define STATUS_MSG_BODY_XOFFSET     0
  #define STATUS_MSG_BODY_YOFFSET     45
  #define STATUS_MSG_BODY_BOTTOM      4
  #define STATUS_TITLE_SHORT          10
  #define SS_TITLE_END_Y              30
  #define STATUS_GANTRY_YOFFSET       6
  #define INFOBOX_P1_WIDTH            75
  #define INFOBOX_P2_WIDTH            75
#endif

#endif
