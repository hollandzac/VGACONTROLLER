
extern void objects_ISR(void);
extern void pixel_ISR(void);

extern void setup_GPIO_clock(int port);
extern void setup_GPIO_TIMER_portB(int pin);
extern void setup_GPIO_RGB_A(int pin);
extern void setup_PLL(void);
extern void setup_PWM_Hsync(void);
extern void setup_PWM_Vsync(void);
extern void disable_HSYNC_interupts_ISR(void);
extern void setup_systick(void);
extern void setup_usrswitch(int pin);
