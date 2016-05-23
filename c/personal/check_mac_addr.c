#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char mac_addr[6];

typedef union L2addrtype_ {
    mac_addr ethernet_address;
    unsigned short dlci;
}L2addrtype;

typedef struct mac_ {
    unsigned char ether_addr_octet[6];
} __attribute__ ((packed))  mac_t;

int main()
{
    L2addrtype l2_addr;
    mac_t arp_mac;

    memset(&l2_addr, 0, sizeof(L2addrtype));
    memset(&arp_mac, 0, sizeof(mac_t));

    printf("mac_t %lu l2addr %lu \n", sizeof(mac_t), sizeof(l2_addr.ethernet_address));

    if (!memcmp(&arp_mac, &l2_addr.ethernet_address, sizeof(mac_t))) {
        printf("!memcmp \n");
    }
    else {
        printf("memcmp \n");
    }

    return 0;
}
