#include "ch.h"
#include "hal.h"
#include "shell_manager.h"

#define CHPRINTF_USE_FLOAT          TRUE
#include "chprintf.h"

#include "lis302dl.h"

/*===========================================================================*/
/* LIS302DL related.                                                          */
/*===========================================================================*/

/* LIS302DL Driver: This object represent an LIS302DL instance */
static LIS302DLDriver LIS302DLD1;

static int32_t accraw[LIS302DL_ACC_NUMBER_OF_AXES];
static float acccooked[LIS302DL_ACC_NUMBER_OF_AXES];
static char axisID[LIS302DL_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};

static const SPIConfig spicfg = {
  FALSE,
  NULL,
  GPIOE,
  GPIOE_CS_SPI,
  SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,
  0
};

static LIS302DLConfig lis302dlcfg = {
  &SPID1,
  &spicfg,
  NULL,
  NULL,
  LIS302DL_ACC_FS_2G,
  LIS302DL_ACC_ODR_100HZ,
#if LIS302DL_USE_ADVANCED
  LIS302DL_ACC_HP_1,
#endif
};



static THD_WORKING_AREA(waAccThd, 128);
static THD_FUNCTION(AccThd, arg) {
  (void)arg;
  chRegSetThreadName("accThd");

  /* LIS302DL Object Initialization.*/
  lis302dlObjectInit(&LIS302DLD1);
  /* Activates the LIS302DL driver.*/
  lis302dlStart(&LIS302DLD1, &lis302dlcfg);

  while (true) {

    lis302dlAccelerometerReadRaw(&LIS302DLD1, accraw);
    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "LIS302DLD1 Raw: ");
    for(uint8_t i = 0; i < LIS302DL_ACC_NUMBER_OF_AXES; i++) {
      chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "%c: %d ", axisID[i], accraw[i]);
    }

    lis302dlAccelerometerReadCooked(&LIS302DLD1, acccooked);
    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), " Cooked: ");
    for(uint8_t i = 0; i < LIS302DL_ACC_NUMBER_OF_AXES; i++) {
      chprintf(smShellGetHandler(SHELL_INTERFACE_CDC), "%c: %.3f ", axisID[i], acccooked[i]/1000);
    }
    chprintf(smShellGetHandler(SHELL_INTERFACE_CDC),"\r\n");

    chThdSleepMilliseconds(100);
  }

  lis302dlStop(&LIS302DLD1);
}


int main(void) {
  halInit();
  chSysInit();

  // Initialize shell with desired interface (e.g., UART or CDC)
  smShellInitInterface(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC

  /* Creates the Acc thread.*/
  chThdCreateStatic(waAccThd, sizeof(waAccThd), NORMALPRIO + 1, AccThd, NULL);

  while (true) {
    smShellKeepAlive(SHELL_INTERFACE_CDC); // Change to SHELL_INTERFACE_CDC for CDC
    chThdSleepMilliseconds(500);
  }
}
