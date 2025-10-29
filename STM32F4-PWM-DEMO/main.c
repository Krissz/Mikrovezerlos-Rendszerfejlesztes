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


int main(void) {
  halInit();
  chSysInit();


  /*
   * Initializes the PWM driver 2 and ICU driver 3.
   * GPIOA8 is the PWM output.
   * GPIOC6 is the ICU input.
   * The two pins have to be externally connected together.
   */
  pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED3, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_ALTERNATE(2));


  while (true) {

    /*
     *  100 Hz.
     */
    pwmChangePeriod(&PWMD4, 100);  /*  PWM period in ticks     */

    /*
     * 75% duty cycle at 100Hz.
     */
    pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
    pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
    pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 7500));
    chThdSleepMilliseconds(5000);

    /*
     * 50% duty cycle at 100Hz.
     */
    pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
    pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
    pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
    chThdSleepMilliseconds(5000);

    /*
     * 25% duty cycle at 100Hz.
     */
    pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 2500));
    pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 2500));
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 2500));
    pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 2500));
    chThdSleepMilliseconds(5000);

    /*
     * 2.5% duty cycle at 10Hz.
     */
    pwmChangePeriod(&PWMD4, 1000);  /*  PWM period in ticks     */
    chThdSleepMilliseconds(5000);


  }


  /*
   * Disables channel 0 and stops the drivers.
   */
  pwmDisableChannel(&PWMD4, 0);
  pwmStop(&PWMD4);

}
