#include "UBL.h"
#include "../TSC_Menu.h"

#if ENABLED(MINGDA_TFT)

static uint8_t ublSlot;
static bool ublIsSaving = true;
static bool ublSlotSaved = false;

//1 title, ITEM_PER_PAGE items(icon+label) 
MENUITEMS ublItems = {
//title
  LABEL_UBL,
//icon                        label
 {{ICON_LEVELING, LABEL_START},
  {ICON_EEPROM_SAVE, LABEL_SAVE},
  {ICON_EEPROM_RESTORE, LABEL_LOAD},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACK,       LABEL_BACK},}
};

// Start UBL process
void ublStart(void)
{
  storeCmd("G28\n");
  storeCmd("G29 P1\n");
  // Run this multiple times since it only fills some missing points, not all.
  storeCmd("G29 P3\n");
  storeCmd("G29 P3\n");
  storeCmd("G29 P3\n");
  storeCmd("M500\n");
  storeCmd("M118 A1 UBL Completed\n");
}

void menuCallBackUBL(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      ublStart();
      break;
    case KEY_ICON_1:
      menuUBLSave();
      break;
    case KEY_ICON_2:
      menuUBLLoad();
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default :
      break;
  }
}

void menuUBL()
{
  menuDrawPage(&ublItems);
  menuSetFrontCallBack(menuCallBackUBL);
}

void menuUBLSaveLoad(void)
{
  MENUITEMS UBLSaveLoadItems = {
    // title
    LABEL_ABL_SETTINGS_UBL_SAVE,
    // icon                          label
    {
      {ICON_EEPROM_SAVE,             LABEL_ABL_SLOT0},
      {ICON_EEPROM_SAVE,             LABEL_ABL_SLOT1},
      {ICON_EEPROM_SAVE,             LABEL_ABL_SLOT2},
      {ICON_EEPROM_SAVE,             LABEL_ABL_SLOT3},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  if (!ublIsSaving)
  {
    UBLSaveLoadItems.title.index = LABEL_ABL_SETTINGS_UBL_LOAD;
    for (int i = 0; i < 4; i++)
    {
      UBLSaveLoadItems.items[i].icon = ICON_EEPROM_RESTORE;
    }
  }

  menuDrawPage(&UBLSaveLoadItems);
  menuSetFrontCallBack(menuCallBackUBLSaveLoadItems);
}

void menuCallBackUBLSaveLoadItems(void) {
  KEY_VALUES key_num = KEY_IDLE;
  key_num = menuKeyGetValue();
  switch (key_num)
  {
    case KEY_ICON_0:
    case KEY_ICON_1:
    case KEY_ICON_2:
    case KEY_ICON_3:
      ublSlot = key_num;
      infoMenu.menu[++infoMenu.cur] = menuUBLLoadPopup;
      break;

    case KEY_ICON_7:
      if (ublSlotSaved == true)
      {
        ublSlotSaved = false;
        infoMenu.menu[++infoMenu.cur] = menuUBLSavePopup;
      }
      else
      {
        ublSlotSaved = false;
        infoMenu.cur--;
      }
      break;

    default:
      break;
  }
}

void menuUBLSave(void)
{
  ublIsSaving = true;
  infoMenu.menu[++infoMenu.cur] = menuUBLSaveLoad;
}

void menuUBLLoad(void)
{
  ublIsSaving = false;
  infoMenu.menu[++infoMenu.cur] = menuUBLSaveLoad;
}

void ublSaveloadConfirm(void)
{
  char gcode_command[20];
  if (!ublIsSaving)
  {
    sprintf(gcode_command, "G29 L%d\n", ublSlot);
    storeCmd(gcode_command);
  }
  else
  {
    sprintf(gcode_command, "G29 S%d\n", ublSlot);
    ublSlotSaved = storeCmd(gcode_command);
  }
}

void menuCallBackDialogUBLSaveLoad(void)
{
  uint16_t key_num = KEY_GetValue(2, doubleBtnRect);
  switch(key_num)
  {
    case KEY_POPUP_CONFIRM:
      ublSaveloadConfirm();
      infoMenu.cur--;
      break;

    case KEY_POPUP_CANCEL:
      infoMenu.cur--;
      break;		
  }
}

void menuUBLLoadPopup(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_ABL_SETTINGS_UBL_LOAD), textSelect(LABEL_CONFIRMATION), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackDialogUBLSaveLoad);
}

void menuUBLSavePopup(void)
{
  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_ABL_SETTINGS_UBL_SAVE), textSelect(LABEL_ABL_SLOT_EEPROM), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANNEL));
  menuSetFrontCallBack(menuCallBackDialogUBLSaveLoad);
}

#endif