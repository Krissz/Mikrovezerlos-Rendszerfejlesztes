#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"

// Declarations
#include "cdcShell.h"

static thread_t *shelltp = NULL;
static event_listener_t el0;

static const ShellConfig shell_cfg1 = {(BaseSequentialStream *)&SDU1, commands};

static const evhandler_t evhndl[] = {cdcShellExit};

BaseSequentialStream* cdcShellGetHandler(void) {
  return (BaseSequentialStream *)&SDU1;
}

/*
 * Base initialization steps of command line communication over USB CDC
 */
void cdcShellInit(void) {
  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Initializes and starts the serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and the USB bus pull-up on D+.
   * Includes a delay to avoid reconnecting issues after reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  chEvtRegister(&shell_terminated, &el0, 0);
  return;
}

/*
 * Use this function for continuous communication
 * Put this function into the while-cycle of main()
 */
void cdcShellKeepAlive(void) {
  if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE)) {
    shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                  shellThread, (void *)&shell_cfg1);
  }
  chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
  return;
}

/*
 * Shell exit event.
 */
void cdcShellExit(eventid_t id) {
  (void)id;
  if (chThdTerminatedX(shelltp)) {
    chThdRelease(shelltp);
    shelltp = NULL;
  }
}

/*
 * Command: echo x
 * - Echoes back "x" if the communication channel exists
 */
void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]) {
  if (argc > 0)
    chprintf(chp, " >> \"%s\" \r\n\n", argv[0]);
  else
    chprintf(chp, " >> echo what? \r\n\n");
  return;
}

/*
 * Command: :)
 * - Displays an ASCII smiley face
 */
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
  chprintf(chp, "                                      @@@@            \r\n\r\n");
  return;
}
