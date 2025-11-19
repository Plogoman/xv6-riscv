#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
	int count = kbdint();
	printf("Keyboard Interrupts since boot: %d\n", count);
	exit(0);
}
