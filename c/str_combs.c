#include <stdio.h>
#include <string.h>


#define PRINT_STR(c, k) {\
    for (int t = 0; t < l; t++) {\
        if(t==k) { printf("%d", c); }\
        else if(t == (k+1)) {printf("");}\
        else { printf("0"); }\
    }\
    printf("\n");\
}

int convert_str(char *str)
{
    int l = strlen(str), flag = 0, a = -1, b = -1, c = -1;
    int nd = l/2, count = 1, ndc = 0;

    for(int i = 1; i <= nd; i++) {

        for (int j = 0; j < l; j++) {

            ndc = 0;

            for (int k = j; k < l; ) {

                a = b = c = -1;
                a = str[k] - '0';

                if ((a == 0) && (!flag)) { 
                    printf("..\n");
                    flag = 1; 
                    count = count - 1; 
                }

                if ( k < (l-1) ) {

                    b = str[k+1] - '0';

                        c = (a*10) + b;

                        if ((c <= 26) && (c >= 10)) {

                            ndc++;
                            printf("c = %d (ndc = %d i = %d j = %d k = %d) ", c, ndc, i, j, k);
                            PRINT_STR(c, k);

                        } else {
                            printf("c = %d not counting \n", c);
                            c = -1;
                            break;
                        }
                } 

                if (ndc == i) {
                    count++;
                    printf("k: %d Count: %d \n", k, count);
                    break;
                }

                if (c != -1) {
                    k = k + 2;
                } else {

                    k = k+1;;

                }

            }

        }

    }

    return count;

}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Arg required \n");
        return 1;
    }

    int count = convert_str(argv[1]);
    printf("count = %d \n", count);

    return 0;
}
