#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "shell_manager.h"

thread_t *shelltp = NULL;
static event_listener_t el0;
static ShellConfig shell_cfg;

// Event handler for shell termination
static const evhandler_t evhndl[] = {smShellExit};

/*
 * Returns the appropriate stream handler based on the interface
 */
BaseSequentialStream* smShellGetHandler(ShellInterface interface) {
  switch (interface) {
    case SHELL_INTERFACE_UART:
      return (BaseSequentialStream *)&SD2;
    case SHELL_INTERFACE_CDC:
      return (BaseSequentialStream *)&SDU1;
    default:
      return NULL;
  }
}

/*
 * Initializes the shell for the specified interface
 */
void smShellInitInterface(ShellInterface interface) {
  // Initialize shell manager
  shellInit();

  if (interface == SHELL_INTERFACE_UART) {
    // Activates the serial driver 2 using the driver default configuration
    // PA2(TX) and PA3(RX) are routed to USART2
    sdStart(&SD2, NULL);
    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
  } else if (interface == SHELL_INTERFACE_CDC) {
    // Initializes and starts the serial-over-USB CDC driver
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    // Activates the USB driver and the USB bus pull-up on D+
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
  }

  // Configure shell with the appropriate stream
  shell_cfg.sc_channel = smShellGetHandler(interface);
  shell_cfg.sc_commands = commands;

  chEvtRegister(&shell_terminated, &el0, 0);
}

/*
 * Keeps the shell alive, to be called in the main loop
 */
void smShellKeepAlive(ShellInterface interface) {
  bool is_usb_active = true;
  if (interface == SHELL_INTERFACE_CDC) {
    // Check if USB driver is initialized and active
    is_usb_active = (SDU1.config != NULL && SDU1.config->usbp->state == USB_ACTIVE);
  }

  if (!shelltp && is_usb_active) {
    shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                  shellThread, (void *)&shell_cfg);
  }
  chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
}

/*
 * Shell exit event handler
 */
void smShellExit(eventid_t id) {
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
}
