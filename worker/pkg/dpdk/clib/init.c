#include "init.h"
#include <stdio.h>

static int
lcore_hello(__rte_unused void *arg)
{
        unsigned lcore_id;
        lcore_id = rte_lcore_id();
        printf("hello from core %u\n", lcore_id);
        return 0;
}

int countDigits( int value )
{
    int result = 0;
    while( value != 0 ) {
       value /= 10;
       result++;
    }
    return result;
}

extern void init(int argc, char **argv){
    int ret;
    unsigned lcore_id;

    for (int j = 0; j < argc; j++){
            printf("Argv [%d]: %s\n", j, argv[j]);
        }
    ret = rte_eal_init(argc, argv);
    if (ret < 0)
		rte_panic("Cannot init EAL\n");

    RTE_LCORE_FOREACH_WORKER(lcore_id) {
		rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
	}

	lcore_hello(NULL);

    rte_eal_mp_wait_lcore();

	rte_eal_cleanup();
}

int eal_param_parse(struct EALParams eal, int argc, char **argv) {
    int counter = 0;
    // TODO Add check that argc is the same as number of arguments
    argv[counter] = " ";
    counter++;

    for (int i = 0; i < eal.numPorts; i++) {
        argv[counter] = "-a";
        counter++;
        argv[counter] = eal.ports[i];
        counter++;
    }

    if (eal.numCpus > 0){
        int firstintsize = countDigits(eal.cpus[0]);
        char* cpustring = malloc(firstintsize);

    // CPUs to string
        for (int i = 0; i < eal.numCpus; i++) {
            if (i == 0) { // If first digit do not put in leading comma
                sprintf(cpustring, "%d", eal.cpus[i]);
            } else { // If NOT first digit put in leading comma
                int intsize = countDigits(eal.cpus[i]);
                char cpustringnext[intsize];
                sprintf(cpustringnext, ",%d", eal.cpus[i]);
                strcat(cpustring, cpustringnext);
            }
            printf("CPU: %d\n", eal.cpus[i]);
        }
        printf("Cpustring: %s\n", cpustring);
        argv[counter] = "-l";
        counter++;
        argv[counter] = cpustring;
        counter++;
    }
   return 1;
}



// Function called by Golang
extern void DPDK(struct EALParams eal){
    //printf("\nmyNum is: %d\n", eal.myNum[1]);
    //printf("\nmyLetter is: %s\n", eal.myLetter);
    //printf("\nmyString is: %s\n", eal.myString[1]);
    int argc = eal.numArgs;
    char *argv[argc];
    eal_param_parse(eal, argc, argv);
    printf("Argvss [%d]: %s\n", 6, argv[6]);
    printf("Testing %s\n",argv[0]);
    printf("Testing %s\n",argv[6]);
    printf("Argvss [%d]: %s\n", 6, argv[6]);
    init(argc, argv);
}

/*

Sample C Code

#include <stdio.h>

struct EALParams {   // Structure declaration
  int  *myNum;           // Member (int variable)
  char *myLetter;
  char **myString;
};

int func(struct EALParams eal){
    printf("\nmyString is: %s\n", eal.myString[1]);
    return 1;
}

int main()
{
    struct EALParams eal;
    
    char *test = "Hi there";

    char *string[2] = {"Hi There33", "Hi There55"};
    char **ptr = string;
    
    int list[3] = {1,5,6};
    eal.myNum = list;
    eal.myLetter = test;
    eal.myString = ptr;
    
    printf("%s", eal.myLetter);
    
    int ret = func(eal);

    return 0;
}

*/
