
typedef int if_ordinal;

typedef struct arp_types_ {
    if_ordinal iod;
    int dmac;
}arp_types_t;

#define MY_PRINT(fmt, args...) {\
    char buf[100];\
    memset(buf, 0, sizeof(buf));\
    snprintf(buf, 100, "iod: %d dmac: %d INFO: "fmt, t.iod, t.dmac, ##args);\
    printf("buf: %s \n", buf);\
}

#define INIT_DEFAULTS() {\
    memset(&t, 0, sizeof(t));\
}

