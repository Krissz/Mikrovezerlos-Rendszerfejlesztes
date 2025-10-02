#ifndef USHELL_H_
#define USHELL_H_
// ChibiOS dependencies
#include "ch.h"
#include "hal.h"
#include "usbcfg.h"

#include "shell.h"

/* SHELL Command Line Communication settings */
#define SHELL_WA_SIZE  THD_WORKING_AREA_SIZE(2048)

// BaseSequentialStream for USB CDC
BaseSequentialStream* cdcShellGetHandler(void);

// Prototypes of Commands
void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_smile(BaseSequentialStream *chp, int argc, char *argv[]);

void cdcShellInit(void);
void cdcShellExit(eventid_t id);
void cdcShellKeepAlive(void);

// Shell command list
static const ShellCommand commands[] = {
  {"echo", cmd_echo},
  {":)", cmd_smile},
  {NULL, NULL}
};

#endif /* USHELL_H_ */
