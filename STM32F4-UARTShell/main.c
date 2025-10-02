#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "uartShell.h"



int main(void) {
  halInit();
  chSysInit();

  uartShellInit();

  while (true) {
    uartShellKeepAlive();
    palTogglePad(GPIOD, GPIOD_LED6); /* Blue.  */
    chThdSleepMilliseconds(500);
  }
}
