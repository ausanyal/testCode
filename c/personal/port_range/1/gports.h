#ifndef __GPORTS_H__
#define __GPORTS_H__

typedef unsigned short ushort;

static int debug = 0;

#define d_printf if (debug) printf

#define mdprintf(arg, var...) {                                   \
    if (debug) {                                                  \
        printf("%-50s.%-3d: "arg"\n", __FUNCTION__, __LINE__, ##var); \
    }                                                             \
}

#define mprintf(arg, var...) {                                   \
    printf("%-50s.%-3d: "arg"\n", __FUNCTION__, __LINE__, ##var); \
}

typedef struct port_mask_ {
  ushort port;
  ushort mask;
} port_mask_t;

ushort  // Port mask count;
get_best_block(ushort start, ushort end, port_mask_t *pmask, ushort count,
               ushort *bstart, ushort *bend);


#endif
