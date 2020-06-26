#include <stdio.h>

int getchar_len();
int main()  {
    int i = getchar_len();
    printf("%d\n", i);
}
int getchar_len() {
    int c, i;
    i = 0;
    while ((c = getchar()) != EOF && c == '\n' == '\t' == '\0')
        ++i;
    return i;
}
