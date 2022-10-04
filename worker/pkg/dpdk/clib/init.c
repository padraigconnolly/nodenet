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

extern void init(){
    int argc = 7;
    char *argv[7];
    int ret;
    unsigned lcore_id;

    argv[0] = " ";      // This is where the start command should be
    argv[1] = "-a";
    argv[2] = "0000:01:00.0";
    argv[3] = "-a";
    argv[4] = "0000:01:00.1";
    argv[5] = "-l";
    argv[6] = "2,3";

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
		rte_panic("Cannot init EAL\n");

    RTE_LCORE_FOREACH_WORKER(lcore_id) {
		/* Simpler equivalent. 8< */
		rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
		/* >8 End of simpler equivalent. */
	}

    /* call it on main lcore too */
	lcore_hello(NULL);
	/* >8 End of launching the function on each lcore. */

    rte_eal_mp_wait_lcore();

    /* clean up the EAL */
	rte_eal_cleanup();
}

extern void DPDK(struct EALParams eal){
    printf("\nmyNum is: %d\n", eal.myNum[1]);
    printf("\nmyLetter is: %s\n", eal.myLetter);
    printf("\nmyString is: %s\n", eal.myString[1]);
    init();
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