#include <stdio.h>
#include <string.h>

int main()
{
    //int mac[6], mac2[6];
    unsigned int mac[6] = {1,2,3,4,5,6};
    unsigned int mac2[6] = {2,3,4,5,6, 7};
    printf("m = %u %u %u %u %u %u m.size = %lu \n", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
            sizeof(mac));
    printf("m2 = %u %u %u %u %u %u m2.size = %lu \n", 
            mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5],
            sizeof(mac2));
    memcpy(mac, mac2, sizeof(mac));

    printf("m = %u %u %u %u %u %u m.size = %lu \n", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
            sizeof(mac));


    return 0;

}
