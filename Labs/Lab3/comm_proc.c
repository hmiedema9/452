/* CIS 452 - Lab 4
 *  By: Hayden Miedema and Sage Heiss
*/

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <signal.h>
#include<sys/wait.h>
#include<math.h>
#define READ 0
#define WRITE 1

void sigHandler (int);
void sigHandlerOne(int);
void sigHandlerTwo(int);
static int num2;

int main() 
{ 
  pid_t pid;
  int num;
  int fd[2];
  
  
  if (pipe (fd) < 0) {
	    perror ("plumbing problem");
        exit (1);
  }
  
    if ((pid = fork()) < 0) { 
        perror("fork failure"); 
        exit(1); 
    } 
    else if (pid == 0) { 
         while(1){
            num=rand();
	    num2 = rand() % 5;
            signal(SIGINT,sigHandler);
            if(num%2 == 0){
                kill(getppid(),SIGUSR1); //send sig1 to parent
            }    
            else{       
                kill(getppid(),SIGUSR2);
            }
            sleep(num2);
         }  
    } 
    else { 
       signal(SIGUSR1,sigHandlerOne);
       signal(SIGUSR2,sigHandlerTwo);
       
       while(1){
	fprintf(stderr, "Waiting...");
	pause();
      }
       exit(0);
    }
 return 0; 

}

void 
sigHandler (int sigNum) 
{ 
  fflush(stdout);
  printf("  recieved... Im out of here\n"); 
  exit(0);
}

void 
sigHandlerOne (int sigNum) 
{
    sleep(num2);
    printf ("Received a SIGUSR1 signal\n"); 
}

void 
sigHandlerTwo (int sigNum) 
{
     sleep(num2);
     printf ("Received a SIGUSR2 signal\n"); 
}
