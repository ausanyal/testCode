#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    size_t read;
    size_t len = 0;
    char *line = NULL, *s;
    FILE *fp = fopen("parse_file_line_by_line.txt", "r");
    if (fp == NULL) {
        return 1;
    }

    /*
    line: link_accel: 1/97, ALL
    line: global-accel: ALL
    */
    char mode[10];
    int mod, port;
    while((read = getline(&line, &len, fp)) != -1) {
        //line[strlen(line) - 1] = '\0';
        printf("line: %s (%lu) \n", line, strlen(line));
        if (!strncmp(line, "link_accel:", 11)) {
            sscanf(line, "link_accel: %d/%d, %s", &mod, &port, mode);
            printf("mod %d port %d mode %s \n", mod, port, mode);
        }
        if (!strncmp(line, "global-accel:", 13)) {
            sprintf(line, "global-accel: %s", mode);
            printf("mode %s \n", mode);
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }
    return 1;
}
