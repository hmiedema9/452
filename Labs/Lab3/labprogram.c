#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler (int);
pid_t pid;

int main () {
    int random1, random2;
    signal (SIGINT, sigHandler);
    
    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    }else if (!pid) {
      //child
      while(1){
	printf ("waiting...\t");
	random1 = rand() % 10;
	random2 = rand() % 2;
	sleep(random1);
	if(random2 == 0){
	  kill(pid, SIGUSR1);
	}else{
	  kill(pid, SIGUSR2);
	}
	
    }
    }
    while(1){
      pause();
      
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
      printf("That's it, I'm shutting you down...");
      exit (0);
    }
}