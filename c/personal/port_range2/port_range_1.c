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
    return (1000 + path_id) % 0x0fff;
}

uint16 itr_get_port_from_path_num(uint16 path_id)
{
    uint16 src_port, running_part;

    running_part = itr_get_srcport(path_id);

    src_port = ITR_UDP_SRC_PORT_MAGGIC_NUMBER;
    src_port = src_port << 12;
    src_port = src_port & 0xf000;
    src_port = src_port | running_part;

    return src_port;
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
itr_ext_validate_get_sport_dport_range (cfg_t *cfg)
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
        SPS = itr_get_port_from_path_num(1);
        SPE = itr_get_port_from_path_num(NP/num_dports + 1);
        num_sports = SPE - SPS + 1;
    }
    else if ((sport_wildcard != TRUE) && (dport_wildcard == TRUE)) {
        // only sport varies
        DPS = itr_get_port_from_path_num(1);
        DPE = itr_get_port_from_path_num(NP/num_sports + 1);
        num_dports = DPE - DPS + 1;
    }
    else if ((sport_wildcard == TRUE) && (dport_wildcard == TRUE)) {
        // both sport/dport vary
        np_sqrt = (int)sqrt((double)NP);
        SPS = itr_get_port_from_path_num(1);
        if (np_sqrt > 16382) {
            // Ephemeral ports range from 49152 - 65535 (Delta = 16383)
            np_sqrt = 16382;
        }
        SPE = itr_get_port_from_path_num(np_sqrt + 1);
        DPS = itr_get_port_from_path_num(1);
        DPE = itr_get_port_from_path_num(np_sqrt);
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
            itr_ext_validate_get_sport_dport_range(&cfg);
            break;
        }

        itr_ext_validate_get_sport_dport_range(&cfg);
    } while(1);

    if (fp != NULL) {
        fclose(fp);
    }

    return 1;
}
