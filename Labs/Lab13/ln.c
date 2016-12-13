/* Lab 13 - Programming Assignment
 * ln command in C
 * By: Hayden Miedema and Sage Heiss
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

// Helper string comparison function
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

int main(int argc, char *argv[]) { 
  
  if(argc < 4){
    link(argv[1], argv[2]);
    exit(1);
  }
  if(strEquals(argv[1], "-s") == 1){
    symlink(argv[2], argv[3]);
    exit(1);
  }
  
  if(strEquals(argv[1], "-T") == 1){
    link(argv[2], argv[3]);
    exit(1);
  }
  
  
  return 0;
}