#include "BabyStep.h"
#include "../TSC_Menu.h"
#include "../../../../../feature/babystep.h"
#include "../../../../../module/configuration_store.h"

//1 title, ITEM_PER_PAGE items(icon+label) 
MENUITEMS babyStepItems = {
//title
  LABEL_BABYSTEP,
//icon                        label
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

#define ITEM_BABYSTEP_UNIT_NUM 3
const ITEM itemBabyStepUnit[ITEM_BABYSTEP_UNIT_NUM] = {
// icon                       label
  {ICON_001_MM,               LABEL_001_MM},
  {ICON_01_MM,                LABEL_01_MM},
  {ICON_1_MM,                 LABEL_1_MM},
};
const float item_babystep_unit[ITEM_BABYSTEP_UNIT_NUM] = {0.01f, 0.1f, 1};

static ELEMENTS elementsUnit;

static void initElements(uint8_t position)
{
  elementsUnit.totaled = ITEM_BABYSTEP_UNIT_NUM;
  elementsUnit.list = itemBabyStepUnit;
  elementsUnit.ele  = item_babystep_unit;

  for(uint8_t i=0; i<elementsUnit.totaled; i++)
  {
    if(memcmp(&elementsUnit.list[i], &babyStepItems.items[position], sizeof(ITEM)) == 0)
    {
      elementsUnit.cur = i;
      break;
    }
  }
}

static float getBabyStepZAxisTotalMM(){
  return babystep.axis_total[BS_TOTAL_AXIS(Z_AXIS)] * planner.steps_to_mm[Z_AXIS];
}

static void setBabyStepZAxisIncMM(float mm){
  babystep.add_mm(Z_AXIS, mm);
}

void showBabyStep(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getBabyStepZAxisTotalMM(), 3, 2, RIGHT);
}
void babyStepReDraw(void)
{
  GUI_DispFloat(CENTER_X - 5*BYTE_WIDTH/2, CENTER_Y, getBabyStepZAxisTotalMM(), 3, 2, RIGHT);
}


void menuCallBackBabyStep(void)
{
  static float baby_step_value = 0.0f;
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      setBabyStepZAxisIncMM(-elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_3:
      setBabyStepZAxisIncMM(elementsUnit.ele[elementsUnit.cur]);
      break;
    case KEY_ICON_4:
      settings.save();
      break;
    case KEY_ICON_5:
      elementsUnit.cur = (elementsUnit.cur + 1) % elementsUnit.totaled;
      babyStepItems.items[key_num] = elementsUnit.list[elementsUnit.cur];
      menuDrawItem(&babyStepItems.items[key_num], key_num);
      break;
    case KEY_ICON_6:
      setBabyStepZAxisIncMM(-baby_step_value);
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default :
      break;
  }
  if(baby_step_value != getBabyStepZAxisTotalMM())
  {
    baby_step_value = getBabyStepZAxisTotalMM();
    babyStepReDraw();
  }
}

void menuBabyStep()
{
  initElements(KEY_ICON_5);
  menuDrawPage(&babyStepItems);
  showBabyStep();
  menuSetFrontCallBack(menuCallBackBabyStep);
}
