#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gports.h"



void usage(char *prog, char *error) {
  printf("Usage: <sport_start> <sport_end> <dport_start> <dport_end> <count>\n");
  exit(-1);
}

ushort get_block_size(ushort number, ushort *bits)
{
  ushort mask = 0x8000;

  *bits = 0;
  if (number <= 0) return 0;

  *bits = 15;
  while (mask) {
   if (mask <= number) {
       mdprintf("number %d mask %d bits %d ", number, mask, *bits);
       return mask;
   }
   mask = mask >> 1;
   (*bits)--;
  }

  return mask;
}

ushort get_largest_block_in_range(ushort start, ushort end, ushort block,
                                  ushort bits, ushort *block_start)
{
  ushort mask = 1, shifts = 1, bstart;

  if ((end - start) < block) {
      return 0;
  }

  while (shifts != bits ) {
    mask = (mask << 1) | 1;
    shifts ++;
  }
  bstart = start;

  bstart = bstart >> bits;
  bstart = bstart << bits;
  mdprintf("bstart %d", bstart);
  if (bstart != start) bstart = bstart | block;
  if (bstart < start) bstart = bstart + block;

  while ((bstart | mask) > end) {
    mdprintf("");
    bstart = start;
    mask = mask >> 1;  
    block = block >> 1;
    bits--;
    bstart = bstart >> bits;
    bstart = bstart << bits;
    if (bstart != start) bstart = bstart | block;
    if (bstart < start) bstart = bstart + block;
  }
  *block_start = bstart;

  return block;
}

ushort  // Port mask count;
get_best_block(ushort start, ushort end, port_mask_t *pmask, ushort count,
               ushort *bstart, ushort *bend)
{
  ushort block, bits, block_start, block_end;
  ushort pmask_count = 0, total = 0;

  mdprintf("Arguments start: %d end: %d count: %d", start, end, count); 

  block = get_block_size(count, &bits);

  total = get_largest_block_in_range(start, end, block, bits, &block_start);
  mdprintf("port %d mask %d", block_start, total);
  pmask[pmask_count].port = block_start;
  pmask[pmask_count].mask = total;
  pmask_count++;
  block_end = block_start + total;

  block = get_block_size((count - total), &bits);  
  while ((total < count) && (block > 0)) {
    //mdprintf("...total %d count %d block %d", total, count, block);
    //mdprintf("...block_start %d start %d", block_start, start);
    while (((block_start - block) > start) && (total < count)) {
      block_start = block_start - block;
      total += block;
      pmask[pmask_count].port = block_start;
      pmask[pmask_count++].mask = block;
    }
    while (((block_end + block) < end) && (total < count)) {
      block_end = block_end + block;
      total += block;
      pmask[pmask_count].port = block_end - block;
      pmask[pmask_count++].mask = block;
    }
    block = block >> 1;
  } 
  *bstart = block_start;
  *bend = block_end;
  return pmask_count; 
}

#if 0
int main(int argc, char **argv)
{
  ushort block_start, block_end;
  ushort sps, spe, dps, dpe, count, pmask_count = 0;
  port_mask_t pmask[16]; 
  int i;

  if (argc < 6) {
    usage(argv[0], "needs 6 arguments\n");
  }

  sps = atoi(argv[1]);
  spe = atoi(argv[2]);
  dps = atoi(argv[3]);
  dpe = atoi(argv[4]);
  count = atoi(argv[5]);
  mdprintf("Arguments SPS: %d SPE: %d DPS: %d DPE: %d count: %d", sps, spe, dps, dpe, count); 

  pmask_count = get_best_block(sps, spe, pmask, count, &block_start, &block_end);          
  mdprintf ("Source Port Block is Block is %d - %d", block_start, block_end);
  for (i = 0; i < pmask_count; ++i) {
    mdprintf ("( %d / %d ) ", pmask[i].port, pmask[i].mask);
  }
  mdprintf("\n");
  
  pmask_count = get_best_block(dps, dpe, pmask, count, &block_start, &block_end);          
  mdprintf ("Dest Port Block is Block is %d - %d", block_start, block_end);
  for (i = 0; i < pmask_count; ++i) {
    mdprintf ("( %d / %d ) ", pmask[i].port, pmask[i].mask);
  }
  mdprintf("\n");

}
#endif
