#ifdef CHANGED
#include "syscall.h"
int
main ()
{
	int i = 0;
	while (i < 10){
		SynchPutString("HEllo\n");
		i++;
	}
	
	
	Halt();

    return 0;
}
#endif // CHANGED
