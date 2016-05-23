#include <stdio.h>
#include <stdlib.h>

// LC 1:  0x1a000000
// LC 2:  0x1a080000
//        000 - 080 => top 5 - slot ; bottom 7 - port 

// LC 3:  0x1a100000
// LC 4:  0x1a180000
// LC 5:  0x1a200000
// LC 6:  0x1a280000
// LC 7:  0x1a300000
// LC 8:  0x1a380000
// LC 9:  0x1a400000
// LC 10: 0x1a480000
// LC 11: 0x1a500000
// LC 12: 0x1a580000
// LC 13: 0x1a600000
// LC 14: 0x1a680000
// LC 15: 0x1a700000
// LC 16: 0x1a780000
#define NUM_LC_SLOTS 16
#define NUM_PORTS_PER_SLOT 48
#define DEV_NAME_LEN 10

#define IF_INDEX_PHY_ETH_SLOT_MASK  0x1F
#define IF_INDEX_PHY_ETH_PORT_MASK  0x7F

typedef unsigned int if_index_t;

typedef struct sim_port_dev_s {
    if_index_t ifindex;
    char *dev_name;
} sim_port_dev_t;

sim_port_dev_t spine_port_dev_map[NUM_LC_SLOTS * NUM_PORTS_PER_SLOT];

void find_ifindex(if_index_t ifindex)
{
    int slot, port, idx;

    slot = (ifindex >> 19) & IF_INDEX_PHY_ETH_SLOT_MASK;
    port = (ifindex >> 12) & IF_INDEX_PHY_ETH_PORT_MASK;

    idx = (slot * NUM_PORTS_PER_SLOT) + port;

    sim_port_dev_t *p_port_dev_map = &spine_port_dev_map[idx];
    printf("ifindex: 0x%x slot: %d port: %d idx: %d \n", ifindex, slot, port, idx);
}

int main()
{
    int idx = 0;

    for (int slot = 0; slot < NUM_LC_SLOTS ; slot++) {
        for (int port_in_slot = 0 ; port_in_slot < NUM_PORTS_PER_SLOT ; port_in_slot++) {

            idx = (slot * NUM_PORTS_PER_SLOT) + port_in_slot;

            spine_port_dev_map[idx].ifindex = ( (0x1a << 24) | 
                                                ((slot & IF_INDEX_PHY_ETH_SLOT_MASK) << 19 ) | 
                                                ((port_in_slot & IF_INDEX_PHY_ETH_PORT_MASK) << 12));

            spine_port_dev_map[idx].dev_name = (char *)malloc(DEV_NAME_LEN * sizeof(char));

            snprintf(spine_port_dev_map[idx].dev_name, DEV_NAME_LEN, "phy%d_%d", slot+1, port_in_slot+1);
            printf("idx: %d ifindex: 0x%x name: %s \n", idx, spine_port_dev_map[idx].ifindex,
                                                spine_port_dev_map[idx].dev_name);
        }
    }
    spine_port_dev_map[NUM_LC_SLOTS * NUM_PORTS_PER_SLOT].ifindex = 0;
    spine_port_dev_map[NUM_LC_SLOTS * NUM_PORTS_PER_SLOT].dev_name = NULL;

    find_ifindex(0x1a08c000);
     
    return 0;
}
