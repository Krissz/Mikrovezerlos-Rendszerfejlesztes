#ifndef SHELL_MANAGER_H_
#define SHELL_MANAGER_H_

// ChibiOS dependencies
#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "shell.h"

// Shell Command Line Communication settings
#define SHELL_WA_SIZE  THD_WORKING_AREA_SIZE(2048)

// Shell interface type
typedef enum {
  SHELL_INTERFACE_UART,
  SHELL_INTERFACE_CDC
} ShellInterface;

// External declaration for shell thread pointer
extern thread_t *shelltp;

// Prototypes
BaseSequentialStream* smShellGetHandler(ShellInterface interface);
void smShellInitInterface(ShellInterface interface);
void smShellKeepAlive(ShellInterface interface);
void smShellExit(eventid_t id);

// Command prototypes
void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_smile(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blink(BaseSequentialStream *chp, int argc, char *argv[]);

// Shell command list
static const ShellCommand commands[] = {
  {"echo", cmd_echo},
  {":)", cmd_smile},
  {"blink", cmd_blink},
  {NULL, NULL}
};

#endif /* SHELL_MANAGER_H_ */
