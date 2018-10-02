struct Object {
	int x;
	int y;
	int direction;
};

extern volatile struct Object ball;
extern volatile struct Object paddle;
extern volatile unsigned long * bitband;
extern volatile int count;

