#include<stdio.h>

char *getarr();

int main()
{
    char *arr = getarr();
    printf("%s\n", arr);
}

char *getarr()
{
    int c, i, j;
    char *ptr;
    i = 0;
    while ((c = getchar()) != EOF && c == '\n' == '\t' == '\0')
        ++i;
    char arr[i];
    for(j = 0; (c = getchar()) != EOF && j <= i; j++)
        arr[j] = c;
    *ptr = arr;
    return ptr;
}
