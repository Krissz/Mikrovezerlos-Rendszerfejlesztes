#include "ch.h"
#include "hal.h"
#include "shell_manager.h"

#define CHPRINTF_USE_FLOAT          TRUE
#include "chprintf.h"

#include "lis3dsh.h"

/*===========================================================================*/
/* LIS3DSH related.                                                          */
/*===========================================================================*/

/* LIS3DSH Driver: This object represent an LIS3DSH instance */
static LIS3DSHDriver LIS3DSHD1;

static int32_t accraw[LIS3DSH_ACC_NUMBER_OF_AXES];
static float acccooked[LIS3DSH_ACC_NUMBER_OF_AXES];
static char axisID[LIS3DSH_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};

static const SPIConfig spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = NULL,
  .ssport           = GPIOE,
  .sspad            = GPIOE_CS_SPI,
  .cr1              = SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,
  .cr2              = 0U
};

static LIS3DSHConfig lis3dshcfg = {
  .spip             = &SPID1,
  .spicfg           = &spicfg,
  .accsensitivity   = NULL,
  .accbias          = NULL,
  .accfullscale     = LIS3DSH_ACC_FS_2G,
  .accodr           = LIS3DSH_ACC_ODR_100HZ,
#if LIS3DSH_USE_ADVANCED
  .accantialiasing  = LIS3DSH_ACC_BW_400HZ,
  .accbdu           = LIS3DSH_ACC_BDU_CONTINUOUS
#endif
};

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


static THD_WORKING_AREA(waAccThd, 128);
static THD_FUNCTION(AccThd, arg) {
  (void)arg;
  chRegSetThreadName("accThd");

  /* LIS3DSH Object Initialization.*/
  lis3dshObjectInit(&LIS3DSHD1);

  /* Activates the LIS3DSH driver.*/
  lis3dshStart(&LIS3DSHD1, &lis3dshcfg);

  while (true) {

    lis3dshAccelerometerReadCooked(&LIS3DSHD1, acccooked);
    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), " Cooked: ");
    for(uint8_t i = 0; i < LIS3DSH_ACC_NUMBER_OF_AXES; i++) {
      chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "%c: %.3f ", axisID[i], acccooked[i]/1000);
    }

    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC),"\r\n");

    float acc_X = acccooked[0];
    int pwmX = (int)(fabs(acc_X));
    if(pwmX < -1000) pwmX = -1000;
    if(pwmX > 1000) pwmX = 1000;

    float acc_Y = acccooked[1];
    int pwmY = (int)(fabs(acc_Y));
    if(pwmY < -1000) pwmY = -1000;
    if(pwmY > 1000) pwmY = 1000;

    //0: Zöld bal, 1: narancs fel,  2: piros jobb, 3 kék le
    if(acc_X < 0)
    {
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, pwmX * 10));
      pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
    }
    else if(acc_X > 0)
    {
      pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, pwmX * 10));
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
    }
    else
    {
      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
      pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
    }

    if(acc_Y < 0)
    {
      pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
      pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, pwmY * 10));
    }
    else if(acc_Y > 0)
    {
      pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, pwmY * 10));
      pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));

    }
    else
    {
      pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
      pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
    }



    chThdSleepMilliseconds(100);
  }

  lis3dshStop(&LIS3DSHD1);
}


int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  /* Creates the Acc thread.*/
  chThdCreateStatic(waAccThd, sizeof(waAccThd), NORMALPRIO + 1, AccThd, NULL);

  pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED3, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_ALTERNATE(2));

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    chThdSleepMilliseconds(500);
  }
}
