#include "ch.h"
#include "hal.h"

int mode = 1;

static void button_cb(void *arg) {
  (void)arg;
  mode++;
  if (mode > 8)
    mode = 1;

  palClearPad(GPIOD, GPIOD_LED3);
  palClearPad(GPIOD, GPIOD_LED4);
  palClearPad(GPIOD, GPIOD_LED5);
  palClearPad(GPIOD, GPIOD_LED6);
}

int main(void) {

  halInit();
  chSysInit();
  /* Szükséges: #define PAL_USE_CALLBACKS TRUE */
  palEnablePadEvent(GPIOA, GPIOA_BUTTON, PAL_EVENT_MODE_RISING_EDGE);

  palSetPadCallback(GPIOA, GPIOA_BUTTON, button_cb, NULL);

  while (true) {
    //Lassabb
    if (mode == 1) {
      palTogglePad(GPIOD, GPIOD_LED3);
      palTogglePad(GPIOD, GPIOD_LED4);
      palTogglePad(GPIOD, GPIOD_LED5);
      palTogglePad(GPIOD, GPIOD_LED6);
      chThdSleepMilliseconds(500);
    }
    //Gyorsabb
    else if (mode == 2) {
      palTogglePad(GPIOD, GPIOD_LED3);
      palTogglePad(GPIOD, GPIOD_LED4);
      palTogglePad(GPIOD, GPIOD_LED5);
      palTogglePad(GPIOD, GPIOD_LED6);
      chThdSleepMilliseconds(100);
    }
    //Szembe egymással
    else if (mode == 3) {
      palSetPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }
    //Körbe: 3 - 5 - 6 - 4
    else if (mode == 4) {
      palSetPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }
    //Körbe: 4 - 6 - 5 - 3
    else if (mode == 5) {
      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palSetPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }
    //Egyesével be és egyesével ki
    else if (mode == 6) {
      palSetPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palSetPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palSetPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palSetPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);

      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }
    //Mindegyik LED ki
    else if (mode == 7) {
      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED6);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }
    //Mindegyik LED be
    else if (mode == 8) {
      palSetPad(GPIOD, GPIOD_LED3);
      palSetPad(GPIOD, GPIOD_LED5);
      palSetPad(GPIOD, GPIOD_LED6);
      palSetPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(200);
    }

  }

}
