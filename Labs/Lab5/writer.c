/* Lab 5 Programming Assignment - CIS 452
 * By: Hayden Miedema and Sage Heiss
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

int main () {
    int shmId;
    char *shmPtr;
    struct shmid_ds shmbuffer; 
    int proj_id = 1;
    key_t key = NULL;
    
    key = ftok("/home/miedemha/Documents/CIS452/Labs/Lab5/writer.c", proj_id);
    
    printf("key is: %d\n", key);
    
    if ((shmId = shmget (key, FOO,
                 IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }
    
    while(1) {
      printf("Type your message: ");
      fgets(shmId, 20, stdin);
    
    }
    
    shmctl(shmId, IPC_STAT, &shmbuffer);
    printf("Size of allocated segment: %i\n", (int)shmbuffer.shm_segsz); // prints the size of the allocated segment
    printf ("value a: %lu\t value b: %lu\n shmId: %i\n", (unsigned long) shmPtr,
            (unsigned long) shmPtr + FOO, shmId); //added the shmId to the print statement
      
    pause();
    
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


  
  
  
