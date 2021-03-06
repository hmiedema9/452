#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

int main (int argc, char * argv[]) {
    int status;
    long int i, loop, temp, *shmPtr;
    int shmId, semId;
    pid_t pid;
    union semun semval;
    struct sembuf sem_op;

    /*
     * TODO: get value of loop variable(from command - line
     * argument
     */
    if(argc > 0){
      loop = atol(argv[1]);
    }else if(argc <= 1){
      printf("Need to supply arguments");
      exit(1);
    }else {
      loop = 0;
    }
    
    if ((shmId = shmget (IPC_PRIVATE, SIZE,
                         IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    shmPtr[0] = 0;
    shmPtr[1] = 1;
    
    semId = semget(IPC_PRIVATE, 1, 00600);
    semval.val = 0;
    semctl (semId, 0, SETVAL, 1);
    

    if (!(pid = fork ())) {
      //request resource
	
        for (i = 0; i < loop; i++) {    
            /*
             * TODO: swap the contents of shmPtr[0] and  shmPtr[1]
             */
	    sem_op.sem_num = 0; // which semaphore
	    sem_op.sem_op = -1; // decrement 
	    sem_op.sem_flg=0; 
	    semop(semId,&sem_op,1);
	
	    temp = shmPtr[0];
	    shmPtr[0] = shmPtr[1];
	    shmPtr[1] = temp;
	    
	    sem_op.sem_num = 0; // which semaphore?
	    sem_op.sem_op = 1; // increment
	    sem_op.sem_flg=0;
	    semop(semId,&sem_op,1);
	     //printf("Swapped the values %li times\n", loop);
        }
        
        if (shmdt (shmPtr) < 0) {
            perror ("just can 't let go\n");
            exit (1);
        }
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {

            /*
             * TODO: swap the contents of shmPtr[1] and shmPtr[0]
             */
	    sem_op.sem_num = 0; // which semaphore
	    sem_op.sem_op = 1; // decrement 
	    sem_op.sem_flg=0; 
	    semop(semId,&sem_op,1);
	    
	    temp = shmPtr[0];
	    shmPtr[0] = shmPtr[1];
	    shmPtr[1] = temp;
	    
	    sem_op.sem_num = 0; // which semaphore?
	    sem_op.sem_op = -1; // increment
	    sem_op.sem_flg=0;
	    semop(semId,&sem_op,1);
        }
        
    }

    wait (&status);
    semctl (semId, 0, IPC_RMID, 0);
    
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    return 0;
}