#include <stdio.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define MAXLINE (10000)


typedef struct _chain {
	char prnt_hsh[8];
	char hsh[8];
	char *data;
	struct _chain *prnt;
	struct _chain *nxt;
} chain;

void print_chain(chain *t, int recursive, int is_sub);
chain *add_block(chain *t, char *data);

main(int argc, char *argv[])
{
	long long int ut;
	unsigned int seed;
	char line[MAXLINE];
	FILE *fp;
	chain *block_chain;

	ut = time(NULL);
	seed = (unsigned int) (ut | 0177777);
	block_chain = NULL;
	srand(seed);
	fp = fopen(*++argv, "r");

	while(getlinef(fp, line, MAXLINE) > 0) {
		block_chain = add_block(block_chain, line);
	}

	print_chain(block_chain, 1, 1);
}
/* readline in char *s from fp*/
int getlinef(FILE *fp, char *s, int lim)
{
	int i;
	char c;

	i = 0;

	while (--lim > 0 && (c = getc(fp)) != EOF && c != '\n') {
		*s++ = c;
		i++;
	}
	/*if (c == L'\n') {
		*s++ = c;
		i++;
	}*/
	*s = '\0';
	return i;
}
chain *add_block(chain *t, char *data)
{
	int hsh;
	chain *t1;

	hsh = rand();
	
	if (t != NULL && t->nxt != NULL) {
		add_block(t->nxt, data);
	} else if (t != NULL && t->nxt == NULL) {
		t1 = (chain *) malloc(sizeof(chain));
		sprintf(t1->prnt_hsh, "%s", t->hsh);
		sprintf(t1->hsh, "%d", hsh);
		t1->data = strdup(data);
		t1->prnt = t;
		t1->nxt = NULL;
		t->nxt = t1;
	} else {
		t = (chain *) malloc(sizeof(chain));
		strcpy(t->prnt_hsh, "genesis");
		sprintf(t->hsh, "%d", hsh);
		t->data = strdup(data);
		t->prnt = NULL;
		t->nxt = NULL;
	}

	return t;
}
void print_chain(chain *t, int recursive, int is_sub)
{
	int is_sub_t;

	is_sub_t = is_sub;
	if (t != NULL) {
		while (is_sub_t--)
			printf("\t");
		is_sub_t = is_sub;
		printf ("Parent Hash     :\t%s\n", t->prnt_hsh);
		while (is_sub_t--)
			printf("\t");
		is_sub_t = is_sub;
		printf ("Hash            :\t%s\n", t->hsh);
		while (is_sub_t--)
			printf("\t");
		is_sub_t = is_sub;
		printf ("Data            :\t%s\n", t->data);
		if (recursive > 0) {
			if (t->nxt != NULL) {
				/*while (is_sub_t--)
					printf("\t");*/
				print_chain (t->nxt, 1, (is_sub+1));
			} else {
				/*while (is_sub_t--)
					printf("\t");
				printf("\n\nno more next\n\n");*/
			}
		} else {
			printf("\n\n");
		}
	} else {
		printf("chain is Null\n");
	}
}