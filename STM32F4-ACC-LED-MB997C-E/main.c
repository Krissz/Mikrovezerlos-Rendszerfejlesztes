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

static THD_WORKING_AREA(waLEDXThd, 128);
static THD_FUNCTION(LEDXThd, arg) {

  chRegSetThreadName("LEDXThd");
  while(true)
  {
    float acc_X = acccooked[0];
    int sleep_X = (int)(fabs(acc_X));
    if(sleep_X == 0) sleep_X = 100;
    if(sleep_X > 1000) sleep_X = 1000;

    if(acc_X > 20.0f) //20mG = 0.02G
    {
      palClearPad(GPIOD, GPIOD_LED4);
      palTogglePad(GPIOD, GPIOD_LED5);
      chThdSleepMilliseconds(sleep_X);
    }
    else if(acc_X < -20.0f)
    {
      palClearPad(GPIOD, GPIOD_LED5);
      palTogglePad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(sleep_X);
    }
    else
    {
      palClearPad(GPIOD, GPIOD_LED5);
      palClearPad(GPIOD, GPIOD_LED4);
      chThdSleepMilliseconds(100);
    }

  }
}

static THD_WORKING_AREA(waLEDYThd, 128);
static THD_FUNCTION(LEDYThd, arg) {

  chRegSetThreadName("LEDYThd");
  while(true)
  {
    float acc_Y = acccooked[1];
    int sleep_Y = (int)(fabs(acc_Y));
    if(sleep_Y == 0) sleep_Y = 100;
    if(sleep_Y > 1000) sleep_Y = 1000;

    if(acc_Y > 20.0f) //20mG = 0.02G
    {
      palClearPad(GPIOD, GPIOD_LED6);
      palTogglePad(GPIOD, GPIOD_LED3);
      chThdSleepMilliseconds(sleep_Y);
    }
    else if(acc_Y < -20.0f)
    {
      palClearPad(GPIOD, GPIOD_LED3);
      palTogglePad(GPIOD, GPIOD_LED6);
      chThdSleepMilliseconds(sleep_Y);
    }
    else
    {
      palClearPad(GPIOD, GPIOD_LED3);
      palClearPad(GPIOD, GPIOD_LED6);
      chThdSleepMilliseconds(100);
    }

  }
}

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

    float acc_X = acccooked[0];
    int sleep_X = (int)(fabs(acc_X));
    if(sleep_X == 0) sleep_X = 100;
    if(sleep_X > 1000) sleep_X = 1000;


    float acc_Y = acccooked[1];
    int sleep_Y = (int)(fabs(acc_Y));
    if(sleep_Y == 0) sleep_Y = 100;
    if(sleep_Y > 1000) sleep_Y = 1000;

    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "Sleep_X: %d Sleep_Y: %d",sleep_X, sleep_Y);
    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC),"\r\n");
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

  chThdCreateStatic(waLEDXThd, sizeof(waLEDXThd), NORMALPRIO + 2, LEDXThd, NULL);
  chThdCreateStatic(waLEDYThd, sizeof(waLEDYThd), NORMALPRIO + 2, LEDYThd, NULL);

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    chThdSleepMilliseconds(500);
  }
}
