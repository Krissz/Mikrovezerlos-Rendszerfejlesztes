#ifndef UARTSHELL_H_
#define UARTSHELL_H_
// ChibiOS-dependencies
#include "ch.h"
#include "hal.h"
///////////////////////

#include "shell.h"


/* SHELL Command Line Communication settings */
#define SHELL_WA_SIZE  THD_WORKING_AREA_SIZE(2048)

BaseSequentialStream* uartShellGetHandler(void);

// Prototypes of Commands ---------------------------------
void cmd_echo(BaseSequentialStream *CHP, int argc, char *argv[]);
void cmd_smile(BaseSequentialStream *CHP, int argc, char *argv[]);

void uartShellExit(eventid_t id);
void uartShellInit(void);
void uartShellKeepAlive(void);
//---------------------------------------------------------

static const ShellCommand commands[] = { // List of commands
  {"echo", cmd_echo},
  {":)", cmd_smile},
  {NULL, NULL}
};//-------------------------------------------------------



#endif /* UARTSHELL_H_ */
