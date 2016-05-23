#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef unsigned int uint16;
typedef unsigned short bool;

#define FALSE 0
#define TRUE 1
#define ITR_UDP_SRC_PORT_MAGGIC_NUMBER 0xb
#define MAX_EXT_PATH 256

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

typedef enum {
    ITR_INT_TRACE = 1,
    ITR_EXT_TRACE
} trace_type_t; 

/*
 *
 * If wildcard:
 * (A) Internal trace port range: 
 * Dport range - 16384 - 24575
 * Sport range - 24576 - 32767
 * (B) External trace port range: 
 * Dport range - 32768 - 49151
 * Sport range - 49152 - 65535
 *
 * INT_DPS 16384 = 0100 0000 0000 0000 (0x04 00 00 00)
 * INT_DPE 24575 = 0101 1111 1111 1111 (0x05 FF FF FF)
 * INT_SPS 24576 = 0110 0000 0000 0000 (0x06 00 00 00)
 * INT_SPE 32767 = 0111 1111 1111 1111 (0x07 FF FF FF)
 *
 * EXT_DPS 32768 = 1000 0000 0000 0000 (0x08 00 00 00) 
 * EXT_DPE 49151 = 1011 1111 1111 1111 (0x0B FF FF FF)
 * EXT_SPS 49152 = 1100 0000 0000 0000 (0x0C 00 00 00)
 * EXT_SPE 65535 = 1111 1111 1111 1111 (0x0F FF FF FF)
 *
 */

#define INT_DPS_MAGIC_NUM (0x04)
#define INT_SPS_MAGIC_NUM (0x06)
#define EXT_DPS_MAGIC_NUM (0x08)
#define EXT_SPS_MAGIC_NUM (0x0c)

 /*
  *********
  * Notes:
  *********
  1. Source and destination will come from different ranges.
  2. Internal and External will work at different set of ports (Care to be taken 
     because there are port based rules already today for internal)
  3. The range selection will work for both full wildcard and when ranges are 
     given and avoid overlap as much as possible.
  4. We will try and assign Dest ports from lower range and src port from higher 
     range so that we reduce clash with user traffic.
  5. Range calculation will be deterministic so as to avoid co-ordination with 
     VPC peer.
  6. There is a secondary goal of # of rules optimization so that most of the 
     rules end up on binary boundaries:
     6.1 First count(say n) the number of bits in computed sps / dps
     6.2 (1 << n) is the next power of 2 for sps / dps
     6.3 Check if selecting the next power of 2 still keeps us in the range
  7. Validate the results:
     - if the result is say sps/spe and dps/dpe
     - we would be installing rules by flipping sp with dp and vice versa
     - so any intersection between sp and dp ranges, would cause issue
     - Having a result of the following form is not good:
       sps(100) .............. spe(200)
                    dps(150) ............ dpe(250) 
    - Possible cases with 4 points through 2 ranges to intersect :

     (1) SPS ------------------ SPE         
     
     (2)                                   SPS ---------------------- SPE
                     
    Ref line >>>>>   DPS ----------------------------- DPE

     (3)                      SPS ------------- SPE

     (4)  SPS --------------------------------------------------- SPE
    


  8. Port isolation takes priority over finding the next power of 2
  *
  *
  *******************************************************************************
  * Cases for both internal and external (WC = wildcard ; F = Fixed ; R = Range):
  *******************************************************************************
  1. WC / WC
  2. F  / F
  3. R  / R
  4. WC / F
  5. WC / R
  6. F  / R
  *
  */

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

uint16
itr_get_num_bits(uint16 n)
{
    uint16 count = 0;

    while(n) {
        count++;
        n >>= 1;
    }

    return count;
}

uint16 itr_check_if_num_pow_of_2(uint16 n)
{
    return (!(n & (n-1)));
}

uint16
itr_get_next_power_of_2(uint16 num)
{
    if (itr_check_if_num_pow_of_2(num)) {
        return num;
    }
    else {
        return (1 << itr_get_num_bits(num));
    }
}

void
itr_check_if_ranges_intersect_get_new_range(cfg_t *cfg, cfg_t *cfg_new)
{
#define NSPS (cfg_new->sps)
#define NSPE (cfg_new->spe)
#define NDPS (cfg_new->dps)
#define NDPE (cfg_new->dpe)
#define NNP  (cfg_new->np)

    int num_sp, num_dp, delta;
    int leeway_1 = 0, leeway_2 = 0, leeway_needed;
    double delta_sqrt;

    num_sp      = SPE - SPS + 1;
    num_dp      = DPE - DPS + 1;
    delta       = (num_sp * num_dp) - NP;
    if (delta > NP) { 
        delta_sqrt  = (int)sqrt((double)delta); 
        leeway_1 = delta_sqrt/2;
        leeway_2 = delta_sqrt - leeway_1;
    }
    else {
        printf("No change possible \n");
        return;
    }

    // Note - we cannot shrink SPS/DPS or push SPE/DPE
    // (1) SPS ------------------ SPE         
    //Ref line >>>>>   DPS ----------------------------- DPE
    if ((SPS <= DPS) && (SPE >= DPS) && (SPE <= DPE)) {
        // Try to move DPS beyond SPE:
        leeway_needed = (SPE - DPS + 1);
        if ((leeway_1 + leeway_2) > leeway_needed) {
            leeway_1 = leeway_needed/2;
            leeway_2 = leeway_needed - leeway_1;
        }
        // shrink SPE by leeway_1 and push DPS by leeway_2
        NSPS = SPS;
        if ( (SPE - SPS + 1) > leeway_1) {
            NSPE = SPE - leeway_1;
        }
        if ( (DPE - DPS + 1) > leeway_2) {
            NDPS = DPS + leeway_2;
        }
        NDPE = DPE;
    }
    // (2)                                   SPS ---------------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS >= DPS) && (SPS <= DPE) && (SPE >= DPE)) {
        // Try to move SPS beyond DPE:
        leeway_needed = (DPE - SPS + 1);
        if ((leeway_1 + leeway_2) > leeway_needed) {
            leeway_1 = leeway_needed/2;
            leeway_2 = leeway_needed - leeway_1;
        }
        // push SPS by leeway_1 and shrink DPE by leeway_2
        if ( (SPE - SPS + 1) > leeway_1) {
            NSPS = SPS + leeway_1;
        }
        NSPE = SPE;
        NDPS = DPS;
        if ( (DPE - DPS + 1) > leeway_2) {
            NDPE = DPE - leeway_2;
        }
    }
    // (3)                      SPS ------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS >= DPS) && (SPE <= DPE)) {
        // Make the call based on (SPE - DPS) < (DPE - SPS)
        if ((SPE - DPS) < (DPE - SPS)) {
            leeway_needed = (SPE - DPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // shrink SPE by leeway_1 and push DPS by leeway_2
            NSPS = SPS;
            if ( (SPE - SPS + 1) > leeway_1) {
                NSPE = SPE - leeway_1;
            }
            if ( (DPE - DPS + 1) > leeway_2) {
                NDPS = DPS + leeway_2;
            }
            NDPE = DPE;

        }
        else {
            leeway_needed = (DPE - SPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // push SPS by leeway_1 and shrink DPE by leeway_2
            if ( (SPE - SPS + 1) > leeway_1) {
                NSPS = SPS + leeway_1;
            }
            NSPE = SPE;
            NDPS = DPS;
            if ( (DPE - DPS + 1) > leeway_2) {
                NDPE = DPE - leeway_2;
            }
        }
    }
    // (4)  SPS --------------------------------------------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS <= DPS) && (SPE >= DPE)) {
        // Make the call based on (DPE - SPS) vs (SPE - DPS)
        if ((DPE - SPS) < (SPE - DPS)) {
            leeway_needed = (DPE - SPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // push SPS by leeway_1 and shrink DPE by leeway_2
            if ( (SPE - SPS + 1) > leeway_1) {
                NSPS = SPS + leeway_1;
            }
            NSPE = SPE;
            NDPS = DPS;
            if ( (DPE - DPS + 1) > leeway_2) {
                NDPE = DPE - leeway_2;
            }
        }
        else {
            leeway_needed = (SPE - DPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // shrink SPE by leeway_1 and push DPS by leeway_2
            NSPS = SPS;
            if ( (SPE - SPS + 1) > leeway_1) {
                NSPE = SPE - leeway_1;
            }
            if ( (DPE - DPS + 1) > leeway_2) {
                NDPS = DPS + leeway_2;
            }
            NDPE = DPE;

        }
    }
    else {
        // Error
        printf("somethign is wrong \n");
    }
}

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

void
itr_ext_validate_get_sport_dport_range (cfg_t *cfg, trace_type_t trace_type)
{
    bool sport_wildcard = FALSE, dport_wildcard = FALSE;
    bool sport_range = FALSE, dport_range = FALSE;
    unsigned int  num_sports = 0, num_dports = 0;
    double np_sqrt;
    cfg_t cfg_new = {0};

    printf("=====================================================================\n");
    printf("I/P (type %d): sps(%d) spe(%d) dps(%d) dpe(%d) np(%d) \n", 
            trace_type, SPS, SPE, DPS, DPE, NP);
    printf("---------------------------------------------------------------------\n");

    if ((SPS == 0) && (SPE == 65535)) { // Wild card
        sport_wildcard = TRUE;
    } else if (SPS != SPE) {
        sport_range = TRUE;
    } else {
        num_sports = SPE - SPS +  1;
    }

    if ((DPS == 0) && (DPE == 65535)) { // Wild card
        dport_wildcard = TRUE;
    } else if (DPS != DPE) {
        dport_range = TRUE;
    } else {
        num_dports = DPE - DPS + 1;
    }

    printf("sp_wc: %d sp_rg: %d dp_wc: %d dp_rg: %d num_sp: %d num_dp: %d\n",
            sport_wildcard, dport_wildcard, 
            sport_range, dport_range, 
            num_sports, num_dports);

    if ((sport_wildcard == TRUE) && (dport_wildcard != TRUE)) {
        // only sport varies
        SPS = itr_get_sport_from_path_num(1, trace_type);
        SPE = itr_get_sport_from_path_num(NP/num_dports + 1, trace_type);
        itr_check_if_ranges_intersect_get_new_range(cfg, &cfg_new);
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
    cfg_t cfg_int = {0}, cfg_ext = {0};
    int ch, rv = 0;
    size_t len, read;
    FILE *fp = NULL;
    char *line = NULL;
    
    printf("Read from file (y/n): ");
    ch = getc(stdin);
    do {
        if (ch  == 'y') {
            if (fp == NULL) {
                fp = fopen("range_input.txt", "r");
            }

            if (line != NULL) {
                free(line);
            }

            line = NULL;
            len = 0;

            read = getline(&line, &len, fp); 
            if (strstr(line, "#") == NULL) {

                rv = sscanf(line, "%d %d %d %d %d", 
                            &cfg_int.sps, &cfg_int.spe, &cfg_int.dps, 
                            &cfg_int.dpe, &cfg_int.np); 
                if (rv == -1) {
                    printf("EOF \n");
                    break;
                }
            }
            else {
                printf("...... skipping line: %s \n", line);
                continue;
            }

            memcpy(&cfg_ext, &cfg_int, sizeof(cfg_t));
            printf("\n********************* Internal *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg_int, ITR_INT_TRACE);
            printf("\n********************* External *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg_ext, ITR_EXT_TRACE);

        }
        else {
            printf("Enter sps / spe / dps / dpe / np : \n");
            scanf("%d", &cfg_int.sps);
            scanf("%d", &cfg_int.spe);
            scanf("%d", &cfg_int.dps);
            scanf("%d", &cfg_int.dpe);
            scanf("%d", &cfg_int.np);
            memcpy(&cfg_ext, &cfg_int, sizeof(cfg_t));
            printf("\n********************* Internal *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg_int, ITR_INT_TRACE);
            printf("\n********************* External *********************\n");
            itr_ext_validate_get_sport_dport_range(&cfg_ext, ITR_EXT_TRACE);
            break;
        }

        printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

    } while(1);

    if (fp != NULL) {
        fclose(fp);
    }

    return 1;
}
