#include <stdio.h>
#include <string.h>

#define ITR_PRINT(format, args...) \
    fprintf(fp2, format"\n", ##args);

int main()
{
    FILE *fp, *fp2;
    char s[50], line[100];
    int i, j, cnt, bt_teps[5];

    fp = fopen("file1.txt", "r");
    if (fp == NULL) {
        return 0;
    }
    
    while(fgets(line, 100, fp) != NULL) {
        //printf("line = %s", line);
        sscanf(line, "%s %d", s, &i);
        if(!strcmp(s, "sport_start:")) {
            printf("sport_start:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "sport_end:")) {
            printf("sport_end:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "dport_start:")) {
            printf("sport_start:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "dport_end:")) {
            printf("sport_end:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "prot:")) {
            printf("prot:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "int_sec:")) {
            printf("int_sec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "int_usec:")) {
            printf("int_usec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "ttl_sec:")) {
            printf("ttl_sec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "ttl_usec:")) {
            printf("ttl_usec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "fake_sec:")) {
            printf("fake_sec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "fake_usec:")) {
            printf("fake_usec:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "max_hops:")) {
            printf("max_hops:%s => i:%d\n", s, i);
        }
        if(!strcmp(s, "num_bts:")) {
            cnt = i;
            printf("num_bts:%s => i:%d\n", s, i);
            for(j = 0; j < cnt ; j++) {
                if (fgets(line, 100, fp) != NULL) {
                    sscanf(line, "%s %d", s, &i);
                    if(!strcmp(s, "bt_tep:")) {
                        bt_teps[j] = i;
                    }
                }
            }

            for(j = 0; j < cnt ; j++) {
                printf("tep: 0x%x \n", bt_teps[j]);
            }
        }
        
    }

    fp2 = fopen("file2.txt", "a");
    if (fp2 == NULL) {
        return 0;
    }
    //fprintf(fp2, "Hello \n");
    ITR_PRINT("Hello world \n");

    return 1;
}
