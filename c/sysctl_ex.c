#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>

#if 0
struct __sysctl_args {
  int    *name;    /* integer vector describing variable */
  int     nlen;    /* length of this vector */
  void   *oldval;  /* 0 or address where to store old value */
  size_t *oldlenp; /* available room for old value,
                      overwritten by actual size of old value */
  void   *newval;  /* 0 or address of new value */
  size_t  newlen;  /* size of new value */
};
#endif

#define OSNAMESZ 100

int main()
{
  //char *str="ip netns exec overlay-1 sysctl -w net.ipv4.conf.all.rp_filter=0";
  //printf("s: %s \n", str);
  char osname[OSNAMESZ];
  size_t osnamel;
  int name[] = {CTL_KERN, KERN_OSTYPE};
  u_int nlen = sizeof(name)/sizeof(name[0]);
  osnamel = sizeof(osname);

  //int
  //sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp, size_t newlen);

  if (sysctl(name, nlen, osname, &osnamel, NULL, 0) == -1) {
    perror("error: sysctl");
    exit(-1);
  }

  printf("This machine is running %s \n", osname);

  return 0;
}
