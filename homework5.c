#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int cp(const char* to, const char* from);

int main(int argc, char *argv[]) {
  bool opt_h = false, opt_d = false, opt_m = false, opt_t = false;
  int opt = getopt(argc, argv, "hdmt");
  while (opt != -1) //GetOpts
  {
    if (opt == 'h') {
      opt_h = true;
    } else if (opt == 'd') {
      opt_d = true;
    } else if (opt == 'm') {
      opt_m = true;
    } else if (opt = 't') {
      opt_t = true;
    }
    opt = getopt(argc, argv, "hdmt");
  }

  
  if (opt_h) //Print Help Information
  {
    printf("HOMEWORK 5 HELP INFORMATION\n");
    printf("*This program will create a copy of a file*\n*Enter file to be copied as first agrument*\n\n");
    printf("OPTIONS-\n");
    printf("-h: Print Help Information\n");
    printf("-d: Customize Backup Location (Will set argument as location)\n");
    printf("-m: Toggle meta-deta duplication\n");
    printf("-t: Toggle duplication time to file name\n");
    printf("- : Print Usage Information\n");
    printf("Exiting...\n");
    return EXIT_SUCCESS;
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
	
	stat("argv[1]", &st);
	char clock = localtime(&(st.st_ctime));
    }


  if (argc < 2) //Print Usage Information
  {
    printf("Usage: %s EXECUTABLE\n",argv[0]);
    return EXIT_SUCCESS;
  }

  if (argc > 2) //Print Usage Information
  {
    printf("Error: Too many arguments\n");
    return EXIT_FAILURE;
  }

///////////////////////////////////////////////////////
  int fd = inotify_init();
  int wd, x;
  wd = inotify_add_watch(fd, argv[1], IN_OPEN | IN_ACCESS | IN_MODIFY);

  char buf[1000];
  char *p = NULL;


  printf("Username: %d\n", getpwuid(getuid()));


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
        cp("/home/douglasporreca/Documents/test","/home/douglasporreca/.git/cse384/test");
      }

      p += sizeof(struct inotify_event) + event->len;

    }
  }

  return EXIT_SUCCESS;
}

int cp(const char* copy, const char* orig) {

    remove(copy);

    int fd_copy = open(copy, O_CREAT | O_WRONLY | O_EXCL);
    int fd_orig = open(orig, O_RDONLY);
    char buf[1000];
    char *p = buf;
    ssize_t read1;
    ssize_t write1;

    if (fd_copy == -1) {
      printf("Error: Can't open copy file\nExiting...");
      return EXIT_FAILURE;
    }

    if (fd_orig == -1) {
      printf("Error: Can't open original file\nExiting...");
      return EXIT_FAILURE;
    }



    while (read1 = read(fd_orig, buf, sizeof buf)) {
        p = buf;
        write1 = write(fd_copy, p, read1);

        if (read1 == -1) {
          printf("Error: Can not read Bytes\nExiting...");
          return EXIT_FAILURE;
        }

        if (write1 == -1) {
          printf("Error: Can not write Bytes\nExiting...");
          return EXIT_FAILURE;
        }

        read1 -= write1;
        p += write1;
  
    }

    close(fd_orig);
    close(fd_copy);

}
