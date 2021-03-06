#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>


// return 0 for false and 1 for true
int strEquals(char * first, char * second){
  if(strlen(first) != strlen(second)) {
    return 0;
  }
  int i = 0;
  int size = strlen(first);
  while(i < size){
    if(first[i] != second[i]){
      return 0;
    }
    i++;
  }
  return 1;
}

int main(int argc, char * argv[])
{
    DIR *dirPtr;
    char * name;
    struct group * g;
    struct stat statBuf;
    struct dirent *entryPtr;
    int boolean;
    char * path;
    struct passwd* pw;
    
    switch(argc){ // will be ./ls
      case 1:
	//argv[1] = "-noArg";
	boolean = 2;
	path = ".";
	break;
      case 2: // could be ./ls [arg] || ./ls [path]
	if(strEquals(argv[1], "-i") == 1){ // ./ls -i
	  boolean = 1;
	  path = ".";
	} else {
	    if(strEquals(argv[1], "-l") == 1){ // ./ls -l
	      boolean = 0; 	
	      path = ".";
	    } else { // ./ls [path]
	      boolean = 2;
	      path = argv[1];
	    }
	}
	break;
      case 3: // could be ./ls [arg] [path]  || ./ls [arg] [arg]
	// the second command argument should be an ls argument - exit if not
	if(strEquals(argv[1], "-i") != 1 && strEquals(argv[1], "-l") != 1){
	  printf("Can only handle arguments: -i or -l (not both)\n");
	  printf("Format: ./ls [OPTIONAL: argument] [OPTIONAL: path]\n");
	  exit(1);
	}
	// this program will not handle both -l and -i at the same time
	if(strEquals(argv[2], "-i") == 1 || strEquals(argv[2], "-l") == 1){
	  printf("Can only handle: -i or -l (not both)\n");
	  printf("Format: ./ls [OPTIONAL: argument] [OPTIONAL: path]\n");
	  exit(1);
	}
	path = argv[2];
	if(strEquals(argv[1], "-i") == 1){
	  boolean = 1;
	} else {
	  boolean = 0;
	}
	
	
	if(strEquals(argv[1], "-l") == 0){
	  boolean = 0;
	} 

	if(strEquals(argv[1], "-li") == 0 || strEquals(argv[1], "-il") == 0){
	  boolean = 3;
	} else {
	  boolean = 1;
	}
	
	
      break;
      default:
	printf("Can only handle arguments: -i or -l (not both)\n");
	printf("Format: ./ls [OPTIONAL: argument] [OPTIONAL: path]\n");
	exit(1);
    }
    
    if((dirPtr = opendir(path)) <= 0){
      printf("PATH WAS INVALID\n");
      printf("Can only handle arguments: -i or -l (not both)\n");
      printf("Format: ./ls [OPTIONAL: argument] [OPTIONAL: path]\n");
      exit(1);
    }
    
    
    //file path
   char buf[512];
   
    while ((entryPtr = readdir(dirPtr)) != NULL){ // for each file in current directory
      name = entryPtr->d_name;
     
//      if(stat(entryPtr->d_name, &statBuf) == -1){
// 	printf("*** Error accessing stat from %-15s.\n", name);
//  	continue;
//        }
      
      switch(boolean){
	case 0: // ls -l
// 	  if((entryPtr = readdir(dirPtr)) == NULL){
// 	    printf("Directory read error");
// 	  }
	  // TODO: Insert proper information
	  
	  // append to file path
	  sprintf(buf, "%s/%s", path, name);
	  stat(buf, &statBuf);
	  
	  // group information
	 if((g = getgrgid(statBuf.st_gid)) == NULL ) {
	    fprintf( stderr, "getgrgid: NULL pointer\n" );
	    return( EXIT_FAILURE );
	  }
	  
	  if((pw = getpwuid(statBuf.st_uid)) == NULL ) {
	    fprintf( stderr, "getpwuid: no password entry\n" );
	    return( EXIT_FAILURE );
    }
    
	  //printf("File Permissions: ");
	  printf( (S_ISDIR(statBuf.st_mode)) ? "d" : "-");
	  printf( (statBuf.st_mode & S_IRUSR) ? "r" : "-");
	  printf( (statBuf.st_mode & S_IWUSR) ? "w" : "-");
	  printf( (statBuf.st_mode & S_IXUSR) ? "x" : "-");
	  printf( (statBuf.st_mode & S_IRGRP) ? "r" : "-");
	  printf( (statBuf.st_mode & S_IWGRP) ? "w" : "-");
	  printf( (statBuf.st_mode & S_IXGRP) ? "x" : "-");
	  printf( (statBuf.st_mode & S_IROTH) ? "r" : "-");
	  printf( (statBuf.st_mode & S_IWOTH) ? "w" : "-");
	  printf( (statBuf.st_mode & S_IXOTH) ? "x" : "-");
	  //printf("\n\n");
	  
	  // print info with names for debugging
// 	  printf("\tnlinks: %i", (int)statBuf.st_nlink);
// 	  //printf(" \tUser ID: %i", statBuf.st_uid);
// 	  printf( "\tlogin name: %s", pw->pw_name );
// 	  printf( "\tgroup name: %s", g->gr_name );
// 	  printf("\tSize: %u", (unsigned)statBuf.st_size);
// 	  printf("\tinode: %i", (int)statBuf.st_ino);
	  
	  // print info to replicate -l
	  printf(" %i", (int)statBuf.st_nlink);
	  //printf(" \tUser ID: %i", statBuf.st_uid);
	  printf( " %s", pw->pw_name );
	  printf( " %s", g->gr_name );
	  printf(" %u", (unsigned)statBuf.st_size);
	  
	  
	  //format the time
	  char buff[20];
	  struct tm * timeinfo;
	  timeinfo = localtime(&(statBuf.st_mtime));
	  strftime(buff, 20, "%b %d %H:%M", timeinfo);
	  printf(" %s", buff);
	  printf("\t%-20s\n", entryPtr->d_name);
	  
// 	  if((g = getgrnam(entryPtr->d_name)) == NULL){
// 	    fprintf( stderr, "getgrnam: %s failed\n",
//             entryPtr->d_name );
// 	    
// 	 // printf("%d %-15s\n", (int)entryPtr->d_ino, name);
//       }
	  //printf("Got here, must be an error\n");
	  //printf( "group name:%s\n", g->gr_name );
	  
	  //printf("%-10s %d %-10s %-10s %d %-3s %d %d:%d %-15s\n", "-rwxrwxrwx", 10, "root", g->gr_name, 2048, "Jan", 13, 7, 11, "afile.exe");
	  break;
	case 1: // ls -i
	  printf("%d %-15s\n", (int)entryPtr->d_ino, name);
	  break;
	case 2:
	  printf("%-15s\n", name);
	  break;
	case 3:
	  
	  
      }
    }
    closedir(dirPtr);
    return 0;
}

/************************************************************
+------------------+-----------------+-------+-------+------+-------+-----+-------+-----------+
| file permissions | number of links | owner | group | size | month | day | time  | filename  |
+--------------+------------------+-----------------+-------+-------+------+-------+-----+-------+-----------+
| -rwxrw-r--       |              10 | root  | root  | 2048 | Jan   |  13 | 07:11 | afile.exe |
+------------------+-----------------+-------+-------+------+-------+-----+-------+-----------+
*********************************************/