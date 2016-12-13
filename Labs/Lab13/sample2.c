/* Sample Program 2 - Lab 13
 * By: Hayden Miedema and Sage Heiss
 */

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h> 

#define SIZE 30

int main (int argc, char *argv[]) {
    struct flock fileLock;
    int fd;
    char buf[SIZE] = "// is this Sample Program 1?";

    if (argc < 2) {
        printf ("usage: filename\n");
        exit (1);
    }
    if ((fd = open (argv[1], O_RDWR)) < 0) {
        perror ("there is");
        exit (1);
    }

    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;

    while(fcntl(fd, F_SETLKW, &fileLock));
   
    read(fd, buf, SIZE);
    printf("First line: %s\n", buf);

    
    return 0;
}