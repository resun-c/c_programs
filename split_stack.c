#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define		STACK_SIZE		8

typedef struct _stack {
	int sp;
	int lsp;
	char **ppc;
} stack;

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
/* initialize a new stack */
void init_stack(stack *stk)
{
	stk->sp = 0;
	stk->lsp = STACK_SIZE;
	stk->ppc = malloc(STACK_SIZE * sizeof(char **));
}
/* grow the size of the stack */
void grow_stack(stack *stk)
{
	char **nppc;
	int i;

	nppc = malloc((stk->sp + STACK_SIZE) * sizeof(char **));

	if (nppc != NULL) {
		for (i = 0; i <= stk->sp; i++) {						/* copy the content of the old array to newly allocated array */
			nppc[i] = stk->ppc[i];
		}
		free(stk->ppc);
	}

	stk->lsp = STACK_SIZE;
	stk->ppc = nppc;
}
/* print a pointer to pointer to char */
int printppc(char **ppc)
{
	int c;

	c = 0;
	while (*ppc != NULL) {
		printf("%d\t:\t%s\n", c, *ppc++);
		c++;
	}
	return c;
}
/* print a pointer to pointer to char from a stack*/
int printstack(stack *stk)
{
	int i;

	for (i = 0; i < stk->sp; i++)
		printf("%s\n", stk->ppc[i]);

	return i;
}
/* append a pointer to char at the end of a pointer to pointer to char of a stck*/
int stackappend(stack *stk, char *s)
{
	if (stk->lsp < 1)
		grow_stack(stk);

	stk->ppc[stk->sp++] = s;
	stk->lsp--;

	return 1;
}
/* split a string in to substring(s) when spltr is found */
char **split(char *s, char spltr)
{
	char *nxt_ini, *subs;
	stack *ppc = malloc(sizeof(stack));

	init_stack(ppc);
	nxt_ini = NULL;
	subs = NULL;

	nxt_ini = s;											/* set the beginning */

	while (*s++) {
		if (*s == spltr) {									/* splitter match */
			subs = sstrdup(nxt_ini, s);						/* duplicate from the begining untill the splitter */
			if (subs == NULL)
				return ppc->ppc;

			stackappend(ppc, subs);							/* append the string to the stack */
			if (ppc == NULL)
				return ppc->ppc;

			nxt_ini = ++s;									/* set the next beginning to the next char of the splitter */
		}
	}

	if ((s - nxt_ini) > 1) {
		subs = sstrdup(nxt_ini, s);
		if (subs != NULL)
			stackappend(ppc, subs);
	}
	stackappend(ppc, NULL);									/* terminating the pointer to pointer to char */
	return ppc->ppc;
}

main(int argc, char *argv[])
{
	char **s = split(argv[2], argv[1][0]);
	printppc(s);

}
