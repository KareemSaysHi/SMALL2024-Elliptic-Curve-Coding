#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

#define MAX_COMMAND_LENGTH 1000  

FILE *fptr;

int check_power_of_two(int numtocheck) {
    while (numtocheck != 1 ) {
        if (numtocheck % 2 != 0) return 0;
        numtocheck /= 2;
    }
}

int main(int argc, char **argv) {
    int upper = 0;
    int numforks = 0;
    char **command = NULL;
    pid_t pid;


    /* check inputs*/
    if(argc < 3) {
        printf("unexpected number of arguments\n");
        return -1;
    }
    
    upper = atoi(argv[1]);
    numforks = atoi(argv[2]);


    if (argc < 4) { //builtin manual override
        if (!(check_power_of_two(numforks)) || (numforks > 100))   {
            printf("you need a forking number of a power of two, and if you're not on armlab probably keep it 8 or less\n");
            return -1;
        }
    }
    
    /* format the forking */

    command = malloc(numforks * sizeof(char*));

    for (int i = 0; i < numforks; i++) {
        command[i] = malloc(MAX_COMMAND_LENGTH);
        //sprintf(command[i], "./test.exe %d %d", i*upper/numforks, (i+1)*upper/numforks);
        //printf("./test.exe %d %d\n", i*upper/numforks, (i+1)*upper/numforks);
    }

    /* and now we do parallel */
    for (int i = 0; i < numforks; i++) {
        pid = fork();

        if (pid == 0) {
            printf("running command %s\n", command[i]);
            system(command[i]);
            printf("done with %d\n", i);
            exit(0);
        }
    }

    for (int i = 0; i < numforks; i++) {
        wait(NULL);
    }
    
    return 0;
}