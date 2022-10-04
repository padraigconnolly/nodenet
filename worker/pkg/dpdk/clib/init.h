#ifndef _INITLIB_H_
#define _INITLIB_H_

#include <rte_eal.h>
#include <rte_debug.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_ethdev.h>

struct EALParams {   // Structure declaration
  int *myNum;           // Member (int variable)
  char *myLetter;
};

extern void DPDK(struct EALParams eal);

#endif