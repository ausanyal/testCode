#include <stdio.h>
#include <string.h>

char ohay[100], oneedle[10];

int find_index(char *hay, char c)
{
    for (int i = 0; i < strlen(hay); i++) {
        if (c == hay[i]) {
            return i;
        }
    }

    return -1;
}

int find_pattern(char *hay, char *needle)
{
    if(!strlen(needle)) {
        return 1;
    }

    if(!strlen(hay)) {
        return 0;
    }

    int i = 0, idx = 0;
    char ch;
    switch (needle[0]) {
        case '.':
            while (needle[i] == '.') {
                ++i;
                if (i >= strlen(needle)) {
                    return 1;
                }
            }
            if(needle[i] != hay[i]) {
                return find_pattern(hay+i, oneedle);;
            } else {
                ++i;
                return find_pattern((hay+i), (needle+i));
            }
            break;
        case '*':
            idx = strlen(oneedle) - strlen(needle);
            ch = oneedle[idx-1];

            while(hay[i] == ch) {
                ++i;
                if ((i >= strlen(hay)) && (strlen(needle) > 1)) {
                    return 1;
                    
                }
            }
            return find_pattern(hay+i, needle+1); 
            break;
        default:
            idx = find_index(hay, needle[i]);
            if (idx != -1) {
                return find_pattern(hay+idx+1, needle+i+1);
            } else {
                return find_pattern(hay+1, oneedle);

            }
            
    }
    
    printf("<<<<<<<<<<< Should not come here >>>>>>>>>>>>>>>> \n");
    return 0;
}

int main()
{
    FILE *fp = fopen("pattern_input.txt", "r");
    if (fp < 0)  {
        return 1;
    }

    while (fscanf(fp, "%s %s", ohay, oneedle) != EOF) {
        if (find_pattern(ohay, oneedle)) {
            printf(" ************************************ found %s in %s \n", oneedle, ohay);
        } else {
            printf("************************************* Not found %s in %s \n", oneedle, ohay);

        }
    }

    return 0;
}
