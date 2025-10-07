#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"

#include <stdlib.h>   // atoi, atol, malloc, free, stb.
#include <string.h>   // strcmp, strlen, memcpy, stb.

#define LED_COUNT 4
#define LED_THREAD_STACK 256
#define LED_THREAD_PRIO NORMALPRIO

/* Board-specifikus LED vonalak */
static const ioline_t led_pads[LED_COUNT] = {
  GPIOD_LED3, GPIOD_LED4, GPIOD_LED5, GPIOD_LED6
};

/* Statikus szálak */
static THD_WORKING_AREA(waLed1, LED_THREAD_STACK);
static THD_WORKING_AREA(waLed2, LED_THREAD_STACK);
static THD_WORKING_AREA(waLed3, LED_THREAD_STACK);
static THD_WORKING_AREA(waLed4, LED_THREAD_STACK);

/* Állapot */
bool led_running[LED_COUNT] = { false, false, false, false };
uint32_t led_period_ms[LED_COUNT] = { 500, 700, 300, 1000 };



/* LED thread generikus */
static THD_FUNCTION(LedThread, arg) {
  int idx = (int)(intptr_t)arg;
  chRegSetThreadName("led");

  while (true) {
    if (led_running[idx]) {
      uint32_t period = led_period_ms[idx];

      if (period == 0)
        period = 500;

      palTogglePad(GPIOD, led_pads[idx]);
      chThdSleepMilliseconds(period / 2);
    } else {
      palClearPad(GPIOD, led_pads[idx]);
      chThdSleepMilliseconds(100);
    }
  }
}


/* Shell parancsok */
void led_start(int id, BaseSequentialStream *chp) {
  if (id < 1 || id > LED_COUNT)
  {
    chprintf(chp, "Ismeretlen id\r\n");
    return;
  }

  int idx = id - 1;
  led_running[idx] = true;
  chprintf(chp, "LED %d elinditva\r\n", id);
}

void led_stop(int id, BaseSequentialStream *chp) {
  if (id < 1 || id > LED_COUNT)
  {
    chprintf(chp, "Ismeretlen id\r\n");
    return;
  }

  int idx = id - 1;
  led_running[idx] = false;
  palClearLine(led_pads[idx]);
  chprintf(chp, "LED %d leallitva\r\n", id);
}

void led_speed(int id, uint32_t ms, BaseSequentialStream *chp) {
  if (id < 1 || id > LED_COUNT)
  {
    chprintf(chp, "Ismeretlen id\r\n");
    return;
  }

  int idx = id - 1;
  led_period_ms[idx] = ms;
  chprintf(chp, "LED %d periodus beallitva: %lu ms\r\n", id, (unsigned long)ms);
}

void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {
  if (argc < 2) {
    chprintf(chp, "Hasznalat: led <start|stop|speed> <id> [ms]\r\n");
    return;
  }

  int id = atoi(argv[1]);
  if (strcmp(argv[0], "start") == 0)
    led_start(id, chp);
  else if (strcmp(argv[0], "stop") == 0)
    led_stop(id, chp);
  else if (strcmp(argv[0], "speed") == 0)
  {
    if (argc < 3)
    {
      chprintf(chp, "speed parancsnak szukseges: <ms>\r\n");
      return;
    }
    uint32_t ms = atoi(argv[2]);
    led_speed(id, ms, chp);
  } else
    chprintf(chp, "Ismeretlen alparancs\r\n");
}

int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  /* Statikus LED szálak indítása */
  chThdCreateStatic(waLed1, sizeof(waLed1), LED_THREAD_PRIO, LedThread, (void*)0);
  chThdCreateStatic(waLed2, sizeof(waLed2), LED_THREAD_PRIO, LedThread, (void*)1);
  chThdCreateStatic(waLed3, sizeof(waLed3), LED_THREAD_PRIO, LedThread, (void*)2);
  chThdCreateStatic(waLed4, sizeof(waLed4), LED_THREAD_PRIO, LedThread, (void*)3);

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    chThdSleepMilliseconds(500);
  }
}
