#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IP_CONST_IF_MODE_ANYCAST_MAC 32
#define IP_CONST_IF_MODE_ANYCAST_V4 8
#define IP_CONST_IF_MODE_ANYCAST_V6 16
#define IP_CONST_IF_MODE_DEFAULT_VALUE 0
#define IP_CONST_IF_MODE_EXTERNAL 256
#define IP_CONST_IF_MODE_FTEP 2
#define IP_CONST_IF_MODE_INFRA_VTEP 64
#define IP_CONST_IF_MODE_PERVASIVE 128
#define IP_CONST_IF_MODE_PROXY_MAC 2048
#define IP_CONST_IF_MODE_PROXY_V4 512
#define IP_CONST_IF_MODE_PROXY_V6 1024
#define IP_CONST_IF_MODE_PTEP 1
#define IP_CONST_IF_MODE_UNSPECIFIED 0
#define IP_CONST_IF_MODE_VPC 4

#define IFMODE_STR_LEN 100

#define IFMODE_STR_APPEND(mode, str) {\
    if (if_mode & mode) {\
        if (!is_first) {\
            is_first = 1;\
            strncpy(if_mode_str, (str), IFMODE_STR_LEN);\
        } else {\
            strncat(if_mode_str, ", ", IFMODE_STR_LEN);\
            strncat(if_mode_str, str, IFMODE_STR_LEN);\
        }\
    }\
}

char *get_mode_str(int if_mode)
{
    static char if_mode_str[IFMODE_STR_LEN];
    int is_first = 0;

    memset (if_mode_str, 0, IFMODE_STR_LEN);
    strncpy(if_mode_str, "unspecified", IFMODE_STR_LEN);

        IFMODE_STR_APPEND(IP_CONST_IF_MODE_ANYCAST_MAC, "anycast-mac");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_ANYCAST_V4, "anycast-v4");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_ANYCAST_V6, "anycast-v6");
        IFMODE_STR_APPEND (IP_CONST_IF_MODE_EXTERNAL, "external");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_FTEP, "ftep");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_INFRA_VTEP, "infra-vtep");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_PERVASIVE, "pervasive");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_PROXY_MAC, "proxy-mac");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_PROXY_V4, "proxy-v4");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_PROXY_V6, "proxy-v6");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_PTEP, "ptep");
        IFMODE_STR_APPEND(IP_CONST_IF_MODE_VPC, "vpc");

        return if_mode_str;
}

int main(int argc, char *argv[])
{

    printf(" %s \n", get_mode_str( atoi( argv[1] )) ); 

    return 0;
}

