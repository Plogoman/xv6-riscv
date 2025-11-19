#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
	if (argc == 2 && strcmp(argv[1], "?") == 0) {
		printf("Usage: fact number\n");
		exit(0);
	}

	if (argc != 2) {
		printf("You can only get the factorial of a single number!\n");
    	exit(0);
	}

	int num = atoi(argv[1]);
	unsigned long long fact = 1;

	if (num < 1) {
		printf("[Error] No factorial for negative numbers!\n");
		exit(0);
	} else {
		for (int i = 1; i <= num; i++)
		{
			fact *= i;
		}
		printf("The factorial of %d is %lld\n", num, fact);
	}

	exit(0);
}
