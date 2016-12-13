#include <unistd.h>
#include <stdio.h>


int main(){
	printf("Before\n");
	execl("/usr/bin/cal", "Venus", NULL);
	printf("After");
	return 0;

}
