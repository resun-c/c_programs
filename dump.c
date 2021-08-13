#include <stdio.h>

#define PROCESSOR_SIZE 64
#define MINUS_1 0x8000000000000000
#define MAX_INT 0xffffffffffffffff


char *bin, bytbuf[12], buf[PROCESSOR_SIZE + 1], prpbuf[81];


/*

   list of converters
_______________________
|  chartoint :	done  |
|  bintoint  :	done  |
|  bintohex  :	done  |
|  bintoct   :	done  |
|  intobin   :	done  |
|  intohex   :	done  |
|  intoct    :	done  |
|  hextobin  :	done  |
|  hextoint  :	done  |
|  hextoct   :	done  |
|  octobin   :	done  |
|  octoint   :	done  |
|  octohex   :	done  |
_______________________

*/


/*	these fnction are from library strings.h. I wrote them just to test myself	*/

charindex (char *s, char t)
{
	int i, j, k;
	for (i = 0; *s++ != 0; i++) {
		if (*s == t) return i;
	}
	return -1;
}


strlen(char *s)
{
	int i;
	i = 0;
	while(s[i] != '\0') i++;
	return i;
}


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


strcat(char *s, char *t)
{
	while (*s != '\0') *s++;

	while ((*s++ = *t++) != '\0') ;
}


strcpy(char *s, char *t)
{
	while ((*s++ = *t++) != '\0') ;
}


/* end of library functions */


/* functions hereafter are for checking and converting between int, bin, hex and oct */


/* prepares a string containing binary numbers to show them in a convenient manner */

prpbin (char *s) {
	int l;
	char *p;

	for (*s; *s != '1'; *s++) ;
	p = s;

	reverse(p);
	while ((l = strlen(p)) && ( l %= 4)) {
		strcat(p, "0");
	}
	reverse (p);
	strcpy(buf, p);
}

/* prepares a string containing 8 binary numbers to show them in a convenient manner */
prpbyt (char *s)
{
	int i, j;
	char *p;

	reverse(s);

	p = bytbuf;
	for (i = 0; i < 8; i++) {
		*p++ = *s++;
	}
	*p = '\0';
	reverse(bytbuf);
}

/* prepares a string containing nibbles */

prpnbl (char *s)
{
	int i;
	char *p;
	p = prpbuf;
	while (*s != '\0') {
		for (i = 0; i < 4 && *s != '\0'; i++) {
			*p++ = *s++;
		}
		*p++ = ' ';
	}
	*p = '\0';
}

/* prepares a string containing hexadecimal numbers to show them in a convenient manner */

prphex (char *s)
{
	reverse(s);
	prpnbl (s);
	reverse (prpbuf);
}

/* prepares a string containing trio binary numbers to show them in a convenient manner */

prptrio (char *s)
{
	int i;
	char *p;
	p = prpbuf;

	while (*s != '\0') {
		for (i = 0; i < 3 && *s != '\0'; i++) {
			*p++ = *s++;
		}
		*p++ = ' ';
	}
	*p = '\0';
}

/* prepares a string containing octal numbers to show them in a convenient manner */
prpoct (char *s)
{
	reverse (s);
	prptrio (s);
	reverse (prpbuf);
}


/* convert character into integers */

_Idigt (x, base)
{
switch (x)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
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

/* checks if the string contains only ints */

isint (char *s)
{
	int cond;
	char ints[] = "0123456789";

	for(*s; *s != '\0'; *s++) {
		if ((cond = charindex (ints, *s)) < 0) {
			fprintf (stderr, "%c not a standard integer symbole\n", *s);
			return -1;
		}
	}
	return 1;
}

/* checks if the string contains only bins */

isbin (char *s)
{
	int cond;
	char bins[] = "010";

	for(*s; *s != '\0'; *s++) {
		if ((cond = charindex (bins, *s)) < 0) {
			fprintf (stderr, "%c not a standard binary symbole\n", *s);
			return -1;
		}
	}
	return 1;
}

/* checks if the string contains only hexs */

ishex (char *s)
{
	int cond;
	char hexs[] = "XHRUZhruxz%&#+=:-$\"\'\\0123456789ABCDEFabcdef";

	for(*s; *s != '\0'; *s++) {
		if ((cond = charindex (hexs, *s)) < 0) {
			fprintf (stderr, "%c not a standard hexadecimal symbole\n", *s);
			return -1;
		}
	}
	return 1;
}

/* checks if the string contains only octs */

isoct (char *s)
{
	int cond;
	char octals[] = "oq&$@\\01234567";

	for(*s; *s != '\0'; *s++) {
		if ((cond = charindex (octals, *s)) < 0) {
			fprintf (stderr, "%c not a standard octal symbole\n", *s);
			return -1;
		}
	}
	return 1;
}

/* convert string into integers */

long long chartoint (char *s)
{
	long long d = 0;
	for(*s; *s != '\0'; *s++) {
		d = 10 * d + (_Idigt(*s, 10));
	}
	return d;
}


/* intobin: int to binary */
intobin (long long n)
{
	unsigned long long a;
	for (a = MINUS_1; a >= 0x01; a /= 0x02) {
		if (n & a) {
			*bin++ = '1';
		} else {
			*bin++ = '0';
		}
	}
	*bin = '\0';
}

/* intohex: int to hexadecimal */
intohex (long long n)
{
        int d;
	long long a;
        if (a = n>>4) {
                intohex ( a & MAX_INT);
	}
        d = n&017;
        *bin++ =  d > 9 ? 'A'+d-10 : '0' + d;
}



/* intoct: int to octal */
intoct (long long n)
{
        int d;
	long long a;
        if (a = n>>3)
                 intoct (a & MAX_INT);
        d = n & 0x07;
        *bin++ =  '0' + d;
}

/* bintoint: return the int value of the binary string s */

long long bintoint (char *s)
{
	int i, l;
	long long d, d1;
	char *p;

	p = buf;
	d = 0;

	reverse (s);
	strcpy (p, s);

	while ((l = strlen(p)) <= 64) {
		strcat (p, "0");
	}
	reverse (p);

	for (i = 0; buf[i] != '\0'; i++) {
		d1 = (long long) (buf[i] - '0');
		d |= d1 << (64 - i);
	}
	return d;
}

/* bintohex:  binary to hexadecimals */

bintohex (char *s)
{
	long long i;

	i = bintoint(s);
	bin = buf;

	intohex (i);
	*bin = '\0';
}

/* bintoct: binary to octals */

bintoct (char *s)
{
	long long i;

	i = bintoint(s);
	bin = buf;

	intoct (i);
	*bin = '\0';
}

/* hextobin : hexadecimal char to binary string*/

char *hextobin (c)
{
	char *s = (char *) malloc (sizeof (char) * 5);

	switch(c) {
		case '0':
			s = "0000";
			break;
		case '1':
			s = "0001";
			break;
		case '2':
			s = "0010";
			break;
		case '3':
			s = "0011";
			break;
		case '4':
			s = "0100";
			break;
		case '5':
			s = "0101";
			break;
		case '6':
			s = "0110";
			break;
		case '7':
			s = "0111";
			break;
		case '8':
			s = "1000";
			break;
		case '9':
			s = "1001";
			break;
		case 'A': case 'a':
			s = "1010";
			break;
		case 'B': case 'b':
			s = "1011";
			break;
		case 'C': case 'c':
			s = "1100";
			break;
		case 'D': case 'd':
			s = "1101";
			break;
		case 'E': case 'e':
			s = "1110";
			break;
		case 'F': case 'f':
			s = "1111";
			break;
		default:
			s = "";
			break;
	}
	return s;
}

/* hextoint :  return int of hexadecimal string s */

long long hextoint (char *s)
{
	long long d = 0;
	for(*s; *s != '\0'; *s++) {
		d = 16 * d + (_Idigt(*s, 16));
	}
	return d;
}

/* hextoct :  hexadecimal string to octal */

hextoct (char *s)
{
	long long d;
	d = hextoint(s);
	intoct(d);
}



/* octobin : octalal char to binary string */

char *octobin (c)
{
	char *s = (char *) malloc (sizeof (char) * 4);

	switch(c) {
		case '0':
			s = "000";
			break;
		case '1':
			s = "001";
			break;
		case '2':
			s = "010";
			break;
		case '3':
			s = "011";
			break;
		case '4':
			s = "100";
			break;
		case '5':
			s = "101";
			break;
		case '6':
			s = "110";
			break;
		case '7':
			s = "111";
			break;
		default:
			s = "";
			break;
	}
	return s;
}

/* octoint : octal string to */;

long long octoint (char *s)
{
	long long d = 0;
	for(*s; *s != '\0'; *s++) {
		d = 8 * d + (_Idigt(*s, 8));
	}
	return d;
}

/* octohex : octal string to hexadecimal */;
octohex (char *s)
{
	long long d;
	d = octoint(s);
	intohex(d);
}


main (int argc, char *argv[])
{
	int c, j, n, aflag, fflag, bflag, iflag, hflag, oflag, b, i, h, o, cond;
	FILE *fp;
	char *cp, *istr, *p;
	long long idata = 0;
	aflag = bflag = fflag = iflag = hflag = oflag = b = i = h = o = 0;

	for (cp = *++argv; *cp; *cp++) {
		switch(*cp) {
			case 'b':
				bflag++;
				switch(*++cp) {
					case 'i':
						i++;
						break;
					case 'h':
						h++;
						break;
					case 'o':
						o++;
						break;
				}
				break;
			case 'i':
				iflag++;
				switch(*++cp) {
					case 'b':
						b++;
						break;
					case 'h':
						h++;
						break;
					case 'o':
						o++;
						break;
				}
				break;
			case 'h':
				hflag++;
				switch(*++cp) {
					case 'b':
						b++;
						break;
					case 'i':
						i++;
						break;
					case 'o':
						o++;
						break;
				}
				break;
			case 'o':
				oflag++;
				switch(*++cp) {
					case 'b':
						b++;
						break;
					case 'i':
						i++;
						break;
					case 'h':
						h++;
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
		if (iflag) {
			if ((cond = isint (istr)) > 0) {
				if (b) {
					idata = chartoint (istr);
					bin = buf;
					intobin (idata);
					prpbin (buf);
					prpnbl (buf);
					printf ("bin:  %s\n", prpbuf);
				} else if (h) {
					idata = chartoint (istr);
					bin = buf;
					intohex (idata);
					*bin = '\0';
					prphex (buf);
					printf ("hex:%s\n", prpbuf);
				} else if (o) {
					idata = chartoint (istr);
					bin = buf;
					intoct (idata);
					*bin = '\0';
					prpoct (buf);
					printf ("oct: %s\n", prpbuf);
				} else {
					idata = chartoint (istr);
					bin = buf;
					intobin (idata);
					prpbin (buf);
					prpnbl (buf);
					printf ("bin: %s\n", prpbuf);
					bin = buf;
					intohex (idata);
					*bin = '\0';
					prphex (buf);
					printf ("hex: %s\n", prpbuf);
					bin = buf;
					intoct (idata);
					*bin = '\0';
					prpoct (buf);
					printf ("oct: %s\n", prpbuf);
				}
			}
		} else if (bflag) {
			if ((cond = isbin (istr)) > 0) {
				if (i) {
					idata = bintoint (istr);
					printf ("int: %lld\n", idata);
				} else if (h) {
					bintohex (istr);
					prphex (buf);
					printf ("hex: %s\n", prpbuf);
				} else if (o) {
					bintoct (istr);
					prpoct (buf);
					printf ("oct: %s\n", prpbuf);
				} else {
					idata = bintoint (istr);
					printf ("int: %lld\n", idata);
					bintohex (istr);
					prphex (buf);
					printf ("hex: %s\n", prpbuf);
					bintoct (istr);
					prpoct (buf);
					printf ("oct: %s\n", prpbuf);
				}
			}
		} else if (hflag) {
			if ((cond = ishex (istr)) > 0) {
				if (b) {
					printf ("bin: ");
					for (j = 0; istr[j] != '\0'; j++)
						printf ("%s", hextobin(istr[j]));
				} else if (i) {
					idata = hextoint (istr);
					printf ("int: %lld\n", idata);
				} else if (o) {
					bin = buf;
					hextoct (istr);
					prptrio (buf);
					printf ("oct: %s\n", prpbuf);
				}
			}
		} else if (oflag) {
			if ((cond = isoct (istr)) > 0) {
				if (b) {
					for (j = 0; istr[j] != '\0'; j++)
						printf ("bin: %s\n", octobin(istr[j]));
				} else if (i) {
					idata = octoint (istr);
					printf ("int: %lld\n", idata);
				} else if (h) {
					bin = buf;
					octohex (istr);
					prpnbl (buf);
					printf ("hex: %s\n", prpbuf);
				}
			}
		}
	} else if (fflag) {
		fp = fopen (istr, "r");
		if (fp != NULL) {
			if (iflag) {
				if (b) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intobin (c);
						prpbyt (buf);
						prpnbl (bytbuf);
						printf ("%s\t", prpbuf);
					}
				} else if (h) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intohex (c);
						*bin = '\0';
						prphex (buf);
						printf ("%s\t", prpbuf);
					}
				} else if (o) {
					while ((c = getc (fp)) != EOF) {
						bin = buf;
						intoct (c);
						*bin = '\0';
						prpoct (buf);
						printf ("%s\t", prpbuf);
					}
				}
			}
		} else {
			printf ("unable to open file %s", istr);
		}
	}
}
