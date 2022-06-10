#include <stdio.h>

int main() {
        int c, i;
        FILE *fp;
        char buf[512];

        fp = fopen("huhu.in", "r+");

        for (;;) {
                i = 0;
                while ((c = getc(fp)) != EOF) {
                        if (c != '\n') {
                                buf[i++] = c;
                        } else {
                                if (i > 0) {
                                        buf[i] = '\0';
                                        sfile(buf);
                                        i = 0;
                                }
                        }
                }
        }
        fclose(fp);
}

sfile(char *filen) {
        int c;
        FILE *fp;

        fp = fopen(filen, "r");

        while((c = getc(fp)) != EOF) {
                putchar(c);
        }
        fclose(fp);
}