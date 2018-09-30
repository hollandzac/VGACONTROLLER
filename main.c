
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
volatile unsigned long * bitband;




int main(void)
{	
	//Array of each of pixel bitbands address
	//const unsigned long *screen[320][240];
	
	unsigned long i;
	bitband = (unsigned long *)BITBAND_ALIAS_BASE;
	

	
	//int x,y,j;
	/*These for loops apply a bit banded alias address to each pixel
	for (x=0; x<160; x++){
		for (y=0; y<240; y++){
			screen[x][y] = (unsigned long *)(BITBAND_ALIAS_BASE + (((x*y)/4)*32) + (((x*y)%4)*4));
		}
	}
	for (x=160; x<320; x++){
		for (y=0; y<240; y++){
			screen[x][y] = (unsigned long *)(BITBAND_ALIAS_BASE + ((((x*y)/4)+9600)*32) + (((x*y)%4)*4));
		}
	*/
	
	//set initial ball position
	ball.x = 160;
	ball.y = 120;
	ball.direction = 4;
	
		
	setup_GPIO_clock(A);
	setup_GPIO_clock(B);
	
	
	setup_GPIO_TIMER_portB(6);
	setup_GPIO_TIMER_portB(0);
	setup_GPIO_TIMER_portB(1);
	
	for (i=0; i<8; i++){
		setup_GPIO_RGB_A(i);
	}
	
	setup_PLL();
	
	setup_PWM_Hsync();
	
	setup_PWM_Vsync();
	
	// Enable syncs 
	TIMER0_CTL |= 0x1;
	TIMER2_CTL |= 0x1;
	
	// spin 
	while(1){
		//poll systick counter for value to enable hsync interupts end of vertical back porch 1.048mms
		if (NVIC_ST_CURRENT <= 1220240){ TIMER0_TAMR |= (1 << 9);}
	}
}


