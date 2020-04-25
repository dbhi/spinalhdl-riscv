#include <stdint.h>
#include <murax.h>


void main() {
  interruptCtrl_init(TIMER_INTERRUPT);
  prescaler_init(TIMER_PRESCALER);
  timer_init(TIMER_A);

  TIMER_PRESCALER->LIMIT = 12000-1; //1 ms rate

  TIMER_A->LIMIT = 10-1;  //1 second rate
  TIMER_A->CLEARS_TICKS = 0x00010002;

  TIMER_INTERRUPT->PENDINGS = 0xF;
  TIMER_INTERRUPT->MASKS = 0x1;

  GPIO_A->OUTPUT_ENABLE = 0x000000FF;
  GPIO_A->OUTPUT = 0x00000000;

  UART->STATUS = 2; //Enable RX interrupts
  UART->DATA = 'A';

  while(1){
    while(AXIS->IN_VALID==0) { asm volatile(""); }
    AXIS->OUT_DATA = 3 * AXIS->IN_DATA;
    AXIS->OUT_VALID = 0xFFFF;
    while(AXIS->OUT_VALID!=0) { asm volatile(""); }
    AXIS->IN_READY = 0xFFFF;
    while(AXIS->IN_READY!=0) { asm volatile(""); }
  }

}

void irqCallback(){
  if(TIMER_INTERRUPT->PENDINGS & 1){  //Timer A interrupt
    GPIO_A->OUTPUT ^= 0x80; //Toogle led 7
    TIMER_INTERRUPT->PENDINGS = 1;
  }
  while(UART->STATUS & (1 << 9)){ //UART RX interrupt
    UART->DATA = (UART->DATA) & 0xFF;
  }
}
