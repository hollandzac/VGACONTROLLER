
#include "defines.h"
#include "globals.h"




// Enable GPIO clocks
void setup_GPIO_clock(int port){
	
	SYSCTL_RCGCGPIO |= (1 << port);
	
	while ((SYSCTL_PRGPIO & (1 << port)) != (1 << port)){
		__asm("NOP");
	}
}

// Setup GPIO for PWM on port B
void setup_GPIO_TIMER_portB(int pin){
	
	GPIO_B_DIR |= (1 << pin);
	
	GPIO_B_AFSEL |= (1 << pin);
	
	GPIO_B_DEN |= (1 << pin);
	
	// set pin for TIMER MODE 0x4
	GPIO_B_PCTL |= (0x7 << (pin*4));
	
}

/// Setup GPIO pins on Port A for digital output
void setup_GPIO_RGB_A(unsigned long pin){
	
	GPIO_A_DIR |= (1 << pin);
	
	GPIO_A_AFSEL &= ~(1 << pin);
	
	GPIO_A_DEN |= (1 << pin);
	
	GPIO_A_PCTL &= ~(0xF << (pin*4));
	
}
// Setup the PLL for higher clock speed

void setup_usrswitch (int pin){
	GPIO_F_DIR &= ~(1 << pin);
	
	GPIO_F_AFSEL &= ~(1 << pin);
	
	GPIO_F_DEN |= (1 << pin);
	
	GPIO_F_PCTL &= ~(0xF << (pin*4));
	
	
}


void setup_PLL(void){
	
	// Set crystal value bits 10:6 for required clock speed 0x15 for max crystal of 16MHz
	SYSCTL_RCC |= (0x15 << 6);
	
	//Set use sysdivbit
	SYSCTL_RCC |= (1 << 22);
	
	
	//Config for adavanced systems
	SYSCTL_RCC2 |= 0x80000000;
	
	// Bypass PLL for init
	SYSCTL_RCC2 |= (1 << 11);
	
	// Set oscillator source as main oscillator bits 6:4
	SYSCTL_RCC2 &= ~(0x7 << 4);
	
	// Activate PLL and select 400MHz PLL
	SYSCTL_RCC2 &= ~(1 << 13);
	
	//clear DIV400
	SYSCTL_RCC2 |= (1 << 30);
	
	///Clear sysdiv lsb
	SYSCTL_RCC2 &= ~(1 << 22);
	
	// Set system divisor required speed FOR NOW IS 80MHz
	SYSCTL_RCC2 &= ~(0x3F << 23);
	
	// Set system divisor required speed FOR NOW IS 80MHz
	SYSCTL_RCC2 |= (0x2 << 23);

	// Wait for PLL to Lock
	while ((SYSCTL_RIS & (1 << 6)) != (1 << 6)){
		__asm("NOP");
	}
	
	// Enable PLL
	SYSCTL_RCC2 &= ~(1 << 11);
	
	
}
	

/* Setup the PWM for H-SYNC with postive sync pulses

PERIOD IS 800 PIXELS 31.7775 micro seconds
LOAD VALUE 2542

SYNC PULSE IS 96 PIXELS 3.81333 micro seconds
MATCH VALUE 2237 */

void setup_PWM_Hsync(void){
	//Enable clock for pwm
	SYSCTL_RCGTIMER |= 0x1;
	
	//Wait for clock to stabalize
	while ((SYSCTL_PRTIMER & (0x1)) != (0x1)){
		__asm("NOP");;
	}
	
	// Disable timer A
	TIMER0_CTL &= ~0x1;
	
	// Select mode 16 bit
	TIMER0_CFG &= ~0x7;
	TIMER0_CFG |= 0x4;
	
	// Set PWM mode
	TIMER0_TAMR |= (1 << 3);
	TIMER0_TAMR &= ~(1 << 2);
	TIMER0_TAMR |= 0x2;
	
	// Set output state
	TIMER0_CTL &= ~(1 << 6);

	//No prescaler
	
	// Set Preload value for timer A h-sync
	TIMER0_TAILR &= 0;
	TIMER0_TAILR |= 0x9EE;
	
	// Set match value
	TIMER0_MATCH &= 0;
	TIMER0_MATCH |= 0x8BD;
	
	//setup interrupt for falling edge triggered
	TIMER0_CTL |= (1 << 2);
	TIMER0_TAMR |= (1 << 9);
	
	//enable nvic interupt IRQ 19
	NVIC_ST_EN0 |= (1 << 23);
	
	//set priority
	
	// Enable timer A DISABLED FOR TIMING
	//TIMER0_CTL |= 0x1;
	
}

/* Setup the PWM for V-SYNC with postive sync pulses

PERIOD IS 525 LINES 16.683217 milli seconds
LOAD VALUE 1,334,657

SYNC PULSE IS 2 LINES 63.555 micro seconds
MATCH VALUE 1,329,573 */

void setup_PWM_Vsync(void){
	//Enable clock for pwm
	SYSCTL_RCGTIMER |= (1 << 2);
	
	//Wait for clock to stabalize
	while ((SYSCTL_PRTIMER & (1 << 2)) != (1 << 2)){
		__asm("NOP");
	}
	
	// Disable timer A
	TIMER2_CTL &= ~0x1;
	
	// Select mode 16 bit
	TIMER2_CFG &= ~0x7;
	TIMER2_CFG |= 0x4;
	
	// Set PWM mode
	TIMER2_TAMR |= (1 << 3);
	TIMER2_TAMR &= ~(1 << 2);
	TIMER2_TAMR |= 0x2;
	
	// Set output state
	TIMER2_CTL &= ~(1 << 6);

	//8-bit prescaler
	TIMER2_TAPR &= 0;
	TIMER2_TAPR |= 0x14;
	
	// Set Preload value for timer A h-sync
	TIMER2_TAILR &= 0;
	TIMER2_TAILR |= 0x5D81;
	
	//8bit match prescaler
	TIMER2_MATCHPR &= 0;
	TIMER2_MATCHPR |= 0x14;
	
	// Set match value
	TIMER2_MATCH &= 0;
	TIMER2_MATCH |= 0x49A5 ;
	
	//setup interrupt for falling edge triggered
	TIMER2_CTL |= (1 << 2);
	
	//enable interrupt
	TIMER2_TAMR |= (1 << 9);
	
	//enable nvic interupt IRQ 23
	NVIC_ST_EN0 |= (1 << 23);
	
	//set priority
	//NVIC_ST_PRI0 |= (1
	
	// Enable timer A DISABLE FOR TIMING
	//TIMER2_CTL &= ~0x1;
	
}


	
	
	
void objects_ISR(void){

	int x,y;
	unsigned long byteoff,bitnum;
	//Ackowledge interrupt
	TIMER2_ICR |= (1 << 2);
	
	//Setup timer interupt for end of vertical back porch enable h-syncs
	// Disable timer A
	TIMER3_CTL &= ~0x1;
	
	// Select mode 16 bit
	TIMER3_CFG &= ~0x7;
	TIMER3_CFG |= 0x4;
	
	// Set oneshot mode
	TIMER3_TAMR |= 1;

	//No prescaler
	
	// Set Preload value end of back porch
	TIMER3_TAILR &= 0;
	TIMER3_TAILR |= 83822;

	// Enable timer
	TIMER3_CTL |= 0x1;
	
	//defined for a 2*2 square *ball object black all pixels
	for (x=0; x<2; x++){
		for (y=0; y<2; y++){
			byteoff = ((*ball.x + x)*(*ball.y + y))/4; // this is the byte offset in the bitband address 
			bitnum = ((*ball.x + x)*(*ball.y + y))%4; // the bitnumber within the byte
			//change green bit
			(*(unsigned long *)(PIXEL_BITBAND_ALIAS_BASE + (byteoff * 32) + (bitnum*4))) = 0;
			//change red bit
			(*(unsigned long *)(PIXEL_BITBAND_ALIAS_BASE + (byteoff*32) + (bitnum*4)+1)) = 0;
		}
	}
	
			
	//Handles if the *ball hits a wall
	if (*ball.x == 318) {
		if (*ball.direction == 1) { *ball.direction = 7; }
		else if (*ball.direction == 3) { *ball.direction = 5; }
	}
	if (*ball.x == 2) {
		if (*ball.direction == 7) { *ball.direction = 1;}
		else if (*ball.direction == 5) { *ball.direction = 3;}
	}
	if (*ball.y == 238) {
		if (*ball.direction == 0){ 
			if (*ball.x<=160) { *ball.direction = 3; }
			else {*ball.direction = 5;}
		}
		else if (*ball.direction == 1){ *ball.direction = 3;}
		else {*ball.direction = 5;}
	}
	if (*ball.y == 2) {
		if (*ball.direction == 3){ 
			if (*ball.x<=60 || (*ball.x>120 && *ball.x<=180) || (*ball.x>240 && *ball.x<=380)){ *ball.direction = 0; }
			else {*ball.direction = 0;}
		}
		else if (*ball.direction == 5){
			if (*ball.x<=60 || (*ball.x>120 && *ball.x<=180) || (*ball.x>240 && *ball.x<=380)){ *ball.direction = 0; }
			else {*ball.direction = 5;}
		}
	}
	
	//increments *balls position in regards to direction
	switch (*ball.direction)
	{
		case 1:
			++*ball.x;
			++*ball.y;
		case 3:
			++*ball.x;
			--*ball.y;
		case 0:
			++*ball.y;
		case 5:
			--*ball.x;
			--*ball.y;
		case 7:
			--*ball.x;
			++*ball.y;
	}
	
	//write new *ball postition
	for (x=0; x<2; x++){
		for (y=0; y<2; y++){
			byteoff = ((*ball.x + x)*(*ball.y + y))/4; // this is the byte offset in the bitband address 
			bitnum = ((*ball.x + x)*(*ball.y + y))%4; // the bitnumber within the byte
			//change red bit
			(*(unsigned long *)(PIXEL_BITBAND_ALIAS_BASE + (byteoff*32) + (bitnum*4)+1)) = 1;
		}
	}	
}


//setup systick for interupt at the end of active video
void setup_systick(void){
	//setup systick
	NVIC_ST_CTRL &= ~1;
	
	//Set reload value length of screen 16.68322ms 1,334,657 clock cycles
	NVIC_ST_RELOAD |= 1334657;
	
	//Clear counter
	NVIC_ST_CURRENT |= 1;
	
	//Set clock source and enable interupts
	NVIC_ST_CTRL |= (3 << 1);
	
	//enable systick in main
	//NVIC_ST_CTRL |= 1;
}
	
void pixel_ISR(void){
	int i;
	//Ackowledge interrupt
	TIMER0_ICR |= (1 << 2);
	//Setup timer interupt for end of vertical back porch enable h-syncs
	// Disable timer A
	TIMER3_CTL &= ~0x1;
	
	// Select mode 16 bit
	TIMER3_CFG &= ~0x7;
	TIMER3_CFG |= 0x4;
	
	// Set oneshot mode
	TIMER3_TAMR |= 1;

	//No prescaler
	
	// Set Preload value end of horizontal back porch 152 - instruction time
	TIMER3_TAILR &= 0;
	TIMER3_TAILR |= 113;
	
	// Enable timer
	TIMER3_CTL |= 0x1;
	
	
	//wait for timer start of video
	while ((TIMER3_RIS & 1) != 1){ }
	//clear interupt bit 
	TIMER3_ICR |= 1;
	
	//start draw
	for (i=0; i<20; i++){
		GPIO_A_DATAMASK_PIN_BOTH = *bitband;
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 2);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 4);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 6);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 8);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 10);
		__asm("NOP");
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 12);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 14);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 16);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 18);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 20);
		__asm("NOP");
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 22);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 24);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 26);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 28);
		GPIO_A_DATAMASK_PIN_BOTH = (*bitband >> 30);
	}
	
	count++;
}

				
		
	
	
	
	
	
