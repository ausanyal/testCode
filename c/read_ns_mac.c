#include <stdio.h>

typedef struct mac_ {
    unsigned char mac[6];
} mac_t;

int main()
{
    unsigned long long ns_mac = 0x000c0c0c0c0c;
    mac_t m = {0};

    //printf("sz_ns_mac: %lu (0x%012llx) mac: %lu \n", sizeof(ns_mac), ns_mac, sizeof(mac));
    m.mac[0] = ns_mac >>40 & 0xff;
    m.mac[1] = ns_mac >>32 & 0xff;
    m.mac[2] = ns_mac >>24 & 0xff;
    m.mac[3] = ns_mac >>16 & 0xff;
    m.mac[4] = ns_mac >>8  & 0xff;
    m.mac[5] = ns_mac >>0  & 0xff;
    //m.mac = (unsigned char *)&ns_mac;
    printf("%x %x %x %x %x %x \n", m.mac[0], m.mac[1], m.mac[2], m.mac[3], m.mac[4], m.mac[5]);

    char str[100] = "Hello";

    printf("%s \n", str);
    printf("%c %c %c\n", str[0], str[1], str[99]);

    int i = 0x10203040;
    unsigned char *p = (unsigned char *)&i;
    printf("%x %x \n", p[0], p[1]);

    return 0;
}
