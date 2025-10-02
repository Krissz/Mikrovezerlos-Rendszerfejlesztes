// ChibiOS-dependencies -------------------
#include "ch.h"
#include "hal.h"

// Declarations ---------------------------
#include "uartShell.h"
#include "chprintf.h"

// Static working area for shell thread
static THD_WORKING_AREA(waShell, SHELL_WA_SIZE);

static const ShellConfig shell_cfg1 = {(BaseSequentialStream*)&SD2, commands};

BaseSequentialStream* uartShellGetHandler() {
  return (BaseSequentialStream*)&SD2;
}

/*
 * Base initialization steps of command line communication over the UART
 */
void uartShellInit(void) {
  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
   * Create static shell thread
   */
  chThdCreateStatic(waShell, sizeof(waShell), NORMALPRIO + 1, shellThread,
                    (void*)&shell_cfg1);

  return;
}

/*
 * incoming command: echo x
 * - throw back "x" if the COM channel exists
 */
void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]) {
  if (argc > 0)
    chprintf(chp, " >> \"%s\" \r\n\n", argv[0]);
  else
    chprintf(chp, " >> echo what? \r\n\n");
  return;
}

void cmd_smile(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  chprintf(chp, "                                @              @      \r\n");
  chprintf(chp, "                               @@@            @@@     \r\n");
  chprintf(chp, "                               @@@            @@@     \r\n");
  chprintf(chp, "                                @              @      \r\n");
  chprintf(chp, "                           ~~                      ~~ \r\n");
  chprintf(chp, "                             @                    @   \r\n");
  chprintf(chp, "                              @                  @    \r\n");
  chprintf(chp, "                               @                @     \r\n");
  chprintf(chp, "                                @              @      \r\n");
  chprintf(chp, "                                 @@          @@       \r\n");
  chprintf(chp, "                                   @@@    @@@         \r\n");
  chprintf(chp,
           "                                      @@@@            \r\n\r\n");
  return;
}
