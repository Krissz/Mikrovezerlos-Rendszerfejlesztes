#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"



void cmd_LEDOn(BaseSequentialStream *chp, int argc, char *argv[])
{
  palSetPad(GPIOD, GPIOD_LED3);
  palSetPad(GPIOD, GPIOD_LED4);
  palSetPad(GPIOD, GPIOD_LED5);
  palSetPad(GPIOD, GPIOD_LED6);

  chprintf(chp, "LED-ek bekapcsolva\r\n");
}

void cmd_LEDOff(BaseSequentialStream *chp, int argc, char *argv[])
{
  palClearPad(GPIOD, GPIOD_LED3);
  palClearPad(GPIOD, GPIOD_LED4);
  palClearPad(GPIOD, GPIOD_LED5);
  palClearPad(GPIOD, GPIOD_LED6);

  chprintf(chp, "LED-ek kikapcsolva\r\n");
}

//LED 1 1 0 0
void cmd_LED(BaseSequentialStream *chp, int argc, char *argv[])
{
  if(argc > 4)
  {
    chprintf(chp, "Hibas parameterszam (max:4), megadott: %d \r\n", argc);
    return;
  }

  for(int i = 0; i<argc; i++)
  {
    int ledstatus = atoi(argv[i]);

    //LEDek lábszáma: 12,13,14,15
    if(ledstatus)
    {
      palSetPad(GPIOD, 12 + i);
    }
    else
    {
      palClearPad(GPIOD, 12 + i);
    }

  }
}


int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
   // palTogglePad(GPIOD, GPIOD_LED6); /* Blue. */
    chThdSleepMilliseconds(500);
  }
}
