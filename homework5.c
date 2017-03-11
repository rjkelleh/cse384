#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#define birthtime(x) x.st_ctime

int cp(const char *to, const char *from);

int main(int argc, char *argv[]) {
    bool opt_h = false, opt_d = false, opt_m = false, opt_t = false;
    int opt = getopt(argc, argv, "hdmt");
    while (opt != -1) //GetOpts
    {
        if (opt == 'h')
	{
            opt_h = true;
        }
	else if (opt == 'd')
	{
            opt_d = true;
        }
	else if (opt == 'm')
	{
	    opt_m = true;
	}
	else if (opt == 't')
	{
	    opt_t = true;
	}
        opt = getopt(argc, argv, "hdmt");
    }

    
    if (opt_h) //Print Help Information
    {
        printf("$Usage\n");
    }

    if (opt_d) //Customize Backup Location
    {
        printf("*Customize Backup Location*\n");
    }

    if (opt_m) //Disable Meta-Data Duplication
    {
	FILE *p1, *p2;
	char x;

	p1 = fopen("argv[1]", "r");
	if (p1 == NULL)
	{
	    printf("ERROR! File cannot be opened!");
	    exit(1);
	}

	p2 = fopen("argv[1]_copy", "w");
	if (p2 == NULL)
	{
	    printf("ERROR! File cannot be opened!");
	    exit(1);
	}

	do
	{
	    x = fgetc(p1);
	    fputc(x, p2);
	} while (x != EOF);
	
	fclose(p1);
	fclose(p2);
    }

    if (opt_t) //Append Duplication Time
    {
	struct stat st;
	
	if(stat(argv[1], &st) != 0)
	{
	    perror(argv[1]);
	}
	
	("argv[1]_%i\n", birthtime(st));

	exit(1);
    }

    if (argc < 2) //Print Usage Information
    {
        printf("Usage: %s EXECUTABLE\n",argv[0]);
    }


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
                cp("/home/ryan/Documents/test","/home/ryan/cse384/test");
            }



            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return EXIT_SUCCESS;
}

int cp(const char* to, const char* from) {

    int to_value, from_value;
    char buf[4096];
    ssize_t size;
    int saved_value;

    from_value = open(from, O_RDONLY);
    if (from_value < 0)
        return -1;

    to_value = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (to_value < 0)
        goto output;

    while (size = read(from_value, buf, sizeof buf), size > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(to_value, out_ptr, size);

            if (nwritten >= 0)
            {
                size -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto output;
            }
        } while (size > 0);
    }

    if (size == 0)
    {
        if (close(to_value) < 0)
        {
            to_value = -1;
            goto output;
        }
        close(from_value);

        return EXIT_SUCCESS;
    }

  output:
    saved_value = errno;

    close(from_value);
    if (to_value >= 0)
        close(from_value);

    errno = saved_value;
    return -1;
}

