
#include "defines.h"
#include "functions.h"
#include "globals.h"

/* 
RESOLUTION 320 X 420
PIXEL CLOCK 12.5MHz 80ns CLOCK
*/

/*   PORTS AND PINS  

PORT B PIN 6   TIMER 0 PWM for H sync
PORT B PIN 0 	 TIMER 2 PWM for V sync

PORT A PIN 0   DIGITAL OUT RED BIT 0	NOT USED
PORT A PIN 1   DIGITAL OUT RED BIT 1	NOT USED
PORT A PIN 2   DIGITAL OUT RED BIT 2

PORT A PIN 3   DIGITAL OUT GREEN BIT 0	NOT USED
PORT A PIN 4   DIGITAL OUT GREEN BIT 1	NOT USED
PORT A PIN 5   DIGITAL OUT GREEN BIT 2

PORT A PIN 6   DIGITAL OUT BLUE BIT 0	NOT USED
PORT A PIN 7   DIGITAL OUT BLUE BIT 1
*/


/********************* MAIN ****************************/

volatile struct Object ball;
volatile unsigned long * bitband = (unsigned long*)BITBAND_BASE;
volatile int count = 0;




int main(void)
{	
	//Array of each of pixel bitbands address
	//const unsigned long *screen[320][240];
	
	unsigned long i;
	//disbale interupts
	__asm("CPSID I");
	
	
	//set initial ball position
	ball.x = 160;
	ball.y = 120;
	ball.direction = 4;

	setup_GPIO_clock(A);
	setup_GPIO_clock(B);
	
	
	setup_GPIO_TIMER_portB(6);
	setup_GPIO_TIMER_portB(0);
	setup_GPIO_TIMER_portB(2);
	
	for (i=0; i<8; i++){
		setup_GPIO_RGB_A(i);
	}
	
	setup_PLL();
	
	setup_PWM_Hsync();
	
	setup_PWM_Vsync();
	
	//Enable clock for timer 3
	SYSCTL_RCGTIMER |= (1 << 3);
	
	//Wait for clock to stabalize
	while ((SYSCTL_PRTIMER & (1 << 3)) != (1 << 3)){
		__asm{ NOP }
	}
	


	// Enable syncs 
	TIMER0_CTL |= 0x1;
	TIMER2_CTL |= 0x1;
	
	//ENABLE INTERRUPTS
	__asm("CPSIE I");
	
	// spin 
	while(1){
		if ((TIMER3_RIS & 1) == 1){
			//clear interupt bit 
			TIMER3_ICR |= 1;
			//enable h-sync interuppts
			TIMER0_TAMR |= (1 << 9);
		}
		if (count >= 320){
			count = 0;
			//disable h-sync interuppts
			TIMER0_TAMR &= (1 << 9);
			//reset memory to base
			bitband = (unsigned long*)BITBAND_BASE;
		}
	}
}


