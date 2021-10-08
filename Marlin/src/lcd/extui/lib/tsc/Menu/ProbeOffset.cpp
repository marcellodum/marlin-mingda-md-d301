#include "../TSC_Menu.h"
#include "../../../../../module/configuration_store.h"

//1 titl, ITEM_PER_PAGE item
MENUITEMS probeOffsetItems = {
// title
LABEL_PROBE_OFFSET,
// icon                        label
 {{ICON_DEC,                  LABEL_DEC},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_INC,                  LABEL_INC},
  {ICON_EEPROM_SAVE,          LABEL_EEPROM_SAVE},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_NORMAL_SPEED,         LABEL_VALUE_ZERO},
  {ICON_BACK,                 LABEL_BACK},}
};

typedef struct
{
  const ITEM *list;
  const float *ele;
  uint8_t cur;
  uint8_t totaled;
}ELEMENTS;

#define ITEM_PROBE_OFFSET_UNIT_NUM 3
const ITEM itemProbeOffsetUnit[ITEM_PROBE_OFFSET_UNIT_NUM] = {
// icon                       label
  {ICON_001_MM,               LABEL_001_MM},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_1_MM,                 LABEL_1_MM},
};
const float item_probeOffset_unit[ITEM_PROBE_OFFSET_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;

static void initElements(uint8_t position)
{
  elementsUnit.totaled = ITEM_PROBE_OFFSET_UNIT_NUM;
  elementsUnit.list = itemProbeOffsetUnit;
  elementsUnit.ele  = item_probeOffset_unit;

  for(uint8_t i=0; i<elementsUnit.totaled; i++)
  {
    if(memcmp(&elementsUnit.list[i], &probeOffsetItems.items[position], sizeof(ITEM)) == 0)
    {
      elementsUnit.cur = i;
      break;
    }
  }
}

/* Get current offset */
float getCurrentOffset(void)
{  
  return ExtUI::getZOffset_mm();
}
/* Set current offset */
void setCurrentOffset(float offset)
{  
  ExtUI::setZOffset_mm(offset);
}

void showProbeOffset(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getCurrentOffset(), 3, 2, RIGHT);
}
void probeOffsetReDraw(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getCurrentOffset(), 3, 2, RIGHT);
}


void menuCallBackProbeOffset(void)
{
  static float probe_offset_value = 0.0f;
  KEY_VALUES key_num =  menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setCurrentOffset(probe_offset_value - elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_3:
      setCurrentOffset(probe_offset_value + elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_4:
      settings.save();;
      break;
    case KEY_ICON_5:
      elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
      probeOffsetItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
      menuDrawItem(&probeOffsetItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      setCurrentOffset(0);
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default :
      break;
  }
  if(probe_offset_value != getCurrentOffset())
  {
    probe_offset_value = getCurrentOffset();
    probeOffsetReDraw();
  }
}


void menuProbeOffset()
{
  initElements(KEY_ICON_5);
  menuDrawPage(&probeOffsetItems);
  showProbeOffset();
  menuSetFrontCallBack(menuCallBackProbeOffset);
}

