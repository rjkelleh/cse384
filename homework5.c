#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	bool opt_h = false, opt_d = false;
	int opt = getopt(argc, argv, "hd");
	while (opt != -1)
	{
		if (opt == 'h') {
			opt_h = true;
		} else if (opt == 'd') {
			opt_d = true;
		}
		opt = getopt(argc, argv, "hd");
	}

	if (opt_h) {
		printf("*Display Help Information*\n");
	}
	if (opt_d) {
		printf("you selected option d\n");
	}

	return EXIT_SUCCESS;
}