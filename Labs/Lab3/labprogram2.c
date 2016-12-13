#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>



void sigHandler (int);
pid_t pid;

int main () {
    printf("SIGUSR1: %d, SIGUSR2: %d\n", SIGUSR1, SIGUSR2);
    int random1, random2;
    signal (SIGUSR1, sigHandler);
    signal (SIGUSR2, sigHandler);
    signal (SIGINT, sigHandler);
    
    if ((pid = fork ()) < 0) {
        perror ("fork failed\n");
        exit (1);
    }else if (!pid) {
      //child
      while(1){
	
	random1 = (rand() % 7) + 1;
	random2 = rand() % 2;
	sleep(random1);
	if(random2 == 0){
	  kill(getppid(), SIGUSR1);
	}else{
	  kill(getppid(), SIGUSR2);
	}
      }
      printf("About to exit child.");
      exit(0);
    } else {
      // this is the parent
      printf("Parent pre while loop with pause.\n");
      while(1){
        pause();
      
      }
    }
    printf("Are we even getting this far?\n");
    return 0;
}

void sigHandler (int sigNum) {
      fflush(stdout);
      printf ("waiting...\t\n");
      fflush(stdout);
      printf("^C received\n");
      printf("That's it, I'm shutting you down...\n");
      exit (0);
    }


void sigHandler1 (int sigNum) {
      fflush(stdout);
      printf("received a SIGUSR1 signal\n");
}

void sigHandler2 (int sigNum) {
      fflush(stdout);
      printf("received a SIGUSR2 signal\n");
}


