/* Lab 5 - CIS 452
 * By: Hayden Miedema and Sage Heiss
 */

#include <string.h>
#include <iostream>
#include <stdlib.h>  
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define SIZE 128
void sigHandler(int sigNum);

using namespace std;

char *shmPtr;
int shmId;
int main()
{
	signal(SIGINT,sigHandler);
	char *s;
	key_t key;
	int proj_id = 's';
	
	// replace key with a path valid for the computer
	key = ftok("keyfile",proj_id);
		
	if ((shmId = shmget (key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) { 
		cerr << "i can't get no..."<<endl;
		exit(1);
	} 
	 
    if ((shmPtr = (char*)shmat (shmId, 0, 0)) == (void*) -1) { 
      cerr << "can't attach"<<endl; 
      exit (1); 
    }
    char * message = (char *)malloc(128);
    while(1){
      if(*shmPtr!='*'){
	message = shmPtr;
        for(s = shmPtr; *s!='\0';s++){
          putchar(*s);
	}
        putchar('\n');
       // sleep(1);
	if(!strcmp(message, "quit")) {
          shmdt(shmPtr);
          shmctl(shmId,IPC_RMID,NULL);
          cout<<endl;
          exit(0);
        }
        *shmPtr='*';
      }
      while(*shmPtr=='*') //sleep(1)
	;
    }
   
return 0;
}

void sigHandler(int sigNum){
	shmdt(shmPtr);
	shmctl(shmId,IPC_RMID,NULL);
	cout<<endl;
    exit(0);	
}
	
