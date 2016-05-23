#include <stdio.h>
#include <math.h>

typedef unsigned int uint16;
typedef unsigned short bool;

#define FALSE 0
#define TRUE 1
#define ITR_UDP_SRC_PORT_MAGGIC_NUMBER 0xb
#define MAX_EXT_PATH 256

uint16
itr_get_srcport (uint16 path_id)
{
    // return (1000 + path_id) % 0x0fff;
    return path_id % 0x0fff;
}

uint16 itr_get_port_from_path_num(uint16 path_id, uint16 magic_num)
{
    uint16 src_port, running_part;

    running_part = itr_get_srcport(path_id);
    // src_port = ITR_UDP_SRC_PORT_MAGGIC_NUMBER;
    src_port = magic_num;
    src_port = src_port << 12;
    src_port = src_port & 0xf000;
    src_port = src_port | running_part;

    return src_port;
}

/*
 * If wildcard:
 * (A) Internal trace port range: 
 * Dport range - 16384 - 24575
 * Sport range - 24576 - 32767
 * (B) Internal trace port range: 
 * Dport range - 32768 - 49151
 * Sport range - 49152 - 65535
 * INT_DPS 16384 = 0100 0000 0000 0000 (0x04 00 00 00)
 * INT_DPE 24575 = 0101 1111 1111 1111 (0x05 FF FF FF)
 * INT_SPS 24576 = 0110 0000 0000 0000 (0x06 00 00 00)
 * INT_SPE 32767 = 0111 1111 1111 1111 (0x07 FF FF FF)
 * EXT_DPS 32768 = 1000 0000 0000 0000 (0x08 00 00 00) 
 * EXT_DPE 49151 = 1011 1111 1111 1111 (0x0B FF FF FF)
 * EXT_SPS 49152 = 1100 0000 0000 0000 (0x0C 00 00 00)
 * EXT_SPE 65535 = 1111 1111 1111 1111 (0x0F FF FF FF)
 */

#define INT_DPS_MAGIC_NUM (0x04)
#define INT_SPS_MAGIC_NUM (0x06)
#define EXT_DPS_MAGIC_NUM (0x08)
#define EXT_SPS_MAGIC_NUM (0x0c)

typedef enum {
    ITR_INT_TRACE = 1,
    ITR_EXT_TRACE
} trace_type_t; 

uint16 
itr_get_dport_from_path_num(uint16 path_id, trace_type_t type)
{
    if (type == ITR_INT_TRACE) {
        return itr_get_port_from_path_num(path_id, INT_DPS_MAGIC_NUM); 
    } else {
        return itr_get_port_from_path_num(path_id, EXT_DPS_MAGIC_NUM); 
    }
}

uint16 
itr_get_sport_from_path_num(uint16 path_id, trace_type_t type)
{
    if (type == ITR_INT_TRACE) {
        return itr_get_port_from_path_num(path_id, INT_SPS_MAGIC_NUM); 
    } else {
        return itr_get_port_from_path_num(path_id, EXT_SPS_MAGIC_NUM); 
    }
}

typedef struct cfg_ {
    int sps;
    int spe;
    int dps;
    int dpe;
    int np;
    int npp;
}cfg_t;

#define SPS (cfg->sps)
#define SPE (cfg->spe)
#define DPS (cfg->dps)
#define DPE (cfg->dpe)
#define NP  (cfg->np)
#define NPP (cfg->npp)

void
itr_ext_validate_get_sport_dport_range (cfg_t *cfg, trace_type_t trace_type)
{
    bool sport_wildcard = FALSE, dport_wildcard = FALSE;
    unsigned int  num_sports = 0, num_dports = 0;
    double np_sqrt;

    printf("=====================================================================\n");
    printf("I/P: sps(%d) spe(%d) dps(%d) dpe(%d) np(%d) \n", 
            SPS, SPE, DPS, DPE, NP);
    printf("---------------------------------------------------------------------\n");

    if ((SPS == 0) && (SPE == 65535)) { // Wild card
        sport_wildcard = TRUE;
    }
    else {
        num_sports = SPE - SPS +  1;
    }

    if ((DPS == 0) && (DPE == 65535)) { // Wild card
        dport_wildcard = TRUE;
    }
    else {
        num_dports = DPE - DPS + 1;
    }

    if ((sport_wildcard == TRUE) && (dport_wildcard != TRUE)) {
        // only sport varies
        SPS = itr_get_sport_from_path_num(1, trace_type);
        SPE = itr_get_sport_from_path_num(NP/num_dports + 1, trace_type);
        num_sports = SPE - SPS + 1;
    }
    else if ((sport_wildcard != TRUE) && (dport_wildcard == TRUE)) {
        // only sport varies
        DPS = itr_get_dport_from_path_num(1, trace_type);
        DPE = itr_get_dport_from_path_num(NP/num_sports + 1, trace_type);
        num_dports = DPE - DPS + 1;
    }
    else if ((sport_wildcard == TRUE) && (dport_wildcard == TRUE)) {
        // both sport/dport vary
        np_sqrt = (int)sqrt((double)NP);
        SPS = itr_get_sport_from_path_num(1, trace_type);
        if (np_sqrt > 16382) {
            // Ephemeral ports range from 49152 - 65535 (Delta = 16383)
            np_sqrt = 16382;
        }
        SPE = itr_get_sport_from_path_num(np_sqrt + 1, trace_type);
        DPS = itr_get_dport_from_path_num(1, trace_type);
        DPE = itr_get_dport_from_path_num(np_sqrt, trace_type);
        num_sports = SPE - SPS + 1;
        num_dports = DPE - DPS + 1;
    }
    else {
        // both sport and dport fixed range provided 
    }

    if ((num_sports * num_dports) < NP ) {
        NPP = (num_sports * num_dports);
    }
    else {
        NPP = NP;
    }

    if (NPP > MAX_EXT_PATH) {
        NPP = MAX_EXT_PATH;
    }

    printf("O/P: sps(%d) spe(%d) dps(%d) dpe(%d) np(%d) npp(%d)\n", 
            SPS, SPE, DPS, DPE, NP, NPP);
    printf("=====================================================================\n");
    fflush(stdout);

    return;
}

int main()
{
    cfg_t cfg;
    int ch, rv = 0;
    FILE *fp = NULL;
    char file_line[100];
    
    printf("Read from file (y/n): ");
    ch = getc(stdin);
    do {
        if (ch  == 'y') {
            if (fp == NULL) {
                fp = fopen("range_input.txt", "r");
            }
            rv = fscanf(fp, "%d %d %d %d %d", &cfg.sps, &cfg.spe, &cfg.dps, &cfg.dpe, &cfg.np); 
            if (rv == -1) {
                printf("EOF \n");
                break;
            }
        }
        else {
            printf("Enter sps / spe / dps / dpe / np : \n");
            scanf("%d", &cfg.sps);
            scanf("%d", &cfg.spe);
            scanf("%d", &cfg.dps);
            scanf("%d", &cfg.dpe);
            scanf("%d", &cfg.np);
            printf("\n********************* Internal *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg, ITR_INT_TRACE);
            printf("\n********************* External *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg, ITR_EXT_TRACE);
            break;
        }

        printf("\n********************* Internal *********************\n");
        itr_ext_validate_get_sport_dport_range(&cfg, ITR_INT_TRACE);
        printf("\n********************* External *********************\n");
        itr_ext_validate_get_sport_dport_range(&cfg, ITR_EXT_TRACE);

        printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    } while(1);

    if (fp != NULL) {
        fclose(fp);
    }

    return 1;
}
