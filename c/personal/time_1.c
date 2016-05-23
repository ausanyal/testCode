#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{
    struct timeval t;
    struct timeval t2;
    time_t nowtime;
    struct tm *nowtm;
    char tmpbuf[64], buf[64];

    gettimeofday(&t, NULL);
    sleep(1);
    usleep(100);
    gettimeofday(&t2, NULL);
    nowtime = t.tv_sec;
    nowtm = localtime(&nowtime);

    printf("%lu : %d \n", t.tv_sec, t.tv_usec);
    printf("%lu : %d \n", t2.tv_sec, t2.tv_usec);
    strftime(tmpbuf, sizeof(tmpbuf), "%Y-%m-%d %H:%M:%S", nowtm);
    //printf("%s \n", tmpbuf);
    snprintf(buf, sizeof(buf), "%s.%06d", tmpbuf, t.tv_usec);
    //printf("%s ; %s\n", tmpbuf, buf);

    unsigned long int tot1 = (t.tv_sec * 1000000) + t.tv_usec;
    unsigned long int tot2 = (t2.tv_sec * 1000000) + t2.tv_usec;
    unsigned long int diff = tot2 - tot1;
    unsigned long int diff2 = t2.tv_sec - t.tv_sec;
    unsigned long int diff3 = t2.tv_usec - t.tv_usec;
    printf("tot1: %lu tot2: %lu diff: %lu diff2: %lu diff3: %lu \n", tot1, tot2, diff, diff2, diff3);

    unsigned long int sec = diff/(1000000);
    unsigned long int usec = diff%(1000000);

    printf("\n\nsec: %lu ; usec: %lu \n", sec, usec);


    return 1;
}
