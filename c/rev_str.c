#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rev_str(char *str)
{
    int wrd_len = 0, strl = strlen(str), nstr_len = 0;
    char *nstr = (char *)calloc(1, strl);

    for (int i = strl-1; i >= 0; ) {
        wrd_len = 0;
        while((str[i] != ' ') && (i >= 0)) {
            wrd_len++;
            i--;
        }
        strncpy(nstr + nstr_len, &(str[i+1]), wrd_len);
        nstr_len += wrd_len;
        while(str[i] == ' ') {
            i--;
           strcat(nstr, " ");
           nstr_len++;
        }
        printf("%d : wrd_len: %d str: %s    nstr: %s.\n", i, wrd_len, &(str[i+1]), nstr);
    }

    return NULL;
}

char *rev_str2(char *str)
{
    // First reverse whole string
    char temp, *nstr;
    int l = strlen(str), ws = 0, we = 0;
    int s = 0, e = l-1, temp_c;

    nstr = (char *)calloc(1, strlen(str));
    strcpy(nstr, str);

    while(s < e) {
        temp = nstr[s];
        nstr[s] = nstr[e];
        nstr[e] = temp;
        s++;
        e--;
    }
    printf("%s \n", nstr);

    s = e = 0;

    while(s < l) {
        while(nstr[e] != ' ' && nstr[e] != '\0') e++;
        ws = s; 
        we = e-1;
        while(ws < we) {
            temp = nstr[ws];
            nstr[ws] = nstr[we];
            nstr[we] = temp;
            ws++;
            we--;
        }
        s = e = e+1;
    }

    printf("%s \n", nstr);

    return nstr;
}

int main()
{
    char *str;
    //str = rev_str("I am       Aubin");
    str = rev_str2("Hello how are you doing ??");

    return 0;
}
