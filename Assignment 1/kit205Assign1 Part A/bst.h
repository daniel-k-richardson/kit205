#ifndef BST_H
#define BST_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct bstNode
{
	long student_id;
	struct bstNode *left;
	struct bstNode *right;
} *BST;

int number_of_elements_in_BST(BST self);

BST add_element_to_BST(BST self, long student_id);
BST delete_element_from_BST(BST self, long target);
void inorder(BST self, void (print)(long value));
bool search_BST(BST self, long student_id);
void destroy_tree(BST *self);

#endif