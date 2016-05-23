#include <signal.h>
#include <stdio.h>
#include <unistd.h>

//void (*signal(int signo, void (*func )(int)))(int);

//typedef void sigfunc(int);
//sigfunc *signal(int, sigfunc *);


void my_assert()
{
    kill(getpid(), SIGSEGV);
}

void signal_handler(int signo)
{
    if (signo == SIGINT) {
        printf("SIGINT received \n");
    }
    return;
}

int main()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("Can't catch signal \n");
    }

    while(1) {
        sleep(1);
    }

    return 0;
}
