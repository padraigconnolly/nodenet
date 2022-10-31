#include "init.h"
#include <stdio.h>

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024

#define BURST_SIZE 32

struct lcore_params {
    uint16_t portid;
};

static int
lcore_pkt_process(struct lcore_params *p){
    uint16_t portid = p->portid;
    printf("Lcore Id: %d Port ID: %d\n", rte_lcore_id(), portid);
    fflush(stdout);
    return 0;

    /*RTE_ETH_FOREACH_DEV(port)
		if (rte_eth_dev_socket_id(port) >= 0 &&
				rte_eth_dev_socket_id(port) !=
						(int)rte_socket_id())
			printf("WARNING, port %u is on remote NUMA node to "
					"polling thread.\n\tPerformance will "
					"not be optimal.\n", port);

    printf("\nCore %u forwarding packets. [Ctrl+C to quit]\n",
			rte_lcore_id());

	for (;;) {
		RTE_ETH_FOREACH_DEV(port) {

			struct rte_mbuf *bufs[BURST_SIZE];
			const uint16_t nb_rx = rte_eth_rx_burst(port, 0,
					bufs, BURST_SIZE);

			if (unlikely(nb_rx == 0))
				continue;

			const uint16_t nb_tx = rte_eth_tx_burst(port, 0,
					bufs, nb_rx);

			if (unlikely(nb_tx < nb_rx)) {
				uint16_t buf;
				for (buf = nb_tx; buf < nb_rx; buf++)
					rte_pktmbuf_free(bufs[buf]);
			}
		}
	}*/
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

static inline int
port_init(uint16_t port, struct rte_mempool *mbuf_pool)
{
    struct rte_eth_conf port_conf;
	const uint16_t rx_rings = 1, tx_rings = 1;
	uint16_t nb_rxd = RX_RING_SIZE;
	uint16_t nb_txd = TX_RING_SIZE;
	int retval;
	uint16_t q;
	struct rte_eth_dev_info dev_info;
	struct rte_eth_txconf txconf;

    if (!rte_eth_dev_is_valid_port(port))
		return -1;

	memset(&port_conf, 0, sizeof(struct rte_eth_conf));

    retval = rte_eth_dev_info_get(port, &dev_info);
	if (retval != 0) {
		printf("Error during getting device (port %u) info: %s\n",
				port, strerror(-retval));
		return retval;
	}


    if (dev_info.tx_offload_capa & RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE)
		port_conf.txmode.offloads |=
			RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE;

    retval = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
	if (retval != 0)
		return retval;

    retval = rte_eth_dev_adjust_nb_rx_tx_desc(port, &nb_rxd, &nb_txd);
	if (retval != 0)
		return retval;

	/* Allocate and set up 1 RX queue per Ethernet port. */
	for (q = 0; q < rx_rings; q++) {
		retval = rte_eth_rx_queue_setup(port, q, nb_rxd,
				rte_eth_dev_socket_id(port), NULL, mbuf_pool);
		if (retval < 0){
            printf("RX Queue setup failed");
            return retval;
        }
			
	}

    txconf = dev_info.default_txconf;
	txconf.offloads = port_conf.txmode.offloads;
	/* Allocate and set up 1 TX queue per Ethernet port. */
	for (q = 0; q < tx_rings; q++) {
		retval = rte_eth_tx_queue_setup(port, q, nb_txd,
				rte_eth_dev_socket_id(port), &txconf);
		if (retval < 0)
			return retval;
	}

    /* Starting Ethernet port. 8< */
	retval = rte_eth_dev_start(port);
	/* >8 End of starting of ethernet port. */
	if (retval < 0)
		return retval;

	/* Display the port MAC address. */
	struct rte_ether_addr addr;
	retval = rte_eth_macaddr_get(port, &addr);
	if (retval != 0)
		return retval;

    printf("Port %u MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
        port,
        (unsigned char) addr.addr_bytes[0],
        (unsigned char) addr.addr_bytes[1],
        (unsigned char) addr.addr_bytes[2],
        (unsigned char) addr.addr_bytes[3],
        (unsigned char) addr.addr_bytes[4],
        (unsigned char) addr.addr_bytes[5]);
    
    /* Enable RX in promiscuous mode for the Ethernet device. */
	retval = rte_eth_promiscuous_enable(port);
	/* End of setting RX port in promiscuous mode. */
	if (retval != 0){
        printf("Promiscuous mode config error\n");
        return retval;
    }

    return 0;
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
    unsigned nb_ports;
    char *argv[argc];
    int ret;
    struct rte_mempool *mbuf_pool;
    uint16_t portid;
    unsigned lcore_id;

    eal_param_parse(eal, argc, argv);
    for (int j = 0; j < argc; j++){
            printf("Argv [%d]: %s\n", j, argv[j]);
        }
    ret = rte_eal_init(argc, argv);
    if (ret < 0)
		rte_panic("Cannot init EAL\n");

    nb_ports = rte_eth_dev_count_avail();
	if (nb_ports < 1)
		rte_exit(EXIT_FAILURE, "Error: number of ports must be even\n");
    
    /* Allocates mempool to hold the mbufs. 8< */
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    RTE_ETH_FOREACH_DEV(portid)
		if (port_init(portid, mbuf_pool) != 0)
			rte_exit(EXIT_FAILURE, "Cannot init port %"PRIu16 "\n",
					portid);
    struct lcore_params *pr =
        rte_malloc(NULL, sizeof(*pr), 0);
    *pr = (struct lcore_params){portid};
    
    lcore_pkt_process(pr);
        
    RTE_LCORE_FOREACH_WORKER(lcore_id) {
		if (rte_eal_wait_lcore(lcore_id) < 0) {
			ret = -1;
			break;
		}
	}    


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
