#include <stdio.h>
#include <string.h>

int strindex(char *s, char t[]);

int main() {
    char srccd[] = getchar();
    int iknp, tknp, df, i, k;
    char ik[] = "<p>";
    char tk[] = "</p>";
    iknp = strindex(srccd, ik);
    tknp = ((strindex(srccd, tk)) + 3);
    df = tknp - iknp;
    char codeary[df];
    for (i = 0, k = iknp; i <= df && k <= tknp; i++, k++) {
        codeary[i] = srccd[k];
        }
    printf("%s\n", codeary);
}
int strindex(char *s, char t[])
{
    int i, j, k;
    for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
            return i;
    }
    return -1;
}
