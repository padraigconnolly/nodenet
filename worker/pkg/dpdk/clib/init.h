#ifndef _INITLIB_H_
#define _INITLIB_H_

#include <rte_eal.h>
#include <rte_debug.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_ethdev.h>
#include <rte_malloc.h>

struct EALParams {   // Structure declaration
  //int *myNum;           // List of Numbers
  //char *myLetter;       // String
  //char **myString;      // List of Strings
  char **ports;         // List of Ports
  int numPorts;         // Number of Ports
  int *cpus;            // List of CPUs
  int numCpus;          // Number of CPUs
  int numArgs;          // Number of EAL Arguments
};

extern void DPDK(struct EALParams eal);

#endif