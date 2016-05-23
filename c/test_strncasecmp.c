#include <stdio.h>
#include <string.h>

char line[] = "link_accel: 1/97, PROFILE1";

int main()
{
    char mode[10];
    int slot, port;
    
    sscanf(line, "link_accel: %d/%d, %s", &slot, &port, mode);
    if (!strncasecmp(mode, "profile1", 8)) {
        printf("found \n");
    } else {
        printf("not found \n");
    }

    return 0;
}
