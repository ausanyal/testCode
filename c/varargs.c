#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int my_printf(const char *fmt, ...)
{
    const char  *p;
    char        *s, t;
    char        fmt_buf[1024];
    int         i;
    va_list     argp;

    va_start(argp, fmt);

    for (p = fmt ; p != '\0'; p++) {
        if (*p != '%') {
            printf("%s: %c \t", __func__, *p);
            putchar(*p);
            continue;
        }
        // In va_arg: char, short int promoted to int
        //            float promoted to double
        t = *++p;
        printf("\nfmt: %c \n", t);
        switch(t) {
            case 'c':
                i = va_arg(argp, int); 
                printf("\n i = %d \n", i);
                putchar(i);            
                break;
            case 'd':
                i = va_arg(argp, int);
                printf("\n d = %d \n", i);
                //s = itoa(i, fmt_buf, 10);
                //fputs(s, stdout);
                putchar(i);
                break;
            case 'x':
                i = va_arg(argp, int);
                //s = itoa(i, fmt_buf, 16);
                //fputs(s, stdout);
                putchar(i);
                break;
            case 's':
                s = va_arg(argp, char *);
                fputs(s, stdout);
                break;
            case '%':
                putchar('%');
                break;
        }
    }

    va_end(argp);

    return 0;
}

int main()
{
    int i = 5, c = 't';
    char s[100], str[20];
    
    memcpy(s, "Hello", 6);
    strcpy(str, "str: %c");

    //my_printf("str: %c %d", c, i);
    my_printf(str, c);
    
    printf("\n");

    return 1;
}
