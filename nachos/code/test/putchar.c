#include "syscall.h"

#ifdef CHANGED

void print(char c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		PutChar(c+i);
	}

	PutChar('\n');
}
int
main()
{
	print('a',26);
	Halt();
}
//test
#endif // CHANGED
