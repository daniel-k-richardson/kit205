
/* File name: AVL.c
* Purpose: This is a complete implimentaion of an AVL tree to store
* student ids for kit205 Assignment 1 this is part B of the assignment.
*
* Author: Daniel Richardson
*/

#include "avl.h"

/* internal functions that the courselist and main does not need access to */
static AVL find_min(AVL self);
static AVL create_new_node(long student_id);
static AVL rebalance_AVL(AVL self, int student_id);
static int check_balance(AVL self);
static AVL right_rotate(AVL parent);
static AVL left_rotate(AVL parent);
static int height(AVL self);
static int maximum(int a, int b);

/* create_new_node: Create a new node with the value of 'student_id'. Sets the
* children to NULL and starting height at 1.
*
* Param: the student's id number that is to be added to the AVL node.
*
* Returns: the newly created node to the caller.
*/
static AVL create_new_node(long value)
{
	AVL new_node = malloc(sizeof(*new_node));
	new_node->student_id = value;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 1;

	return new_node;
}

/* add_element_to_AVL: Adds a student id to the AVL tree creating new nodes
* and linking them as necessary. This also checks that the tree is still balanced
* after a student has been added.
*
* Params: the AVL and the student id to add to the AVL.
*
* Returns: the Root of the AVL.
*/
AVL add_element_to_AVL(AVL self, long student_id)
{
	if (self == NULL) {
		return create_new_node(student_id);
	}
	if (student_id < self->student_id) {
		self->left = add_element_to_AVL(self->left, student_id);

	} else {
		self->right = add_element_to_AVL(self->right, student_id);
	}

	self = rebalance_AVL(self, student_id);

	return self;
}

/* rebalance_AVL: checks if the left and right branches of the tree are even,
* if they are not. Check which rotation needs to be carried out and do it.
*
* Params: the AVL root node, and last value entered.
*
* Returns: a newly balanced tree if rebalancing is needed, or just returns
* the current root if no balancing is necessary.
*/
static AVL rebalance_AVL(AVL self, int value)
{
	int balance;

	/* update the height of the ancestor */
	self->height = maximum(height(self->left), height(self->right)) + 1;

	/* check that the left and right subtrees are balanced. */
	balance = check_balance(self);

	/* check for left rotate */
	if (balance > 1 && value < self->student_id) {
		return right_rotate(self);
	}
	/* check for right rotate */
	if (balance < -1 && value > self->student_id) {
		return left_rotate(self);
	}
	/* check for double rotate (left then right) */
	if (balance > 1 && value > self->left->student_id) {
		self->left = left_rotate(self);
		return right_rotate(self);
	}
	/* check for double rotate (right then left) */
	if (balance < -1 && value < self->right->student_id) {
		self->right = right_rotate(self);
		return left_rotate(self);
	}
	return self;
}

/* check_balance: this function counts the height of rthe left and right subtrees
* and checks if they are balanaced.
*
* Params: the AVL parent node to check for balance.
*
* Returns: 0 if both branches are equal, otherwise a negative value if left
* branch is heigher or a positive value if the right side is heigher.
*/
static int check_balance(AVL self)
{
	if (self == NULL) {
		return 0;
	}
	return height(self->left) - height(self->right);
}

/* right_rotation: This rotates an un-balanced left subtree by rotating it right.
* e.g.
*
* Before:   (parent)    After: (child)
*           /                  /      \
*       (child)       (grand Child)  (parent)
*        /
*   (grand child)
*
* Params: the AVL parent that needs to be reblanced.
*
* Returns: the balanced AVL tree.
*/
static AVL right_rotate(AVL parent)
{
	AVL temp = parent;
	AVL child = parent->left;
	AVL grandchild = child->left;

	/* swap nodes */
	parent = child;
	temp->left = parent->right;
	parent->right = temp;

	/* update height of AVL tree */
	parent->height = maximum(height(parent->left), height(parent->right)) + 1;

	return child;
}

/* left_rotate: rotates the left unbalanced substree right.
*
*   before  (parent)                    After:  (child)
*                   \                           /      \
*                   (child)                 (parent)    (grand child)
*                       \
*                       (grand child)
*
* Parmas: the AVL parent that needs to be rebalanced.
*
* Returns: a balanced AVL tree.
*/
static AVL left_rotate(AVL parent)
{
	AVL temp = parent;
	AVL child = parent->right;
	AVL grandchild = child->right;

	/* swap nodes */
	parent = child;
	temp->right = parent->left;
	parent->left = temp;

	/* update tree height */
	parent->height = maximum(height(parent->left), height(parent->right)) + 1;
	
	return parent;
}

/* delete_element_from_AVL: searches the AVL tree for the target(student_id), if
it is found, then remove it from the AVL. Once the student has been deleted
check and rebalance the tree..
*
* Params: the AVL to delete an item from and the item to remove.
*
* Returns: a new AVL tree with the item removed.
*/
AVL delete_element_from_AVL(AVL self, long target)
{
	if (self == NULL) {
		return NULL;
	}

	/*find the target node */
	if (target < self->student_id) {
		self->left = delete_element_from_AVL(self->left, target);

	} else if (target > self->student_id) {
		self->right = delete_element_from_AVL(self->right, target);

	} else {

		/* found student to remove check if it has only one child */
		if (self->left == NULL) {
			AVL temp = self->right;
			free(self);
			self = NULL;
			return temp;

		} else if (self->right == NULL) {
			AVL temp = (self)->left;
			free(self);
			self = NULL;
			return temp;
		} else {
			/* AVL node to remove has two children */

			AVL temp = find_min(self->right);
			int smallest_large_value = temp->student_id;

			/* assign smallest largest value to target and remove leaf node */
			self->student_id = temp->student_id;
			self->right = delete_element_from_AVL(self->right, temp->student_id);

			/* check height and reblance if needed */
			self = rebalance_AVL(self, smallest_large_value);
		}
	}
	return self;
}

/* find_min: Helper function to find the smallest largest value from the left
* branch of the tree. This is only visiable from within this source file.
*
* Param: The parents right child in which to find the smallest value from.
*
* Returns: the AVL node with the smallest value from the left branch.
*/
static AVL find_min(AVL self)
{
	if (self == NULL) {
		return NULL;
	}
	if (self->left == NULL) {
		return self;
	}
	return find_min(self->left);
}

/* height: just a getter function of convenience.
*
* Params: get the height stored in a node.
*
* Returns: height value stored in node.
*/
static int height(AVL self)
{
	int left_height = 0;
	int right_height = 0;

	if (self == NULL) {
		return left_height > right_height ? left_height : right_height;
	}
	
	left_height = height(self->left);
	right_height = height(self->right);
}

/* maximum: takes two int value and returns the largest of the two.
*
*Param: two int values to compare.
*
* Returns: the largest of the two values.
*/
static int maximum(int a, int b)
{
	return (a > b) ? a : b;
}

/* search_AVL: this searches an entire AVL for a student.
*
* Params: the AVL to search and the value to search for.
*
* Returns: true if the student_id has been found in the AVL and false otherwise.
*/
bool search_AVL(AVL self, long target)
{
	if (self != NULL) {
		if (self->student_id == target) {
			return true;

		} else if (self->student_id > target) {
			return search_AVL(self->left, target);
		} else {
			return search_AVL(self->right, target);
		}
	}
	return false;
}

/* inorder: Prints an inorder list of all elements stored inside the AVL
*
* Params: the AVL in which to print all nodes inorder.
*
* Returns: void.
*/
void inorder(AVL self)
{
	if (self != NULL) {
		inorder(self->left);
		printf("%d\n",self->student_id);
		inorder(self->right);
	}
}

/* number_of_elements_in_AVL: Count the number of nodes inside the AVL
* tree. This is useful for summerizing how many students are enrolled in course.
*
* Param: root of the tree to count the number of nodes contained within.
*
* Returns: a count of all nodes inside a tree.
*/
int number_of_elements_in_AVL(AVL self)
{
	int count;

	if (self == NULL) {
		return (0);
	}

	count = 1 + number_of_elements_in_AVL(self->left) +
		number_of_elements_in_AVL(self->right);

	return (count);
}

/* destroy_tree: Destroys an entire AVL.
*
* Params: the AVL to delete.
*
* Returns: void.
*/
void destroy_tree(AVL *self)
{
	if (*self != NULL) {
		destroy_tree(&(*self)->left);
		destroy_tree(&(*self)->right);
		free(*self);
		*self = NULL;
	}
}