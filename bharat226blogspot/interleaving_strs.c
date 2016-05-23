/*Given two strings, print all interleaving strings.
Solution #1: Note that interleaving of two strings does not change the actual order of the characters within the two strings. For e.g.

str1 = "AB"
str2 = "MN"

Interleaving strings are,

ABMN
AMBN
AMNB
MABN
MANB
MNAB
*/
void interleave(char* str1, char* str2, char* str, int len)
{
    int i=0;
 
    if(str1[0] == '\0' && str2[0] == '\0')
    {
        printf("%s\n", str-len);
        return;
    }
 
    if(str1[0] != '\0')
    {
        str[0] = str1[0];
        interleave(str1+1, str2, str+1, len);
    }
    if(str2[0] != '\0')
    {
        str[0] = str2[0];
        interleave(str1, str2+1, str+1, len);
    }
}
 
int main()
{
    char* str1 = "AB";
    char* str2 = "MNO";
     
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int len = len1+len2;
 
    char* str = (char*)malloc(len+1);
    memset(str, 0, len+1);
 
    interleave(str1, str2, str, len);
    return 0;
}

