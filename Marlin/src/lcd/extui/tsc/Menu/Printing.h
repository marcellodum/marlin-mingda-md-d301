#ifndef _PRINTING_H_
#define _PRINTING_H_

/**
  * Maximum hotend temperature of automatic shut down after printing.
  * When enable automatic shutdown(Auto Power), when the hotend temperature is higher than this value
  * turn on the fan to cool down, wait for the hotend temperature to be lower than this value, then turn off the power automatically
  */
#define AUTO_SHUT_DOWN_MAXTEMP 50

bool isPrinting(void);
bool isPaused(void);
bool setPrintPause(bool is_pause);

void menuPrinting(void);
void menuStopPrinting(void);
void menuShutDown(void);

#endif
