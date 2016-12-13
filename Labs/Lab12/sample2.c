#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat statBuf;
    
    dirPtr = opendir(".");

    while ((entryPtr = readdir(dirPtr))){
	printf("%-20s", entryPtr->d_name);
      if(!stat(entryPtr->d_name, &statBuf)){
	printf (" %lu Bytes\n", (unsigned long) statBuf.st_size);
      }
    }
    closedir(dirPtr);
    return 0;
}