#include <stdio.h>
#include <string.h>
#include <math.h>

/*
input data structure:
	["x", "y"],
	["x1", "y2"],
	["x3", "y4"],


*/


typedef struct _graph_input_tree {
	char *x;
	char *y;
	struct _graph_input_tree *next;
} Gtree;

Gtree *Gtree_push(Gtree *tree, char *x, char *y)
{
	if (tree != NULL) {
		tree->next = Gtree_push(tree->next, x, y);
	} else {
		tree = malloc(sizeof(Gtree));
		tree->x = x;
		tree->y = y;
		tree->next = NULL;
	}
	return tree;
}

Gtree *Gtree_pop(Gtree *tree, char *s, char axis, int recursive)
{
	Gtree *parent, *child;

	parent = child = tree;

	if (axis == 'x') {
		goto search_for_x;
	} else if (axis == 'y') {
		goto search_for_y;
	} else {
		goto end;
	}

search_for_x:

	while (child != NULL) {
		if (strcmp(s, child->x) == 0) {
			parent->next = child->next;
			if (!recursive)
				goto end;
		}
		parent = child;
		child = child->next;
	}

search_for_y:

	while (child != NULL) {
		if (strcmp(s, child->y) == 0) {
			parent->next = child->next;
			if (!recursive)
				goto end;
		}
		parent = child;
		child = child->next;
	}

end:
	return tree;
}

Gtree *int_list_to_Gtree(int list[][2], int len, int x_index, int y_index)
{
	int i, l;
	double num_len;
	char *x, *y;
	Gtree *tree;

	tree = NULL;

	for (i = 0; i < len; i += 1) {
		printf("%d\t%d\n", list[i][x_index], list[i][y_index]);
	}

	for (i = 0; i < len; i += 2) {
		printf("%d\n", i);

		printf("%d\t%d\n", list[i][x_index], list[i][y_index]);

		l = log10(list[i][x_index]);
		num_len = (l + 1) + 1;	/* 1 for '\0' */
		x = (char *) malloc(num_len * sizeof(char *));
		printf("%d\n", sprintf(x, "%d", list[i][x_index]));

		l = log10(list[i][y_index]);
		printf("%d\n", l);
		num_len = (l + 1) + 1;	/* 1 for '\0' */
		y = (char *) malloc(num_len * sizeof(char));
		sprintf(y, "%d", list[i][y_index]);

		printf("%s\t%s\n", x, y);

		tree = Gtree_push(tree, x, y);			/* append it in the tree */
	}
	return tree;
}

int print_gtree(Gtree *tree)
{
	while (tree != NULL) {
		printf("%s\t%s\n", tree->x, tree->y);
		tree = tree->next;
	}
}

main ()
{
	int list[6][2] = {
		{1, 2},
		{2, 3},
		{3, 4},
		{4, 5},
		{5, 6},
		{6, 7}
		};
	Gtree *tree = int_list_to_Gtree(list, 6, 0, 1);
	print_gtree(tree);
}
