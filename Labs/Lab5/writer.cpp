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

int shmId;
char * shmPtr;

int main()
{
	signal(SIGINT,sigHandler);
	char input[SIZE];
	
	char*s,*shmPtrA;
	key_t key;
	int proj_id = 's';
	
	// replace key with a path valid for the computer
	key = ftok("keyfile",proj_id);
	//cout<<"key: "<<key<<endl;
	
	if ((shmId = shmget (key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) { 
		cerr << "i can't get no..."<<endl;
		exit(1);
	} 
	
	//cout<<"shmId: "<<shmId<<endl;
	 
   if ((shmPtr = (char*)shmat (shmId, 0, 0)) == (void*) -1) { 
      cerr << "can't attach"<<endl; 
      exit (1); 
   }
   shmPtrA = shmPtr;
    while(1){
      cout<<"Enter Message: \n";
      cin.getline(input,SIZE); 
      s=shmPtr;
      for(int i=0; i<SIZE;i++) {
        *s++=input[i];
      }
      if(!strcmp(input, "quit")){
        shmdt(shmPtr);
        shmctl(shmId,IPC_RMID,NULL);
        cout<<endl;
        exit(0);
      }
      while(*shmPtr !='*'){} //sleep(1);
      shmPtr = shmPtrA;
    }
    return 0;
}

void sigHandler(int sigNum){
  shmdt(shmPtr);
  shmctl(shmId,IPC_RMID,NULL);
  cout<<endl;
  exit(0);	
}
	





