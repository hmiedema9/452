#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define READ 0
#define WRITE 1


void sigHandler (int);
pid_t pid;

int main () {
    int random1, random2;
    signal (SIGUSR1, sigHandler);
    signal (SIGUSR2, sigHandler);

    pid_t parent = getpid();
    
    if ((pid = fork ()) < 0) {
        perror ("fork failed\n");
        exit (1);
    }else if (!pid) {
      //child
      printf ("waiting...\n");
      while(1){
	random1 = (rand() % 7) + 1;
	random2 = rand() % 2;
	sleep(random1);
	if(random2 == 0){
	  kill(parent, SIGUSR1);
	  printf("What PID am I? %d\n", getpid());// send signal
	}else{
	  //kill(parent, SIGUSR2);
	  printf("What PID am I? %d\n", getpid());
	}
	// this is an infinite loop.
	// it should not exit unless ^C is hit.
      }
      printf("About to exit child.");
      exit(0);
     }else {
      // this is the parent
      signal (SIGINT, sigHandler);
      while(1){
	printf("Wieners");
	// wait for signals and let sigHandler handle them.
        pause();
	if(random2 != 0){
	  printf("What PID am I? %d\n", getpid());
	  kill(parent, SIGUSR2);
	}
      }
    }
    return 0;
}

void sigHandler (int sigNum) {
    if(sigNum == SIGUSR1){
      printf("received a SIGUSR1 signal\n");
    }else if(sigNum == SIGUSR2){
      printf("received a SIGUSR2 signal\n");
    }else{
      printf("^C received\n");
      printf("That's it, I'm shutting you down...\n");
      exit (0);
    }
}