#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct AVLNode
{
	long student_id;
	struct AVLNode *left;
	struct AVLNode *right;
	int height;
} *AVL;

int number_of_elements_in_AVL(AVL self);
AVL add_element_to_AVL(AVL self, long student_id);
AVL delete_element_from_AVL(AVL self, long target);
void inorder(AVL self);
bool search_AVL(AVL self, long student_id);
void destroy_tree(AVL *self);

#endif

