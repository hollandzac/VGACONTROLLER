#include "defines.h"
#include "globals.h"



// Enable GPIO clocks
void setup_GPIO_clock(int port){
	
	SYSCTL_RCGCGPIO |= (1 << port);
	
	while ((SYSCTL_PRGPIO & (1 << port)) != (1 << port)){
		__asm{ NOP }
	}
}

// Setup GPIO for PWM on port B
void setup_GPIO_TIMER_B(int pin){
	
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
		__asm{ NOP }
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
		__asm{ NOP }
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
		__asm{ NOP }
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
	
	// Enable timer A DISABLE FOR TIMING
	//TIMER2_CTL &= ~0x1;
	
}


	
	
	
void __irq Objects_ISR(void){
	
	//setup systick
	NVIC_ST_CTRL &= ~1;
	
	//Set reload value length of active video + vertical back porch 16.301ms 1,304.080 clock cycles
	NVIC_ST_RELOAD |= 1304080;
	
	//Clear counter
	NVIC_ST_CURRENT |= 1;
	
	//Set clock source and enable interupts
	NVIC_ST_CTRL |= (3 << 1);
	
	//enable systick
	NVIC_ST_CTRL |= 1;
	
	int x,y,byteoff,bitnum;
	
	//defined for a 2*2 square ball object black all pixels
	for (x=0; x<2; x++){
		for (y=0; y<2; y++){
			byteoff = ((ball.x + x)*(ball.y + y))/4; // this is the byte offset in the bitband address 
			bitnum = ((ball.x + x)*(ball.y + y))%4; // the bitnumber within the byte
			//change green bit
			(*(unsigned long *)(BITBAND_ALIAS_BASE + (byteoff * 32) + (((ball.x*ball.y)%4)*4))) = 0;
			//change red bit
			(*(unsigned long *)(BITBAND_ALIAS_BASE + (byteoff*32) + (bitnum*4)+1)) = 0;
		}
	}
	
			
	//Handles if the ball hits a wall
	if (ball.x == 318) {
		if (ball.direction == 1) { ball.direction = 7; }
		else if (ball.direction == 3) { ball.direction = 5; }
	}
	if (ball.x == 2) {
		if (ball.direction == 7) { ball.direction = 1;}
		else if (ball.direction == 5) { ball.direction = 3;}
	}
	if (ball.y == 238) {
		if (ball.direction == 0){ 
			if (ball.x<=160) { ball.direction = 3; }
			else {ball.direction = 5;}
		}
		else if (ball.direction == 1){ ball.direction = 3;}
		else {ball.direction = 5;}
	}
	if (ball.y == 2) {
		if (ball.direction == 3){ 
			if (ball.x<=60 || (ball.x>120 && ball.x<=180) || (ball.x>240 && ball.x<=380)){ ball.direction = 0; }
			else {ball.direction = 0;}
		}
		else if (ball.direction == 5){
			if (ball.x<=60 || (ball.x>120 && ball.x<=180) || (ball.x>240 && ball.x<=380)){ ball.direction = 0; }
			else {ball.direction = 5;}
		}
	}
	
	//increments balls position in regards to direction
	switch (ball.direction)
	{
		case 1:
			ball.x++;
			ball.y--;
		case 3:
			ball.x++;
			ball.y--;
		case 0:
			ball.x++;
		case 5:
			ball.x--;
			ball.y++;
		case 7:
			ball.x--;
			ball.y++;
	}
	
	//write new ball postition
	for (x=0; x<2; x++){
		for (y=0; y<2; y++){
			byteoff = ((ball.x + x)*(ball.y + y))/4; // this is the byte offset in the bitband address 
			bitnum = ((ball.x + x)*(ball.y + y))%4; // the bitnumber within the byte
			//change red bit
			(*(unsigned long *)(BITBAND_ALIAS_BASE + (byteoff*32) + (bitnum*4)+1)) = 1;
		}
	}	
}

void disable_HSYNC_interupts_ISR(void){
	TIMER0_TAMR &= ~(1 << 9);
}

	
void pixel_ISR(void){
	GPIO_A_BASE |= *bitband;
	bitband++;
	GPIO_B_BASE |= *bitband;
	bitband++;
}

				
		
	
	
	
	
	
