// ChibiOS-dependencies -------------------
#include "ch.h"
#include "hal.h"

// Declarations ---------------------------
#include "uartShell.h"
#include "chprintf.h"

static thread_t *shelltp = NULL;
static event_listener_t el0;

static const ShellConfig shell_cfg1 = {(BaseSequentialStream *)&SD2, commands};

static const evhandler_t evhndl[] = {uartShellExit};

BaseSequentialStream* uartShellGetHandler() {
  return (BaseSequentialStream *)&SD2;
}

/*
 * Base initialization steps of command line communication over the USB
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

  chEvtRegister(&shell_terminated, &el0, 0);
  return;
}

/*************************************************************
 * Use this function for continuous communication
 * Put this function into the while-cycle of main()
 */
void uartShellKeepAlive(void) { // terminate or/and restart the shell-communication

  if (!shelltp) {
    shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                  shellThread, (void *)&shell_cfg1);
  }
  chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));

  return;
}

/*
 * Shell exit event.
 */
void uartShellExit(eventid_t id) {

  (void)id;
  if (chThdTerminatedX(shelltp)) {
    chThdRelease(shelltp);
    shelltp = NULL;
  }
}

/************************************************************
 * incoming command: echo x
 * - throw back "x" if the COM channel exist
 */
void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]) { // For test of communication
  if (argc > 0)
    chprintf(chp, " >> \"%s\" \r\n\n", argv[0]);
  else
    chprintf(chp, " >> echo what? \r\n\n");
  return;
}

void cmd_smile(BaseSequentialStream *chp, int argc, char *argv[]) { // Smile demo - you have to see it in (for example puTTy), if you type ":)" and hit Enter

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
  chprintf(chp, "                                      @@@@            \r\n\r\n");
  return;
}
