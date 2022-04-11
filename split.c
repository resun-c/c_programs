#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *sstrdup(char *i, char *t);
char **ppcappend(char **l, char *s);

char **split(char *s, char spltr)
{
	char *nxt_ini, *subs, **ppc;

	nxt_ini = NULL;
	subs = NULL;
	ppc = NULL;

	nxt_ini = s;											/* set the beginning */

	while (*s++) {
		if (*s == spltr) {								/* splitter match */

			subs = sstrdup(nxt_ini, s);						/* duplicate from the begining untill the splitter */
			if (subs == NULL)
				return ppc;

			ppc = ppcappend(ppc, subs);					/* append the string to the list of char */
			if (ppc == NULL)
				return ppc;

			nxt_ini = ++s;								/* set the next beginning to the next char of the splitter */
		}
	}
	return ppc;
}
/* duplicate sub-string */
char *sstrdup(char *ini, char *termn)
{
	char *subs, tmp;

	tmp = *termn;											/* store the current value of *t */
	*termn = '\0';											/* put \0 in the place of *t */

	subs = strdup(ini);										/* duplicate the string */

	*termn = tmp;											/* put back the value of *t */

	return subs;
}
/* print a pointer to pointer to char */
int printppc(char **ppc)
{
	int c;

	c = 0;
	while (*ppc != NULL) {
		printf("%p\t%s\n", ppc, *ppc++);
		c++;
	}
	return c;
}
/* return the  length of pointer to pointer to char */
int ppclen(char **ppc)
{
	int count;

	count = 0;
	if (ppc == NULL)
		return 0;
	while (*ppc++ != NULL)
		++count;
	return count;
}
char **ppcappend(char **ppc, char *s)
{
	int len;
	char **nppc, **rppc, **temp;

	temp = ppc;											/* store the address of l to free it later */

	len = (ppclen(ppc) + 2) * sizeof(char **);			/* new length of the string array is the length of string array plus 1 times size of  */

	nppc = (char **) malloc (len);						/* allocate memory of the new length  */
	if (nppc == NULL)
		return NULL;

	rppc = nppc;										/* store that to return */

	if (ppc != NULL) {									/* if l is not NULL then there's already something in the array */
		while ((*nppc = *ppc) != NULL) {				/* copy the content of the old array to newly allocated array */
			nppc++;
			ppc++;
		}
		free(temp);
	}

	*nppc++ = s;										/* l1 either points to the begining of the new array or the end of the array */
	*nppc = NULL;										/* put NULL at the end of l1 */
	return rppc;										/* return the initial address of l1 */
}
main(int argc, char *argv[])
{
	char **list = split(argv[2], argv[1][0]);
	printppc(list);
}
