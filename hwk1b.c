#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
pid_t pid, pid1;
pid = fork();
if(pid < 0){
	fprintf(stderr, "fork() failed\n");
	return(1);
}
else if (pid==0){
	pid1 = getpid();
	printf("A - pid = %d\n", pid);
	printf("B - pid1 = %d\n", pid1);
}
else{
	pid1 = getpid();
	printf("C - pid = %d\n", pid);
	printf("D - pid1 = %d\n", pid1);
	wait(NULL);
}
return 0;
}

	
