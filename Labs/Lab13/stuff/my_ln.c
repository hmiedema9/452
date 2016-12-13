#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
int main(int argc, const char *argv[]) 
{ 
	printf("%i\n",argc);
	int status;
	 if(strcmp(argv[1],"-s") == 0){
	 status = symlink(argv[2],argv[3]);
	 if(status < 0){
		 printf("error soft linking\n");
	 }
	}else if (argc == 3){
		status = link(argv[1],argv[2]);
		 if(status < 0){
		 printf("error hard linking\n");
	 }
	}
			
return 0;
}
