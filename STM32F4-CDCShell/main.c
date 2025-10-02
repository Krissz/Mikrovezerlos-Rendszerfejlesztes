#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "cdcShell.h"



int main(void) {
  halInit();
  chSysInit();

  cdcShellInit();

  while (true) {
    cdcShellKeepAlive();
    palTogglePad(GPIOD, GPIOD_LED6); /* Blue.  */
    chThdSleepMilliseconds(500);
  }
}
