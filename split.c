#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *sstrdup(char *i, char *t);
char **ppcappend(char **l, char *s);

char **split(char *s, char t)
{
	char *i, *j, **l;

	i = NULL;
	l = NULL;

	i = s;											/* set the beginning */

	while (*s++) {
		if (*s == t) {								/* splitter match */
			printf("b___________________________\n");

			j = sstrdup(i, s);						/* duplicate from the begining untill the splitter */
			if (j == NULL) {
				printf("split:\tcouldn't duplicate sub-string\n");
				return l;
			}

			printf("split:\t\treturned sub-string: %s\n", j);				/* error checking */

			l = ppcappend(l, j);					/* append the string to the list of char */
			if (l == NULL) {
				printf("split:\tcouldn't append sub-string\n");
				return l;
			}

			printf("split:\t\treturned l:\n"); 				/* print the list */
			printppc(l);

			i = ++s;								/* set the next beginning to the next char of the splitter */

			printf("e___________________________\n\n");
		}
	}
	return l;
}
/* duplicate sub-string */
char *sstrdup(char *i, char *t)
{
	char *p, q;
	printf("sstrdup got:\t%s\n", i);

	q = *t;											/* store the current value of *t */
	*t = '\0';										/* put \0 in the place of *t */
	p = strdup(i);									/* duplicate the string */
	if (p == NULL) {
		printf("sstrdup:\tcouldn't allocate memory\n");
		return NULL;
	}
	*t = q;											/* put back the value of *t */

	return p;
}
/* print a pointer to pointer to char */
int printppc(char **l)
{
	int c;

	c = 0;
	while (*l != NULL) {
		printf("%d:%p\t%s\n", c, l, *l++);
		c++;
	}
	return c;
}
/* return the  length of pointer to pointer to char */
int ppclen(char **l)
{
	int count;

	count = 0;
	if (l == NULL)
		return 0;
	while (*l++ != NULL)
		++count;
	return count;
}
char **ppcappend(char **l, char *s)
{
	int len;
	char **l1, **l2, **l3;

	l3 = l;											/* store the address of l to free it later */

	len = (ppclen(l) + 1) * sizeof(char **);		/* new length of the string array is the length of string array plus 1 times size of  */

	l1 = (char **) malloc (len);					/* allocate memory of the new length  */
	if (l1 == NULL) {
		printf("ppcappend:\tcouldn't allocate memory\n");
		return NULL;
	}

	l2 = l1;										/* store that to return */

	printf("ppcappend:\tallocated %p\n", l1);		/* erroe checking */

	if (l != NULL) {								/* if l is not NULL then there's already something in the array */
		while ((*l1 = *l) != NULL) {				/* copy the content of the old array to newly allocated array */
			l1++;
			l++;
		}
		free(l3);
	}

	*l1++ = s;										/* l1 either points to the begining of the new array or the end of the array */
	*l1 = NULL;										/* put NULL at the end of l1 */
	return l2;										/* return the initial address of l1 */
}
main(int argc, char *argv[])
{
	char **list = split("huhu;huhu;huhu;huhu;huhu;huhu;huhu;huhu;huhu;huhu;huhu;huhu;", ';');
	printppc(list);
}
