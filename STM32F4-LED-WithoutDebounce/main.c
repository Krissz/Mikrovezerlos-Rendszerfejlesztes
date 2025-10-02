#include "ch.h"
#include "hal.h"
#include <stdbool.h>
#include <stdint.h>

/* PIN beállítások */
#define BUTTON1_PORT    GPIOA
#define BUTTON1_PAD     GPIOA_BUTTON   /* board.h alias, pl. PA0 */
#define LED1_PORT       GPIOD
#define LED1_PAD        GPIOD_LED3     /* board.h alias, pl. PD13 */


int main(void) {
    halInit();
    chSysInit();

    /* LED kimenet alapállapotban kikapcsolva */
    palClearPad(LED1_PORT, LED1_PAD);

    bool prevState = true;  /* feltételezzük: pulldown → nyugalmi LOW */
    while (true) {

        bool currState = palReadPad(BUTTON1_PORT, BUTTON1_PAD);
        /* gomb aktív HIGH → akkor van lenyomva, ha currState == 1 */

        if ((prevState == true) && (currState == false)) {
            /*lefutó élváltás: HIGH → LOW → gomb elengedve */
            palTogglePad(LED1_PORT, LED1_PAD);
        }

        prevState = currState;
    }
}
