#include "ch.h"
#include "hal.h"
#include <stdbool.h>
#include <stdint.h>

/* PIN beállítások (pad számok, ne bitmask) */
#define BUTTON1_PORT    GPIOA
#define BUTTON1_PAD     GPIOA_BUTTON
#define LED1_PORT       GPIOD
#define LED1_PAD        GPIOD_LED3

static uint8_t TimeInterval = 5; /* ms: gomb vizsgálati időköz */

/* Debounce függvény (ugyanaz a logika mint az eredeti) */
static bool BtnDebounce(void) {
    static uint16_t Btn1_States = 0;
    /* palReadPad visszaadja 0 vagy 1: 1 = pad logikai magas */
    /* aktív HIGH feltételezve */
    Btn1_States = (uint16_t)((Btn1_States << 1) | (palReadPad(BUTTON1_PORT, BUTTON1_PAD)));
    return (Btn1_States == 0xFFF0);
}

/* A mintavételező thread */
static THD_WORKING_AREA(waButtonThread, 128);
static THD_FUNCTION(ButtonThread, arg) {
    (void)arg;
    chRegSetThreadName("btn");

    while (true) {
        chThdSleepMilliseconds(TimeInterval);
        if (BtnDebounce()) {
            palTogglePad(LED1_PORT, LED1_PAD);
            /* várjunk, hogy egyszeri esemény legyen (opcionális) */
            /* chThdSleepMilliseconds(50); */
        }
    }
}

/* Main */
int main(void) {
    halInit();
    chSysInit();

    palClearPad(LED1_PORT, LED1_PAD); /* LED ki alapból */

    /* Indítjuk a gomb mintavételező thread-et */
    chThdCreateStatic(waButtonThread, sizeof(waButtonThread), NORMALPRIO, ButtonThread, NULL);

    /* A main thread üresen fut — itt lehet más feladat */
    while (true) {
        chThdSleepMilliseconds(1000);
    }

    /* soha nem érjük el */
    return 0;
}
