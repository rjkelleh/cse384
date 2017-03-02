#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/inotify.h>

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
///////////////////////////////////////////////////////
	int fd = inotify_init();
	int wd, x;
	wd = inotify_add_watch(fd, argv[1], IN_OPEN | IN_ACCESS | IN_MODIFY);

	char buf[1000];
	char *p = NULL;


	while(1) {
		x = read(fd, buf, 1000);

		if(x == -1) {
			printf("ERROR!");
			return EXIT_FAILURE;
		}
		for(p = buf; p < (buf+x);) {
			struct inotify_event* event = (struct inotify_event*)p;


			if((event -> mask&IN_OPEN) == IN_OPEN){
				printf("file opened\n");
			}
			if((event -> mask&IN_MODIFY) == IN_MODIFY){
				printf("file modified\n");
			}



			p += sizeof(struct inotify_event) + event->len;
		}
	}

	return EXIT_SUCCESS;
}