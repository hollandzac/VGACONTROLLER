struct Object {
	unsigned long *x;
	unsigned long *y;
	unsigned long *direction;
};

extern volatile struct Object ball;
extern volatile struct Object paddle;
extern volatile unsigned long * bitband;
extern volatile unsigned long * count;

