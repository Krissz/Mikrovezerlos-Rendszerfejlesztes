#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell_manager.h"

#define SHORTDELAY 200

int morseABC[36][5] = {
                      {1, 3, 0, 0, 0},      //A
                      {3, 1, 1, 1, 0},      //B
                      {3, 1, 3, 1, 0},      //C
                      {3, 1, 1, 0, 0},      //D
                      {1, 0, 0, 0, 0},      //E
                      {1, 1, 3, 1, 0},      //F
                      {3, 3, 1, 0, 0},      //G
                      {1, 1, 1, 1, 0},      //H
                      {1, 1, 0, 0, 0},      //I
                      {1, 3, 3, 3, 0},      //J
                      {3, 1, 3, 0, 0},      //K
                      {1, 3, 1, 1, 0},      //L
                      {3, 3, 0, 0, 0},      //M
                      {3, 1, 0, 0, 0},      //N
                      {3, 3, 3, 0, 0},      //O
                      {1, 3, 3, 1, 0},      //P
                      {3, 3, 1, 3, 0},      //Q
                      {1, 3, 1, 0, 0},      //R
                      {1, 1, 1, 0, 0},      //S
                      {3, 0, 0, 0, 0},      //T
                      {1, 1, 3, 0, 0},      //U
                      {1, 1, 1, 3, 0},      //V
                      {1, 3, 3, 0, 0},      //W
                      {3, 1, 1, 3, 0},      //X
                      {3, 1, 3, 3, 0},      //Y
                      {3, 3, 1, 1, 0},      //Z
                      {3, 3, 3, 3, 3},      //0
                      {1, 3, 3, 3, 3},      //1
                      {1, 1, 3, 3, 3},      //2
                      {1, 1, 1, 3, 3},      //3
                      {1, 1, 1, 1, 3},      //4
                      {1, 1, 1, 1, 1},      //5
                      {3, 1, 1, 1, 1},      //6
                      {3, 3, 1, 1, 1},      //7
                      {3, 3, 3, 1, 1},      //8
                      {3, 3, 3, 3, 1}       //9

};


void morseWait(int wait)
{
  chThdSleepMilliseconds(SHORTDELAY * wait);
}

void morseBlink(int morse)
{
  palSetPad(GPIOD, GPIOD_LED3);
  palSetPad(GPIOD, GPIOD_LED4);
  palSetPad(GPIOD, GPIOD_LED5);
  palSetPad(GPIOD, GPIOD_LED6);
  morseWait(morse);

  palClearPad(GPIOD, GPIOD_LED3);
  palClearPad(GPIOD, GPIOD_LED4);
  palClearPad(GPIOD, GPIOD_LED5);
  palClearPad(GPIOD, GPIOD_LED6);
  morseWait(1);
}


//morse sos
void cmd_Morse(BaseSequentialStream *chp, int argc, char *argv[])
{
  //Betűkön megy végig
  for(int i=0; i<strlen(argv[0]); i++)
  {
    char c = argv[0][i];
    chprintf(chp, "Betu: %c\r\n", c);

    int index = -1;

    if(c >= 'A' && c <= 'Z')
    {
      index = c - ((int)'A');
    }
    else if(c >= 'a' && c <= 'z')
    {
      index = c - ((int)'a');
    }
    else if(c >= '0' && c <= '9')
    {
      int num = c - ((int)'0');     //Számmá konvertálás pl: '2' ->  50 - 48 = 2
      index = 26 + num;         //0 indexe + szám: 26 + 2 = 28
    }

    //Ha a karakter szóköz
    if(c == ' ')
    {
      morseWait(7);
    }
    //Egyéb érvényes karakter
    else if(index != -1)
    {
      //betű jelein megy végig
      int j = 0;
      while(j < 5)
      {
        int sign = morseABC[index][j];
        if(sign == 0) break;
        morseBlink(sign);
        j++;
      }
    }
    //Karakter szünet
    morseWait(3);  //Three unit

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
