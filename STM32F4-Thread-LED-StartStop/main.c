#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"

thread_t *blinktp = NULL;  // ide mentjük a thread pointerét

static THD_WORKING_AREA(waBlink, 128);
static THD_FUNCTION(blinkThd, arg) {
  (void)arg;
  chRegSetThreadName("blink");

  thread_t *self = chThdGetSelfX();

  while (!chThdShouldTerminateX()) {
    palTogglePad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(100);
  }

  palClearPad(GPIOD, GPIOD_LED3);
  chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "Blink thread vege. (ptr=%p)\r\n", (void*)self);

  chThdExit(MSG_OK);
}

void cmd_blink(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  if (blinktp == NULL) {
    /* LED thread indítása */
    blinktp = chThdCreateStatic(waBlink, sizeof(waBlink), NORMALPRIO + 1, blinkThd, NULL);
    chprintf(chp, "Blink thread elinditva. (ptr=%p)\r\n", (void*)blinktp);
  } else {
    /* Ha már fut, leállítjuk */
    chThdTerminate(blinktp);  // beállítja a terminate flaget
    chThdWait(blinktp);       // megvárja, amíg a szál valóban befejeződik
    chprintf(chp, "Blink thread biztosan leallitva.\r\n");
    blinktp = NULL;
  }
}

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
