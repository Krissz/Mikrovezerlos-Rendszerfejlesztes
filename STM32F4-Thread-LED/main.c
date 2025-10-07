#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"

static THD_WORKING_AREA(waBlink, 128);
static THD_FUNCTION(blinkThd, arg) {
  (void)arg;
  chRegSetThreadName("blink");

  while (true) {
    palTogglePad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(100);
  }
}


int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  // Blink Thread start
  chThdCreateStatic(waBlink, sizeof(waBlink), NORMALPRIO + 1, blinkThd, NULL);

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    palTogglePad(GPIOD, GPIOD_LED6); /* Blue. */
    chThdSleepMilliseconds(500);
  }
}
