/*Defines system control registers*/

#define SYSCTL_RCGCGPIO (*((unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO 	(*((unsigned long *)0x400FEA08))
#define SYSCTL_RCC 			(*((unsigned long *)0x400FE060))
#define SYSCTL_RCC2 		(*((unsigned long *)0x400FE070))
#define SYSCTL_RIS 			(*((unsigned long *)0x400FE050))
#define SYSCTL_RCGTIMER (*((unsigned long *)0x400FE604))
#define SYSCTL_PRTIMER	(*((unsigned long *)0x400FEA04))
#define SYSCTL_RCGC0	  (*((unsigned long *)0x400FE100))
	
/* Defines for timer 0 */
#define TIMER0_CTL      (*((unsigned long *)0x4003000C))
#define TIMER0_CFG			(*((unsigned long *)0x40030000))
#define TIMER0_TAMR			(*((unsigned long *)0x40030004))
#define TIMER0_MATCH		(*((unsigned long *)0x40030030))
#define TIMER0_TAILR		(*((unsigned long *)0x40030028))

/* Defines for timer 2 */
#define TIMER2_CTL      (*((unsigned long *)0x4003200C))
#define TIMER2_CFG			(*((unsigned long *)0x40032000))
#define TIMER2_TAMR			(*((unsigned long *)0x40032004))
#define TIMER2_MATCH		(*((unsigned long *)0x40032030))
#define TIMER2_TAILR		(*((unsigned long *)0x40032028))
#define TIMER2_TAPR			(*((unsigned long *)0x40032038))
#define TIMER2_MATCHPR	(*((unsigned long *)0x40032040))


	
/* Defines for PWM module 0 */
#define PWM0_CTL			  (*((unsigned long *)0x40028040))


	
/* Defines for GPIO ports A */

#define GPIO_A_BASE			(*((unsigned long *)0x40004000))
#define GPIO_A_DIR			(*((unsigned long *)0x40004400))
#define GPIO_A_AFSEL		(*((unsigned long *)0x40004420))
#define GPIO_A_DEN			(*((unsigned long *)0x4000451C))
#define GPIO_A_PCTL			(*((unsigned long *)0x4000452C))
#define GPIO_A_AMSEL		(*((unsigned long *)0x40004528))

/* Defines for GPIO ports B */

#define GPIO_B_BASE			(*((unsigned long *)0x40005000))
#define GPIO_B_DIR			(*((unsigned long *)0x40005400))
#define GPIO_B_AFSEL		(*((unsigned long *)0x40005420))
#define GPIO_B_DEN			(*((unsigned long *)0x4000551C))
#define GPIO_B_PCTL			(*((unsigned long *)0x4000552C))
#define GPIO_B_AMSEL		(*((unsigned long *)0x40005528))

/* Define ports as numbers for ease of use*/
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5

//Bit Band SRAM Base
#define BITBAND_BASE 			 ((unsigned long) 0x22000000)
#define BITBAND_ALIAS_BASE ((unsigned long) 0x22000000)
	
//defines for NVIC
#define NVIC_ST_CTRL			(*((unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD		(*((unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT		(*((unsigned long *)0xE000E018))
;



	