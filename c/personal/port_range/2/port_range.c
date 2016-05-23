#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gports.h"

typedef unsigned int uint16;
typedef unsigned short bool;

#define FALSE 0
#define TRUE 1
#define ITR_UDP_SRC_PORT_MAGGIC_NUMBER 0xb
#define MAX_EXT_PATH 100000

typedef struct cfg_ {
    ushort sps;
    ushort spe;
    ushort dps;
    ushort dpe;
    ushort np;
    unsigned int npp;
}cfg_t;

#define SPS (cfg->sps)
#define SPE (cfg->spe)
#define DPS (cfg->dps)
#define DPE (cfg->dpe)
#define NP  (cfg->np)
#define NPP (cfg->npp)

#define NSPS (cfg_new->sps)
#define NSPE (cfg_new->spe)
#define NDPS (cfg_new->dps)
#define NDPE (cfg_new->dpe)
#define NNP  (cfg_new->np)
#define NNPP (cfg_new->npp)

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
 * INT_DPS 16384 = 0100 0000 0000 0000 (0x040 00)
 * INT_DPE 24575 = 0101 1111 1111 1111 (0x05F FF)
 * INT_SPS 24576 = 0110 0000 0000 0000 (0x060 00)
 * INT_SPE 32767 = 0111 1111 1111 1111 (0x07F FF)
 *
 * EXT_DPS 32768 = 1000 0000 0000 0000 (0x080 00) 
 * EXT_DPE 49151 = 1011 1111 1111 1111 (0x0BF FF)
 * EXT_SPS 49152 = 1100 0000 0000 0000 (0x0C0 00)
 * EXT_SPE 65535 = 1111 1111 1111 1111 (0x0FF FF)
 *
 */

#define INT_DPS_MAGIC_NUM (0x04)
#define INT_DPE_MAGIC_NUM (0x05FFF)

#define INT_SPS_MAGIC_NUM (0x06)
#define INT_SPE_MAGIC_NUM (0x07FFF)

#define EXT_DPS_MAGIC_NUM (0x08)
#define EXT_DPE_MAGIC_NUM (0x0BFFF)

#define EXT_SPS_MAGIC_NUM (0x0C)
#define EXT_SPE_MAGIC_NUM (0x0FFFF)

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
  8.1 WC / WC
  8.2 F  / F
  8.3 R  / R
  8.4 WC / F
  8.5 WC / R
  8.6 F  / R

  9. While selecting ports, the following needs to be satisfied
  9.1 Fixed SPS/SPE/DPS/DPE values – we use those values. Only 1 path 
      is validated.
  9.2 Fixed SPS/SPE/DPS/DPE range – we pick up SPS/SPE and DPS/DPE ports from 
      the range for the required number of paths
  9.3 Fixed SP values / fixed range of DP – same as above except SP values are 
      fixed. Number of paths depends on  the DP range
  9.4 Fixed DP values/ fixed range of SP – similar to (3)
  9.5 Fixed SP values / wildcard DP – similar to (3) except we pick up DP 
      values from one of ephemeral ports range ( > ~45057 and < 65535 ) instead 
      of a fixed range
  9.6 Fixed DP value / wildcard SP – similar to (5)
  9.7 Fixed DP range / wildcard SP – we try to pick up as many ports from the DP 
      range as possible to cater to the number of paths requested. Remaining 
      paths are taken care of by ephemeral ports explained in (5)
  9.8 Fixed SP range / wildcard DP – similar to (7)
  9.9 Wildcard SP / Wildcard DP  - we pick up both SP and DP values from ephemeral 
      port range

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

    if (path_id == 65535) {
        switch (magic_num) {
            case INT_DPS_MAGIC_NUM:
                mdprintf("case INT_DPE. returning %x", INT_DPE_MAGIC_NUM);
                return INT_DPE_MAGIC_NUM;
            case INT_SPS_MAGIC_NUM:
                mdprintf("case INT_SPE. returning %x", INT_SPE_MAGIC_NUM);
                return INT_SPE_MAGIC_NUM;
            case EXT_DPS_MAGIC_NUM:
                mdprintf("case EXT_DPE. returning %x", EXT_DPE_MAGIC_NUM);
                return EXT_DPE_MAGIC_NUM;
            case EXT_SPS_MAGIC_NUM:
                mdprintf("case EXT_SPE. returning %x", EXT_SPE_MAGIC_NUM);
                return EXT_SPE_MAGIC_NUM;
                break;
            default:
                return 0;
        }
    }
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
itr_print_cfg(cfg_t *cfg)
{
    mprintf("\t\t\tSPS: %d SPE: %d DPS: %d DPE: %d NP: %d \n",
            SPS, SPE, DPS, DPE, NP);
}

int
itr_check_if_enough_paths(ushort sps, ushort spe, ushort dps, ushort dpe,
                           unsigned int num_paths)
{
    if ( ((spe - sps + 1) * (dpe - dps + 1)) < num_paths) {
        return 0;
    } 

    return 1;
}

#define ASSIGN_PORTS(a, a_val, b, b_val, c, c_val, d, d_val) {\
    (a) = (a_val);\
    (b) = (b_val);\
    (c) = (c_val);\
    (d) = (d_val);\
    mdprintf("assigning sps %d spe %d dps %d dpe %d", \
              a_val, b_val, c_val, d_val);\
}

#define CHECK_ASSIGN_AND_RET(sps, spe, dps, dpe) {\
    if ( ((spe - sps + 1) * (dpe - dps + 1)) >= np) {\
        mdprintf("(spe - sps + 1) = %d (dpe - dps + 1) = %d np = %d", \
                 (spe - sps + 1), (dpe - dps + 1), np);\
            ASSIGN_PORTS(*fsps, sps, *fspe, spe, \
                         *fdps, dps, *fdpe, dpe); \
        return;\
    }\
}

void
itr_check_if_paths_possible(ushort osps, ushort ospe, ushort odps, ushort odpe, 
                            ushort csps, ushort cspe, ushort cdps, ushort cdpe, 
                            uint32 np, uint32 leeway_1, uint32 leeway_2,
                            ushort *fsps, ushort *fspe, ushort *fdps, ushort *fdpe)
{
    uint32 cnsp, cndp, onsp, ondp;

    onsp = ospe - osps + 1;
    ondp = odpe - odps + 1;

    if ((onsp > leeway_1) && (ondp > leeway_2)) {
        mdprintf(" <1> ");
        CHECK_ASSIGN_AND_RET(csps, cspe, cdps, cdpe);
    } 

    if ((onsp > leeway_1) && (ondp <= leeway_2)) {
        mdprintf(" <2> ");
        CHECK_ASSIGN_AND_RET(csps, cspe, odps, odpe);
    }

    if ((onsp <= leeway_1) && (ondp > leeway_2)) {
        mdprintf(" <3> ");
        CHECK_ASSIGN_AND_RET(osps, ospe, cdps, cdpe);
    }

    // No change
    mprintf("Assigning defaults");
    ASSIGN_PORTS(*fsps, osps, *fspe, ospe, *fdps, odps, *fdpe, odpe);

   return;
}

void
itr_check_if_ranges_intersect_get_new_range(cfg_t *cfg, cfg_t *cfg_new)
{
    int num_sp, num_dp, delta;
    int leeway_1 = 0, leeway_2 = 0, leeway_needed;
    double np_sqrt, spdp_path_sqrt;

    num_sp      = SPE - SPS + 1;
    num_dp      = DPE - DPS + 1;
    delta       = (num_sp * num_dp) - NP;
    memcpy(cfg_new, cfg, sizeof(cfg_t));
    if (delta > 0) { 
        spdp_path_sqrt  = (int)sqrt((double)(num_sp * num_dp));
        np_sqrt         = (int)sqrt((double)NP); 
        np_sqrt         = np_sqrt + 1;
        leeway_1        = leeway_2 = (spdp_path_sqrt - np_sqrt);
        mdprintf("delta %d spdp_paths %d spdp_path_sqrt %d np %d "
                 "np_sqrt %d leeway_1 %d", delta, (num_sp * num_dp), 
                 (int)spdp_path_sqrt, NP, (int)np_sqrt, leeway_1);
    }
    else {
        mprintf("No change possible ");
        return;
    }

    mprintf("\t\t\t ---------------- cfg----------------  \n");
    itr_print_cfg(cfg);
    mdprintf("\t\t\t-------------------------------------\n");

    // Note - we cannot shrink SPS/DPS or push SPE/DPE
    // (1) SPS ------------------ SPE         
    //Ref line >>>>>   DPS ----------------------------- DPE
    if ((SPS <= DPS) && (SPE >= DPS) && (SPE <= DPE)) {
        mdprintf("\t\t\t Case 1\n");
        // Try to move DPS beyond SPE:
        leeway_needed = (SPE - DPS + 1);
        mdprintf("\t\t\t Case 1 leeway_1 %d leeway_2 %d leeway_needed %d \n",
                leeway_1, leeway_2, leeway_needed);
        if ((leeway_1 + leeway_2) > leeway_needed) {
            leeway_1 = leeway_needed/2;
            leeway_2 = leeway_needed - leeway_1;
        }
        // shrink SPE by leeway_1 and push DPS by leeway_2
        itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                            SPS, (SPE - leeway_1), (DPS + leeway_2), DPE, 
                            NP, leeway_1, leeway_2,
                            &cfg_new->sps, &cfg_new->spe, 
                            &cfg_new->dps, &cfg_new->dpe); 

    }
    // (2)                                   SPS ---------------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS >= DPS) && (SPS <= DPE) && (SPE >= DPE)) {
        mdprintf("\t\t\t Case 2\n");
        // Try to move SPS beyond DPE:
        leeway_needed = (DPE - SPS + 1);
        if ((leeway_1 + leeway_2) > leeway_needed) {
            leeway_1 = leeway_needed/2;
            leeway_2 = leeway_needed - leeway_1;
        }
        // push SPS by leeway_1 and shrink DPE by leeway_2
        itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                            (SPS + leeway_1), SPE, DPS, (DPE - leeway_2), 
                            NP, leeway_1, leeway_2,
                            &cfg_new->sps, &cfg_new->spe, 
                            &cfg_new->dps, &cfg_new->dpe); 

    }
    // (3)                      SPS ------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS >= DPS) && (SPE <= DPE)) {
        mdprintf("\t\t\t Case 3\n");
        // Make the call based on (SPE - DPS) < (DPE - SPS)
        if ((SPE - DPS) < (DPE - SPS)) {
            leeway_needed = (SPE - DPS + 1);
            mdprintf("\t\t\t Case 3.1 leeway_1 %d leeway_2 %d leeway_needed %d \n", 
                    leeway_1, leeway_2, leeway_needed);

            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // shrink SPE by leeway_1 and push DPS by leeway_2
            itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                                SPS, (SPE - leeway_1), (DPS + leeway_2), DPE, 
                                NP, leeway_1, leeway_2,
                                &cfg_new->sps, &cfg_new->spe, 
                                &cfg_new->dps, &cfg_new->dpe); 
        }
        else {
            leeway_needed = (DPE - SPS + 1);
            mdprintf("\t\t\t Case 3.2 leeway_1 %d leeway_2 %d leeway_needed %d \n", 
                    leeway_1, leeway_2, leeway_needed);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }

            // push SPS by leeway_1 and shrink DPE by leeway_2
            itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                                SPS + leeway_1, SPE, DPS, DPE - leeway_2, 
                                NP, leeway_1, leeway_2,
                                &cfg_new->sps, &cfg_new->spe, 
                                &cfg_new->dps, &cfg_new->dpe); 

        }
    }
    // (4)  SPS --------------------------------------------------- SPE
    //Ref line >>>>>   DPS ----------------------------- DPE
    else if ((SPS <= DPS) && (SPE >= DPE)) {
        mdprintf("\t\t\t Case 4\n");
        // Make the call based on (DPE - SPS) vs (SPE - DPS)
        if ((DPE - SPS) < (SPE - DPS)) {
            mdprintf("\t\t\t\t Case 4.1\n");
            leeway_needed = (DPE - SPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // push SPS by leeway_1 and shrink DPE by leeway_2
            itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                                (SPS + leeway_1), SPE, DPS, (DPE - leeway_2), 
                                NP, leeway_1, leeway_2,
                                &cfg_new->sps, &cfg_new->spe, 
                                &cfg_new->dps, &cfg_new->dpe); 

        }
        else {
            mdprintf("\t\t\t\t Case 4.2\n");
            leeway_needed = (SPE - DPS + 1);
            if ((leeway_1 + leeway_2) > leeway_needed) {
                leeway_1 = leeway_needed/2;
                leeway_2 = leeway_needed - leeway_1;
            }
            // shrink SPE by leeway_1 and push DPS by leeway_2
            itr_check_if_paths_possible(SPS, SPE, DPS, DPE, 
                                SPS, (SPE - leeway_1), (DPS + leeway_2), DPE, 
                                NP, leeway_1, leeway_2,
                                &cfg_new->sps, &cfg_new->spe, 
                                &cfg_new->dps, &cfg_new->dpe); 

        }
    }
    else {
        // Error
        mprintf("No intersection detected. Not changing anything \n");
        memcpy(cfg_new, cfg, sizeof(cfg_t));
    }

    mprintf("\t\t\t -------------- cfg_new --------------  \n");
    itr_print_cfg(cfg_new);
    mdprintf("\t\t\t-------------------------------------\n");
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
itr_ext_validate_get_sport_dport_range (cfg_t *cfg, cfg_t *cfg_new,
                                        trace_type_t trace_type)
{
    bool sport_wildcard = FALSE, dport_wildcard = FALSE;
    bool sport_range = FALSE, dport_range = FALSE;
    unsigned int  num_sports = 0, num_dports = 0;

    memset(cfg_new, 0, sizeof(cfg_t));
    mdprintf("===============================================================\n");
    mdprintf("I/P (type %d): sps(%d) spe(%d) dps(%d) dpe(%d) np(%d)", 
            trace_type, SPS, SPE, DPS, DPE, NP);
    mdprintf("---------------------------------------------------------------\n");

    if ((SPS == 0) && (SPE == 65535)) { // Wild card
        sport_wildcard = TRUE;
    } else if (SPS != SPE) {
        sport_range = TRUE;
        num_sports = SPE - SPS +  1;
    } else {
        num_sports = SPE - SPS +  1;
    }

    if ((DPS == 0) && (DPE == 65535)) { // Wild card
        dport_wildcard = TRUE;
    } else if (DPS != DPE) {
        dport_range = TRUE;
        num_dports = DPE - DPS + 1;
    } else {
        num_dports = DPE - DPS + 1;
    }

    mdprintf("sp_wc: %d sp_rg: %d dp_wc: %d dp_rg: %d num_sp: %d num_dp: %d\n",
            sport_wildcard, sport_range, dport_wildcard, dport_range, 
            num_sports, num_dports);

    itr_print_cfg(cfg);

    if ((sport_wildcard == TRUE) && (dport_wildcard != TRUE)) {
        // only sport varies
        SPS = itr_get_sport_from_path_num(0, trace_type);
        //SPE = itr_get_sport_from_path_num(NP/num_dports + 1, trace_type);
        SPE = itr_get_sport_from_path_num(65535, trace_type);
        mdprintf("Only sport varies SPS %d SPE %d DPS %d DPE %d",
                 SPS, SPE, DPS, DPE);
        itr_check_if_ranges_intersect_get_new_range(cfg, cfg_new);
        num_sports = SPE - SPS + 1;
    }
    else if ((sport_wildcard != TRUE) && (dport_wildcard == TRUE)) {
        // only sport varies
        DPS = itr_get_dport_from_path_num(0, trace_type);
        //DPE = itr_get_dport_from_path_num(NP/num_sports + 1, trace_type);
        DPE = itr_get_dport_from_path_num(65535, trace_type);
        mdprintf("Only dport varies SPS %d SPE %d DPS %d DPE %d", 
                 SPS, SPE, DPS, DPE);
        itr_check_if_ranges_intersect_get_new_range(cfg, cfg_new);
        num_dports = DPE - DPS + 1;
    }
    else if ((sport_wildcard == TRUE) && (dport_wildcard == TRUE)) {
        // both sport/dport vary
        SPS = itr_get_sport_from_path_num(0, trace_type);
        SPE = itr_get_sport_from_path_num(65535, trace_type);
        DPS = itr_get_dport_from_path_num(0, trace_type);
        DPE = itr_get_dport_from_path_num(65535, trace_type);
        mdprintf("Both sport/dport vary SPS %d SPE %d DPS %d DPE %d\n", 
                 SPS, SPE, DPS, DPE);
        itr_check_if_ranges_intersect_get_new_range(cfg, cfg_new);
        num_sports = SPE - SPS + 1;
        num_dports = DPE - DPS + 1;
    }
    else {
        // both sport and dport fixed range provided 
        itr_check_if_ranges_intersect_get_new_range(cfg, cfg_new);
    }

    if ((num_sports * num_dports) < NP ) {
        NNPP = NPP = (num_sports * num_dports);
    }
    else {
        NNPP = NPP = NP;
    }

    if (NPP > MAX_EXT_PATH) {
        NNPP = NPP = MAX_EXT_PATH;
    }

    mdprintf("O/P: sps(%d) spe(%d) dps(%d) dpe(%d) np(%d) npp(%d) \n", 
            cfg_new->sps, cfg_new->spe, cfg_new->dps, cfg_new->dpe, 
            cfg_new->np, cfg_new->npp);
    mdprintf("==============================================================\n");
    fflush(stdout);

    return;
}


void 
itr_ext_validate_get_sport_dport_range_best_blocks (cfg_t *cfg_int, 
                                        cfg_t *cfg_new, trace_type_t trace_type)
{
    ushort block_start, block_end;
    ushort block_start2, block_end2;
    ushort pmask_count = 0, pmask_count2 = 0;
    port_mask_t pmask[16], pmask2[16]; 
    int i, num_sports, num_dports;
    double np_sqrt;

    itr_ext_validate_get_sport_dport_range(cfg_int, cfg_new, trace_type);

    num_sports  = NSPE - NSPS + 1;
    num_dports  = NDPE - NDPS + 1;
    np_sqrt     = (int)sqrt((double)NNPP); 

    if (num_sports < np_sqrt) {
        // Sport Range: No-Op - num_sports remains unchanged
        block_start = NSPS;
        block_end   = NSPE;
    } else {
        pmask_count = get_best_block(cfg_new->sps, cfg_new->spe, pmask, 
                                     np_sqrt, &block_start, &block_end);          
        num_sports = block_end - block_start + 1;
    }

    if (num_dports < np_sqrt) {
        // Dport Range: No-Op - num_sdorts remains unchanged
        // Sport range to cater for deficiency of dport range
        block_start2 = NDPS;
        block_end2   = NDPE;
        pmask_count = get_best_block(cfg_new->sps, cfg_new->spe, pmask, 
                            (NNPP/num_dports + 1), &block_start, &block_end);          

    } else {
        pmask_count2 = get_best_block(cfg_new->dps, cfg_new->dpe, pmask2, 
                            (NNPP/num_sports + 1), &block_start2, &block_end2);          
    }

    NSPS = block_start;
    NSPE = block_end;
    NDPS = block_start2;
    NDPE = block_end2;
    
    mprintf ("NSPS %d NSPE %d (%d) NDPS %d NDPE %d (%d) cfg_new->np %d", 
                NSPS, NSPE, NSPE - NSPS + 1, NDPS, NDPE, NDPE - NDPS + 1, NNPP);

    mprintf ("Source Pmask:");
    for (i = 0; i < pmask_count; ++i) {
        mprintf ("( %d / %d ) ", pmask[i].port, pmask[i].mask);
    }

    mdprintf ("Dest Pmask:");
    for (i = 0; i < pmask_count2; ++i) {
        mprintf ("( %d / %d ) ", pmask2[i].port, pmask2[i].mask);
    }

    mprintf("\n");

}

int main()
{
    cfg_t   cfg_int = {0}, cfg_ext = {0}, cfg_new;
    int     ch, rv = 0;
    size_t  len;
    int     read;
    FILE    *fp = NULL;
    char    *line = NULL;
    
    mdprintf("Read from file (y/n): ");
    //ch = getc(stdin);
    ch = 'y';
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
            if (read == -1) {
                exit(0);
            }
            if (strstr(line, "#") == NULL) {

                rv = sscanf(line, "%hu %hu %hu %hu %hu", 
                            &cfg_int.sps, &cfg_int.spe, &cfg_int.dps, 
                            &cfg_int.dpe, &cfg_int.np); 
                if (rv == -1) {
                    mdprintf("EOF \n");
                    break;
                }
            }
            else {
                mdprintf("...... skipping line: %s \n", line);
                continue;
            }

            memcpy(&cfg_ext, &cfg_int, sizeof(cfg_t));
            mprintf("\n\n");
            //mprintf("********************* Internal *********************");
            //itr_ext_validate_get_sport_dport_range_best_blocks(&cfg_int, 
            //                                        &cfg_new, ITR_INT_TRACE);
            //mprintf("********************* External *********************");
            itr_ext_validate_get_sport_dport_range_best_blocks(&cfg_ext, 
                                                    &cfg_new, ITR_EXT_TRACE);

        }
        else {
            printf("Enter sps / spe / dps / dpe / np : \n");
            scanf("%hu", &cfg_int.sps);
            scanf("%hu", &cfg_int.spe);
            scanf("%hu", &cfg_int.dps);
            scanf("%hu", &cfg_int.dpe);
            scanf("%hu", &cfg_int.np);
            memcpy(&cfg_ext, &cfg_int, sizeof(cfg_t));
            mprintf("********************* Internal *********************");
            itr_ext_validate_get_sport_dport_range_best_blocks(&cfg_int, 
                                                    &cfg_new, ITR_INT_TRACE);
            mprintf("********************* External *********************");
            itr_ext_validate_get_sport_dport_range_best_blocks(&cfg_ext, 
                                                    &cfg_new, ITR_EXT_TRACE);
            break;
        }

    } while(1);

    if (fp != NULL) {
        fclose(fp);
    }

    return 1;
}
