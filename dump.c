#include <stdio.h>
#include <string.h>


#if _INTEGRAL_MAX_BITS > 32
	#define PROCESSOR_SIZE 64
	#define MINUS_1 0x8000000000000000
	#define MAX_INT 0xffffffffffffffff
#else
	#define PROCESSOR_SIZE 32
	#define MINUS_1 0x80000000
	#define MAX_INT 0xffffffff
#endif

int ibase, obase;
char *bin, bytbuf[12], buf[PROCESSOR_SIZE + 1], prpbuf[81];
char nuremals[] = "0123456789ABCDEF";



charindex (char *s, char t)
{
	int i, j, k;
	for (i = 0; *s++ != 0; i++) {
		if (*s == t) return i;
	}
	return -1;
}

/* revers s */
reverse(char *s)
{
	int c, i, j;
	char *p;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* checks whether the inputed string is valid or not */
isvalid (char *s)
{
	int cond;

	char bins[] = "010 ";
	char octs[] = "oq&$@\\01234567 ";
	char hexs[] = "XHRUZhruxz%&#+=:-$\"\'\\ 0123456789ABCDEFabcdef";

	char *p;

	switch(ibase) {
		case 2: p = bins; break;
		case 8: p = octs; break;
		case 10: return 1;
		case 16: p = hexs; break;
		default: break;
	}
	for(*s; *s != '\0'; *s++) {
		if ((cond = charindex (p, *s)) < 0) {
			fprintf (stderr, "%c not a standard base %d symbole\n", *s, ibase);
			return -1;
		}
	}
	return 1;
}


prepnum (char *s)
{
	int bitgrp, i, l;
	char *p, sprtr;

	switch(obase) {
		case 2: bitgrp = 4; sprtr = ' '; break;
		case 8: bitgrp = 3; sprtr = ' '; break;
		case 10: bitgrp = 3; sprtr = ','; break;
		case 16: bitgrp = 4; sprtr = ' '; break;
		default: break;
	}
	if (obase == 2) {
		reverse (s);
		while ((l = strlen(s)) && ( l %= bitgrp)) {
			strcat(s, "0");
		}
		reverse (s);
	}
	p = prpbuf;

	reverse (s);
	while (*s != '\0') {
		for (i = 0; i < bitgrp && *s != '\0'; i++) {
			*p++ = *s++;
		}
		*p++ = sprtr;
	}
	*--p = '\0';
	reverse (prpbuf);
}


/* char string s to numeral of base */

long long chartoint (char *s, int base)
{
	long long d = 0;
	for(*s; *s != '\0'; *s++) {
		d = base * d + (_Idigt(*s, base));
	}
	return d;
}

/* convert character into integers */

_Idigt (x, base)
{
switch (x)
	{
	case '0':
	case '1':
		return(x-'0');
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
		if (base > 2)
			return(x-'0');
	case '8':
	case '9':
		if (base > 8)
			return(x - '0');
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		if (base >10)
			return(x - 'a' + 10);
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		if (base > 10)
			return(x-'A' + 10);
	}
return(-1);
}

/* intobase: int n is converted into the the numeral system defined by base and put into buf */

intobase (long long n, int base)
{
        register long long  a;

        if (n<0) {      /* shouldn't happen */
                putchar('-');
                n = -n;
        }
        if(a = n/base)
                intobase (a, base);
        *bin++ = nuremals[(int)(n%base)];
}

main (int argc, char *argv[])
{
	int c, j, n, aflag, fflag, cond;
	FILE *fp;
	char *cp, *istr, *p, *numnm = (char *) malloc (sizeof(char) * 4);
	long long idata = 0;
	aflag = fflag = 0;

	for (cp = *++argv; *cp; *cp++) {
		switch(*cp) {
			case 'b':
				ibase = 2;
				switch(*++cp) {
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
				}
				break;
			case 'i':
				ibase = 10;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
				}
				break;
			case 'h':
				ibase = 16;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
				}
				break;
			case 'o':
				ibase = 8;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
				}
				break;
			case 'f':
				fflag++;
				istr = *++argv;
				break;
			case 'a':
				aflag++;
				istr = *++argv;
				break;
		}
	}

	if (aflag) {
		if ((cond = isvalid (istr)) < 0)
			return;
		idata = chartoint (istr, ibase);
		if (ibase == 10) {
			if (obase) {
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("bin: %s\n", prpbuf);
				obase = 8;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("oct: %s\n", prpbuf);
				obase = 16;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("hex: %s\n", prpbuf);
			}
		} else if (ibase == 2) {
			if (obase) {
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 8;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("oct: %s\n", prpbuf);
				obase = 10;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("int: %s\n", prpbuf);
				obase = 16;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("hex: %s\n", prpbuf);
			}
		}else if (ibase == 8) {
			if (obase) {
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("bin: %s\n", prpbuf);
				obase = 10;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("int: %s\n", prpbuf);
				obase = 16;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("hex: %s\n", prpbuf);
			}
		} else if (ibase == 16) {
			if (obase) {
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("bin: %s\n", prpbuf);
				obase = 8;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("oct: %s\n", prpbuf);
				obase = 10;
				bin = buf;
				intobase (idata, obase);
				*bin = '\0';
				prepnum (buf);
				printf ("int: %s\n", prpbuf);
			}
		} 
	} else if (fflag) {
		fp = fopen (istr, "r");
		if (fp != NULL) {
			if (ibase == 10) {
				if (obase == 2) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intobase (c, obase);
						*bin = '\0';
						prepnum (buf);
						printf ("%s ", prpbuf);
					}
				} else if (obase == 8) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intobase (c, obase);
						*bin = '\0';
						prepnum (buf);
						printf ("%s ", prpbuf);
					}
				} else if (obase == 16) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intobase (c, obase);
						*bin = '\0';
						prepnum (buf);
						printf ("%s ", prpbuf);
					}
				}
			}
		} else {
			printf ("unable to open file %s", istr);
		}
	}
}
