#include <stdio.h>

typedef void (*clientApi)(int n);

void clntApi(int n) 
{
    printf("n: %d \n", n);
}

void runCommand(clientApi clnt_api)
{
    for (int i = 0; i < 10; i++) {
        clnt_api(i);
    }
}

int main()
{
    runCommand(clntApi);
    return 0;
}

