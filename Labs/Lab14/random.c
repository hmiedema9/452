/* Lab 14 - Pseudo Random Number Generator
 * CIS 452 - Professor Dulimarta
 * By: Hayden Miedema and Sage Heiss
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>


int main () {
int randomData = open("/dev/urandom", O_RDONLY);
char myRandomData[50];

size_t randomDataLen = 0;
while (randomDataLen < sizeof myRandomData)
{
    ssize_t result = read(randomData, myRandomData + randomDataLen, (sizeof myRandomData) - randomDataLen);
    if (result < 0)
    {z
        // error, unable to read /dev/random 
    }
    randomDataLen += result;
}

close(randomData);

printf("Random Number: %d\n", (int)myRandomData);

return 0;

}