#pragma src
#include "defines.h"
#include <stdio.h>
#include "functions.c"

/*   PORTS AND PINS  

PORT B PIN 6   TIMER 0 PWM for H sync
PORT B PIN 0 	 TIMER 2 PWM for V sync

PORT A PIN 0   DIGITAL OUT RED BIT 0
PORT A PIN 1   DIGITAL OUT RED BIT 1
PORT A PIN 2   DIGITAL OUT RED BIT 2

PORT A PIN 3   DIGITAL OUT GREEN BIT 0
PORT A PIN 4   DIGITAL OUT GREEN BIT 1
PORT A PIN 5   DIGITAL OUT GREEN BIT 2

PORT A PIN 6   DIGITAL OUT BLUE BIT 0
PORT A PIN 7   DIGITAL OUT BLUE BIT 1
*/


/********************* MAIN ****************************/
int main(void)
{	
	int i;
	setup_GPIO_clock(A);
	setup_GPIO_clock(B);
	
	setup_GPIO_PWM_B(6);
	setup_GPIO_PWM_B(0);
	
	for (i=0; i<8; i++){
		setup_GPIO_RGB_A(i);
	}
	
	setup_PLL();
	
	setup_PWM_Hsync();
	
	setup_PWM_Vsync();
	
	// Enable syncs 
	TIMER0_CTL &= ~0x1;
	TIMER2_CTL &= ~0x1;
// spin 
while(1) ;
}


