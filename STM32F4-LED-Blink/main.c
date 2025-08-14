#include "ch.h"
#include "hal.h"

int main(void) {

  halInit();
  chSysInit();

  while (true) {
      palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
      chThdSleepMilliseconds(500);
      palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
      chThdSleepMilliseconds(500);
  }
}
