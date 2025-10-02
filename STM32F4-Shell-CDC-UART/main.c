#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"

int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    palTogglePad(GPIOD, GPIOD_LED6); /* Blue. */
    chThdSleepMilliseconds(500);
  }
}
