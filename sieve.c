#include <pthread.h> 
#include <stdio.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>


/* 2 to range (inclusive) */
void filter_r(int fd);

/* first k primes */
void filter_k(int fd);

/* global*/
int n = 0;

/*global takes advantage of copy on write for first k primes*/
int primeCounter=0;


int main(int argc, char *argv[])
{

	int fd_p[2],pid,i;
	
    if(argc<=1)
    {
      printf("This program requires arguments\n");
      exit(1);
    }
    else if(argc==2)
    {   
      printf("the first %d primes are: \n",atoi(argv[1]));
      n=atoi(argv[1]); //argument to n
      
      pipe(fd_p); //create pipe for ipc
 
      if((pid=fork())==0){ //child
			close(fd_p[1]);//close write
			filter_k(fd_p[0]);//pass read fd to filter_k
	  }else{ //parent
		close(fd_p[0]);//close read prepare to write
		  for(i=2;;i++)//stream of ints
			write(fd_p[1],&i,sizeof(int));
		close(fd_p[1]);
	  }
    }
    else if(argc==3)
    {
      printf("the primes between 2 and %i (inclusive) are: \n",atoi(argv[2]));
      n=atoi(argv[2]); //argument to n
      i=atoi(argv[1]); //argument to i
      
      pipe(fd_p);
      
      if((pid=fork())==0){ //child
			close(fd_p[1]); //close write
			filter_r(fd_p[0]); //pass read fd to filter_r
	  }else{ //parent
		close(fd_p[0]); //close read prepare to write
		  for(;i<=n;i++)//stream of ints 
			write(fd_p[1],&i,sizeof(int));
		close(fd_p[1]);
	    sleep(1); //sleep for organized print out
	  }
    }
    else
    {
        printf("too many args\n");
    }
    
    return 0;
}

/*
 * Function: filter_k 
 * ---------------------
 * filters out the first k prime numbers of a stream starting from 2. 
 *  
 * fd: file descriptor read value
 * 
 * */
 
void filter_k(int fd)
{
    int fd_p[2],pid,prime,value;
    
    read(fd,&prime,sizeof(int)); //read filter_k params into prime
    if(primeCounter!=n){ //check if user requested primes have been found
     printf("child(%d) read: %d\n",getpid(),prime); //print prime
     primeCounter++; //increment counter indicating that prime has been found
	}
	
    if(primeCounter!=n){//base case
		pipe(fd_p);
		if((pid = fork())==0){ //child
			close(fd_p[1]);
			filter_k(fd_p[0]); //pass read descriptor down chain
		}else{ //parent
			close(fd_p[0]);
			while(read(fd,&value,sizeof(int)))//stream breaks when read reaches EOF
				if(value%prime!=0) //test stream value against prime 'proccess'
				write(fd_p[1],&value,sizeof(int));
		close(fd_p[1]);
	}
  }
} 


/*
 * Function: filter_r
 * ---------------------
 * filters out the prime numbers of a stream starting from 2 to k inclusive. 
 *  
 * fd: file descriptor read value
 * 
 * */

void filter_r(int fd)
{
    int fd_p[2],pid,prime,value;
    read(fd,&prime,sizeof(int)); //read 
    if(prime<=n){
     printf("child(%d) read: %d\n",getpid(),prime); //print prime
	}	
    if(prime<=n){ //base case
		pipe(fd_p);
		if((pid = fork())==0){ //child
			close(fd_p[1]);
			filter_r(fd_p[0]); //chain reaction
		}else{ //parent
			close(fd_p[0]);
			while(read(fd,&value,sizeof(int)))
				if(value%prime!=0)
				  write(fd_p[1],&value,sizeof(int));
		close(fd_p[1]);
	}
  }
}








