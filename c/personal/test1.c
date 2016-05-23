#include <stdio.h>
#include <math.h>

#define EXPN 10
#define NUM (pow(2, EXPN))
#define WC ((unsigned int) -1)

#define syserr_t int
#define uint32 int
#define uint16 int
#define MAX_ALLOWED_PATHS 101
#define TRUE 1
#define FALSE 0
#define ITR_UDP_SRC_PORT_MAGGIC_NUMBER 0xb
#define SUCCESS 0

typedef struct {
    uint32          num_paths;
    uint32          sport_start;
    uint32          sport_end;
    uint32          dport_start;
    uint32          dport_end;
    uint32          num_bts;
    uint32          bt_teps[16];

#define NP  (itr_ext_config->num_paths)
#define SPS (itr_ext_config->sport_start)
#define SPE (itr_ext_config->sport_end)
#define DPS (itr_ext_config->dport_start)
#define DPE (itr_ext_config->dport_end)

} itr_ext_config_t; 

uint16
itr_get_srcport (uint16 path_id)
{
    //const uint16 incr = 523; // a prime number
    //return (udp_srcport_base + incr * path_id)%0x0fff;
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

syserr_t
itr_ext_validate_get_sport_dport_range (itr_ext_config_t *itr_ext_config)
{
    syserr_t ret = SUCCESS;
    int sport_wildcard = FALSE, dport_wildcard = FALSE;
    uint32  num_sports = 0, num_dports = 0;
    uint32 last_port_idx = 0;
    double np_sqrt;

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
        printf("1 np %d num_dp = %d\n", NP, num_dports);
        SPS = itr_get_port_from_path_num(1);
        SPE = itr_get_port_from_path_num(NP/num_dports + 1);
        num_sports = SPE - SPS + 1;
        //num_dports = 1;
    }
    else if ((sport_wildcard != TRUE) && (dport_wildcard == TRUE)) {
        // only dport varies
        printf("2 np %d num_sp = %d\n", NP, num_sports);
        DPS = itr_get_port_from_path_num(1);
        DPE = itr_get_port_from_path_num(NP/num_sports + 1);
        //num_sports = 1;
        num_dports = DPE - DPS + 1;
    }
    else if ((sport_wildcard == TRUE) && (dport_wildcard == TRUE)) {
        // both sport/dport vary
        printf("3 np %d num_sp = %d num_dp = %d\n", NP, num_sports, num_dports);
        np_sqrt = (int)sqrt((double)NP); 
        SPS = itr_get_port_from_path_num(1);
        SPE = itr_get_port_from_path_num(np_sqrt + 1);
        DPS = itr_get_port_from_path_num(1);
        DPE = itr_get_port_from_path_num(np_sqrt);
        num_sports = SPE - SPS + 1;
        num_dports = DPE - DPS + 1;
    }
    else {
        // both sport and dport fixed range provided
    }

    printf("num_sports %d num_dports %d \n", num_sports, num_dports);
    if ((num_sports * num_dports) < NP ) {
        NP = (num_sports * num_dports);
    }

    if (NP > MAX_ALLOWED_PATHS) {
        NP = MAX_ALLOWED_PATHS;
    }

    return ret;
}

int main()
{
    int i = 2;
    int j, k = WC;
    itr_ext_config_t itr_ext_config;
    
    j = i++;
    //printf("%d j = %d ; i = %d k = %x\n", (int)NUM, j, i, k);

    itr_ext_config.num_paths = 10;
    itr_ext_config.sport_start = 1;
    itr_ext_config.sport_end = 100;
    itr_ext_config.dport_start = 0;
    itr_ext_config.dport_end = 65535;
   
    itr_ext_validate_get_sport_dport_range(&itr_ext_config);
    printf("np %d sps %d spe %d (%d) dps %d dpe %d (%d) \n", itr_ext_config.num_paths, itr_ext_config.sport_start, 
                             itr_ext_config.sport_end, itr_ext_config.sport_end - itr_ext_config.sport_start + 1, 
                             itr_ext_config.dport_start, itr_ext_config.dport_end,
                             itr_ext_config.dport_end - itr_ext_config.dport_start + 1);
 
    int sps, spe, dps, dpe, sp, dp, path_num = 0;
    int num_paths = itr_ext_config.num_paths;

    sps = itr_ext_config.sport_start;
    spe = itr_ext_config.sport_end;
    dps = itr_ext_config.dport_start;
    dpe = itr_ext_config.dport_end;

    for (dp = dps; dp <= dpe ; dp++) {
        for (sp = sps; sp <= spe ; sp++) {
            if (path_num == num_paths) {
                printf("All paths done");
                return 1;
            }

            //itr_ext_config->input->five_tuple_info->src_port = sp;
            //itr_ext_config->input->five_tuple_info->dst_port = dp;
            //itr_ext_start_probe(itr_ext_config->input);
            printf("sp: %d, dp: %d \n", sp, dp);
            path_num++;
        }
    } 

    return 1;
}
