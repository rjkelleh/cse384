#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/inotify.h>
//
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>

int cp(const char* to, const char* from);

int main(int argc, char *argv[]) {
  bool opt_h = false, opt_d = false, opt_m = false, opt_t = false;
  int opt = getopt(argc, argv, "hdmt");
  int location = 0;
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
    location = 1;
  }

  if (opt_m) //Disable Meta-Deta Duplication
  {
    printf("*Disable Meta-Deta Duplication*\n");
  }

  if (opt_t) //Append Duplication to File Name
  {
    printf("*Append Duplication to File Name*\n");
  }

  if (argc < 2) //Print Usage Information
  {
    printf("Usage: %s EXECUTABLE\n",argv[0]);
    return EXIT_SUCCESS;
  }

  //if (argc > 4) //Print Usage Information
  //{
  //  printf("Error: Too many arguments\n");
  //  return EXIT_FAILURE;
 // }
///////////////////////////////////////////////////////
  int fd = inotify_init();
  int wd, x;
  wd = inotify_add_watch(fd, argv[1], IN_OPEN | IN_ACCESS | IN_MODIFY);

  char buf[1000];
  char *p = NULL;


  char *name;
  struct passwd *pass = getpwuid(getuid());
  name = pass->pw_name;
  //printf("Username: %s", name);

  char a[1000] = "/home/";
  strcat(a,name);
  char b[1000] = "/Documents/test";
  strcat(a,b);
  

  if (location == 1) {
    strcpy(a, argv[3]);
  }

  printf("%s\n",a);

  char *path = realpath(argv[1], NULL);

  printf("%s\n",path);

  cp(a,path);


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
      cp(a,path);
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