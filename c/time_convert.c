#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t     now;

    //struct tm  ts;
    struct timespec  ts;

    char       buf[80];

    // Get current time
    time(&now);
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = clock_gettime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
    return 0;
}
