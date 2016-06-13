
/* File name: bst.c
 * Purpose: This is a complete implimentaion of a binary search tree to store
 * student ids for kit205 Assignment 1.
 *
 * Author: Daniel Richardson
 */

#include "bst.h"

static BST find_min(BST self);
static BST create_new_node(long student_id);

/* find_min: Helper function to find the smallest largest value from the left
 * branch of the tree. This is only visiable from within this source file.
 *
 * Param: The parents right child in which to find the smallest value from.
 *
 * Returns: the BST node with the smallest value from the left branch.
 */
static BST find_min(BST self)
{
	if (self == NULL) {
		return NULL;
	}
	if (self->left == NULL) {
		return self;
	}
	return find_min(self->left);
}

/* create_new_node: Create a new node with the value of 'student_id'. Sets the
 * children to NULL. This is a helper function only visiable to this source file.
 *
 * Param: the student's id number that is to be added to the BST node.
 *
 * Returns: the newly created node to the caller.
 */
static BST create_new_node(long student_id)
{
	BST new_node = malloc(sizeof(*new_node));
	new_node->student_id = student_id;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

/* number_of_elements_in_BST: Count the number of nodes inside the binary search
 * tree. This is useful for summerizing how many students are enrolled in course.
 *
 * Param: root of the tree to count the number of nodes contained within.
 *
 * Returns: a count of all nodes inside a tree.
 */
int number_of_elements_in_BST(BST self)
{
	int count;
	if (self == NULL) {
        return 0;
	}
    count = 1 + number_of_elements_in_BST(self->left) +
    number_of_elements_in_BST(self->right);

	return count;
}

/* add_element_to_BST: Adds a student id to the binary tree creating new nodes
 * and linking them as necessary.
 *
 * Params: the BST and the student id to add to the BST.
 *
 * Returns: void.
 */
BST add_element_to_BST(BST self, long student_id)
{
    if (self == NULL) {
        return create_new_node(student_id);
    }

    if (student_id < self->student_id) {
        self->left = add_element_to_BST(self->left, student_id);

    } else {
        self->right = add_element_to_BST(self->right, student_id);
    }
    return self;
}

/* delete_element_from_BST: Searches the BST for the target(student_id), if it
 * is found, then remove it from the BST. Returns the new BST back to the caller.

 * Params: the BST to delete an item from and target to search the BST for.

 * Returns: the BST once the delection is complete.
 */
BST delete_element_from_BST(BST self, long target)
{
	if (self == NULL) {
		return NULL;
	}

	if (target < self->student_id) {
		self->left = delete_element_from_BST(self->left, target);

	} else if (target > self->student_id) {
		self->right = delete_element_from_BST(self->right, target);


	} else {
		if (self->left == NULL) {
			BST temp = self->right;
			free(self);
			self = NULL;
			return temp;

		} else if (self->right == NULL) {
			BST temp = self->left;
			free(self);
			self = NULL;
			return temp;

        } else {
		BST temp = find_min(self->right);
		self->student_id = temp->student_id;
		self->right = delete_element_from_BST(self->right, temp->student_id);
        }
	}
	return self;
}

/* inorder: Prints an inorder list of all elements stored inside the BST
 *
 * Params: the BST in which to print all nodes inorder.
 *
 * Returns: void.
 */
void inorder(BST self, void (print)(long value))
{
    if (self != NULL) {
        inorder(self->left, print);
        (print)(self->student_id);
        inorder(self->right, print);
    }
}

/* search: this searches an entire BST for a student.
*
* Params: the BST to search and the student_id to search for.
*
* Returns: true if the student_id has been found in the BST and false otherwise.
*/
bool search_BST(BST self, long student_id)
{
	if (self != NULL) {
		if (self->student_id == student_id) {
			return true;

		} else if (self->student_id > student_id) {
			return search_BST(self->left, student_id);
		} else {
			return search_BST(self->right, student_id);
		}
	}
	return false;
}

/* destroy_tree: Destroys an entire BST.
 *
 * Params: the BST to delete.
 *
 * Returns: void.
 */
void destroy_tree(BST *self)
{
	if (*self != NULL) {
		destroy_tree(&(*self)->left);
		destroy_tree(&(*self)->right);
		free(*self);
		*self = NULL;
	}
}
