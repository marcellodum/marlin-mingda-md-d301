#include "../TSC_Menu.h"

//1title, ITEM_PER_PAGE item(icon + label) 
MENUITEMS printingItems = {
//  title
LABEL_BACKGROUND,
// icon                       label
 {{ICON_PAUSE,                LABEL_PAUSE},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_STOP,                 LABEL_STOP},
  {ICON_HEAT,                 LABEL_HEAT},
  {ICON_PERCENTAGE,           LABEL_PERCENTAGE},
  {ICON_BABYSTEP,             LABEL_BABYSTEP},
  {ICON_MORE,                 LABEL_MORE},}
};

const ITEM itemIsPause[2] = {
// icon                       label
  {ICON_PAUSE,                LABEL_PAUSE},
  {ICON_RESUME,               LABEL_RESUME},
};

//
bool isPrinting(void)
{
  return ExtUI::isPrintingFromMedia() && !card.flag.abort_sd_printing;
}

//
bool isPaused(void)
{
  return ExtUI::isPrintingFromMediaPaused();
}

uint8_t getPrintProgress(void)
{
  return card.percentDone();
}

uint32_t getPrintTime(void)
{
  return print_job_timer.duration();
}



//only return gcode file name except path
//for example:"SD:/test/123.gcode"
//only return "123.gcode"
uint8_t *getCurGcodeName(char *path)
{
  int i=strlen(path);
  for(; path[i]!='/'&& i>0; i--)
  {}
  return (uint8_t* )(&path[i+1]);
}

void resumeToPause(bool is_pause)
{
  if(infoMenu.menu[infoMenu.cur] != menuPrinting) return;
  printingItems.items[KEY_ICON_0] = itemIsPause[is_pause];
  menuDrawItem(&itemIsPause[is_pause],0);
}

bool setPrintPause(bool is_pause)
{
  static bool pauseLock = false;
  if(pauseLock)                      return false;
  if(!isPrinting())                  return false;
  if(isPaused() == is_pause)         return false;
  pauseLock = true;
  if (is_pause) {
    ExtUI::pausePrint();
  } else {
    ExtUI::setUserConfirmed();
    ExtUI::resumePrint();
  }
  pauseLock = false;
  return true;
}



const GUI_RECT progressRect = {1*SPACE_X_PER_ICON, 0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y + ICON_HEIGHT/4,
                               3*SPACE_X_PER_ICON, 0*ICON_HEIGHT+0*SPACE_Y+TITLE_END_Y + ICON_HEIGHT*3/4};

#define BED_X  (progressRect.x1 - 9 * BYTE_WIDTH)
#define TEMP_Y (progressRect.y1 + 3)
#define TIME_Y (TEMP_Y + 1 * BYTE_HEIGHT + 3)
#define POSITION_XY_Y (progressRect.y0 - 1 * BYTE_HEIGHT - 3)

static void redrawToolAct(void)
{
  GUI_DispDec(progressRect.x0+BYTE_WIDTH*3, TEMP_Y, (int16_t)statusMsg.actHotend, 3, RIGHT);
}
static void redrawToolTag(void)
{
  GUI_DispDec(progressRect.x0+BYTE_WIDTH*7, TEMP_Y, (int16_t)statusMsg.tagHotend,  3, LEFT);
}

static void redrawBedAct(void)
{
  GUI_DispDec(BED_X + 2 * BYTE_WIDTH, TEMP_Y, (int16_t)statusMsg.actBed, 3, RIGHT);
}
static void redrawBedTag(void)
{
  GUI_DispDec(BED_X + 6 * BYTE_WIDTH, TEMP_Y, (int16_t)statusMsg.tagBed,  3, LEFT);
}

static void redrawPositionX(void)
{
  GUI_DispFloat(progressRect.x0 + 2*BYTE_WIDTH, POSITION_XY_Y, statusMsg.x, 4, 2, LEFT);
}
static void redrawPositionY(void)
{
  GUI_DispFloat(BED_X + 2*BYTE_WIDTH, POSITION_XY_Y, statusMsg.y, 4, 2, LEFT);
}
static void redrawPositionZ(void)
{
  GUI_DispFloat(BED_X + 2*BYTE_WIDTH, TIME_Y, statusMsg.z, 4, 2, LEFT);
}

void reDrawTime(void)
{
  uint32_t printedTime = getPrintTime();
  uint8_t  hour = printedTime/3600%60,
           min = printedTime%3600/60,
           sec = printedTime%60;
  GUI_SetNumMode(GUI_NUMMODE_ZERO);
  GUI_DispDec(progressRect.x0 + 2 * BYTE_WIDTH, TIME_Y, hour, 2, RIGHT);
  GUI_DispDec(progressRect.x0 + 5 * BYTE_WIDTH, TIME_Y, min, 2, RIGHT);
  GUI_DispDec(progressRect.x0 + 8 * BYTE_WIDTH, TIME_Y, sec, 2, RIGHT);
  GUI_SetNumMode(GUI_NUMMODE_SPACE);
}

void reDrawProgress(uint8_t progress)
{	  
  char buf[5];
  uint16_t progressX = map(progress, 0, 100, progressRect.x0, progressRect.x1);
  GUI_FillRectColor(progressRect.x0, progressRect.y0, progressX, progressRect.y1,BLUE);
  GUI_FillRectColor(progressX, progressRect.y0, progressRect.x1, progressRect.y1,GRAY);
  sprintf(buf, "%3d%%", progress);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_DispStringInPrect(&progressRect, (uint8_t *)buf);    
  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);                     
}

extern SCROLL   titleScroll;
extern GUI_RECT titleRect;


void printingDrawPage(void)
{
  printingItems.title.address = (uint8_t *)card.longest_filename(); //getCurGcodeName(infoFile.title);
  menuDrawPage(&printingItems);
  //	Scroll_CreatePara(&titleScroll, infoFile.title,&titleRect);  //
  // printed time
  GUI_DispString(progressRect.x0, TIME_Y, (uint8_t* )"T:");
  GUI_DispString(progressRect.x0+BYTE_WIDTH*4, TIME_Y, (uint8_t* )":");
  GUI_DispString(progressRect.x0+BYTE_WIDTH*7, TIME_Y, (uint8_t* )":");
  // nozzle temperature 
  GUI_DispString(progressRect.x0+BYTE_WIDTH*2, TEMP_Y,(uint8_t* )":");
  GUI_DispString(progressRect.x0+BYTE_WIDTH*6, TEMP_Y,(uint8_t* )"/");
  // hotbed temperature
  GUI_DispString(BED_X, TEMP_Y, (uint8_t* )"B:");
  GUI_DispString(BED_X+BYTE_WIDTH*5, TEMP_Y, (uint8_t* )"/");

  GUI_DispString(progressRect.x0, POSITION_XY_Y, (uint8_t* )"X:");
  GUI_DispString(BED_X, POSITION_XY_Y, (uint8_t* )"Y:");
  GUI_DispString(BED_X, TIME_Y, (uint8_t* )"Z:");

  reDrawProgress(getPrintProgress());
  reDrawTime();

  statusMsg_GetCurMsg(&statusMsg);
  GUI_DispString(progressRect.x0, TEMP_Y, (uint8_t* )heatDisplayID[statusMsg.curTool + 1]);
  redrawToolAct();
  redrawToolTag();
  redrawBedAct();
  redrawBedTag();

  redrawPositionX();
  redrawPositionY();
  redrawPositionZ();
}

static uint8_t lastProgress = 0;
static uint8_t printPaused = 0;
static uint32_t printedTime = 0;
void menuCallBackPrinting(void)	
{
  STATUS_MSG tempMsg;
  statusMsg_GetCurMsg(&tempMsg);
  
  KEY_VALUES  key_num = menuKeyGetValue();
//  Scroll_DispString(&titleScroll, LEFT); //Scroll display file name will take too many CPU cycles

  // Actual temperature refresh per 1 sec
  static uint32_t nowTime_ms = 0;
  if (millis() - nowTime_ms > 1000) { // Refresh per 1 sec
    nowTime_ms = millis();
    // Refresh position
    if (statusMsg.x != tempMsg.x){
      statusMsg.x = tempMsg.x;
      redrawPositionX();
    }
    if (statusMsg.y != tempMsg.y){
      statusMsg.y = tempMsg.y;
      redrawPositionY();
    }
    if (statusMsg.z != tempMsg.z){
      statusMsg.z = tempMsg.z;
      redrawPositionZ();
    }
    // Refresh temperature
    //  if (tool != tempTool)
    if (statusMsg.actHotend != tempMsg.actHotend) {
      statusMsg.actHotend = tempMsg.actHotend;
      redrawToolAct();
    }    
    if (statusMsg.actBed != tempMsg.actBed) {
      statusMsg.actBed = tempMsg.actBed;
      redrawBedAct();
    }
  }

  // Target temperature refresh in real time  
  if (statusMsg.tagHotend != tempMsg.tagHotend) {
    statusMsg.tagHotend = tempMsg.tagHotend;
    redrawToolTag();
  }
  if (statusMsg.tagBed != tempMsg.tagBed) {
    statusMsg.tagBed = tempMsg.tagBed;
    redrawBedTag();
  }

  // Printing status
  if (printPaused != isPaused()) {
    printPaused = isPaused();
    resumeToPause(printPaused);
  }
  if (lastProgress != getPrintProgress())
  {
    lastProgress = getPrintProgress();
    reDrawProgress(lastProgress);
  }
  if (printedTime != getPrintTime())
  {
    printedTime = getPrintTime();
    reDrawTime();
  }

  switch(key_num)
  {
    case KEY_ICON_0:
      setPrintPause(!isPaused());
      break;
    
    case KEY_ICON_3:
      if(isPrinting())				
        infoMenu.menu[++infoMenu.cur] = menuStopPrinting;	
      break;
      
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = menuHeat;
      break;
    
    case KEY_ICON_5:
      infoMenu.menu[++infoMenu.cur] = menuSpeed;
      break;
    
    case KEY_ICON_6:
      infoMenu.menu[++infoMenu.cur] = menuBabyStep;
      break;
    
    case KEY_ICON_7:
      infoMenu.menu[++infoMenu.cur] = menuMore;
      break;
    
    default :break;
  }
}

void menuPrinting(void)
{
  printPaused = isPaused();
  printingItems.items[KEY_ICON_0] = itemIsPause[printPaused];
  printingDrawPage();
  menuSetFrontCallBack(menuCallBackPrinting);
}

void menuCallBackStopPrinting(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      ExtUI::stopPrint();
      break;

    case KEY_POPUP_CANCEL:
      infoMenu.cur--;
      break;		
  }
}

void menuStopPrinting(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_STOP_PRINT), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackStopPrinting);
}

// Shut down menu, when the hotend temperature is higher than "AUTO_SHUT_DOWN_MAXTEMP"
// wait for cool down, in the meantime, you can shut down by force
void menuCallBackShutDown(void)
{
  static bool lock = false;
  bool shutDown = false;
  bool tempIsLower = true;
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);

  if (lock) return;
  lock = true;
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      shutDown = true;
      break;;

    case KEY_POPUP_CANCEL:
      infoMenu.cur--;
      break;		
  }
  for (ExtUI::extruder_t i = ExtUI::E0; i < EXTRUDERS; i = (ExtUI::extruder_t)(i + 1)) {
    if (ExtUI::getActualTemp_celsius(i) >= AUTO_SHUT_DOWN_MAXTEMP) {
      tempIsLower = false;
    }
  }
  if(tempIsLower || shutDown)
  {
    for (ExtUI::fan_t i = ExtUI::FAN0; i < FAN_COUNT; i = (ExtUI::fan_t)(i + 1))
      ExtUI::setTargetFan_percent(0, i);
    storeCmd(PSTR("M81"));
    infoMenu.cur--;
    popupReminder(textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_SHUTTING_DOWN));
  }
  lock = false;
}

void menuShutDown(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_SHUT_DOWN), textSelect(LABEL_WAIT_TEMP_SHUT_DOWN), textSelect(LABEL_FORCE_SHUT_DOWN), textSelect(LABEL_CANNEL)); 
  for (ExtUI::fan_t i = ExtUI::FAN0; i < FAN_COUNT; i = (ExtUI::fan_t)(i + 1))
    ExtUI::setTargetFan_percent(100, i);
  menuSetFrontCallBack(menuCallBackShutDown);
}
