#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *strdup(char* s);
char *sstrdup(char *i, char *t);
char **appendl(char **l, char *s);

char **split(char *s, char t)
{
	char *i, *j, **l;

	i = NULL;
	l = NULL;

	i = s;

	while (*s++) {
		if (*s == t) {
			printf("b___________________________\n");
			j = sstrdup(i, s);
			printf("split: %s\n", j);
			l = appendl(l, j);
			printf("returned l\n");
			printfl(l);
			i = ++s;
			printf("e___________________________\n\n");
		}
	}
	return l;
}
char *sstrdup(char *i, char *t)
{
	char *p, q;

	q = *t;
	*t = '\0';
	p = strdup(i);
	printf("sstrdup: %s\n", p);
	*t = q;

	return p;
}
char *strdup(char *s)
{
	char *p;
	p = (char *) malloc (strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
int printfl(char **l)
{
	int c;

	c = 0;
	while (*l != NULL) {
		printf("l:%p\t%s\n", l, *l++);
		c++;
	}
	return c;
}
int listlen(char **l)
{
	int count;

	count = 0;
	if (l == NULL)
		return 0;
	while (*l++ != NULL)
		++count;
	return count;
}
char **appendl(char **l, char *s)
{
	int len;
	char **l1, **l2, **l3;

	l3 = l;
	len = (listlen(l) + 1) * sizeof(char **);
	l = (char **) realloc (l, len);
	l2 = l;
	printf("realloced\n");
	if (len > 4) {
	//if (l != NULL) {
		while (*l != NULL) {
			l++;
		}
		/*while ((*l1 = *l) != NULL) {
			l1++;
			l++;
		}*/
	}

	*l++ = s;
	*l = NULL;
	return l2;
}
main(int argc, char *argv[])
{
	char **list = split(argv[1], ';');
	printfl(list);
}
