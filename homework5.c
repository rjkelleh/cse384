#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	bool opt_h = false, opt_d = false;
	int opt = getopt(argc, argv, "hd");
	while (opt != -1) //GetOpts
	{
		if (opt == 'h') {
			opt_h = true;
		} else if (opt == 'd') {
			opt_d = true;
		}
		opt = getopt(argc, argv, "hd");
	}

	
	if (opt_h) //Print Help Information
	{
		printf("*Display Help Information*\n");
	}

	if (opt_d) //Customize Backup Location
	{
		printf("*Customize Backup Location*\n");
	}

	if (argc < 2) //Print Usage Information
	{
		printf("Usage: %s EXECUTABLE\n",argv[0]);
	}

	return EXIT_SUCCESS;
}