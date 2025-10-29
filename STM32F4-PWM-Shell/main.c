#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"


static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
}

static PWMConfig pwmcfg = {
  10000,                                    /* 10kHz PWM clock frequency.   */
  100,                                      /* Initial PWM period in ticks     */
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
  },
  0,
  0
};

int runPulse = false;

static THD_WORKING_AREA(waPulse, 128);
static THD_FUNCTION(pulseThd, arg) {
  (void)arg;
  chRegSetThreadName("pulse");

  while (true) {
    if(!runPulse)
    {
      chThdSleepMilliseconds(100);
      continue;
    }

    for(int i = 0; i<10000; i+=10)
    {
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      chThdSleepMilliseconds(2);
    }
    for(int i = 10000; i>=0; i-=10)
    {
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, i));
      chThdSleepMilliseconds(2);
    }
  }
}



void cmd_half(BaseSequentialStream *chp, int argc, char *argv[])
{
  pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
  pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
  pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
  pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
}

void cmd_full(BaseSequentialStream *chp, int argc, char *argv[])
{
  pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
  pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
  pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
  pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
}

void cmd_off(BaseSequentialStream *chp, int argc, char *argv[])
{
  pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
  pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
  pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
  pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));

}

void cmd_manual(BaseSequentialStream *chp, int argc, char *argv[])
{
  int ledid = atoi(argv[0]);
  int ledpwm = atoi(argv[1]);

  pwmEnableChannel(&PWMD4, ledid, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, (ledpwm*100)));
}

void cmd_pulse(BaseSequentialStream *chp, int argc, char *argv[])
{
  runPulse = !runPulse;
}



int main(void) {
  halInit();
  chSysInit();

  pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED3, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_ALTERNATE(2));

  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  // Blink Thread start
  chThdCreateStatic(waPulse, sizeof(waPulse), NORMALPRIO + 1, pulseThd, NULL);

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    chThdSleepMilliseconds(500);
  }

}
