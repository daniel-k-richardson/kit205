/* File name: courselist.c
* Purpose: This handles a linked-list of string names and a AVL of  long ids.
* This is for kit205 assignment 1 Part B. NOTE: this file has been designed so
* that it can be reused in other projects.
*
* Author: Daniel Richardson
*/

#include "courselist.h"

/* add_course: Creates and initalizes a new CourseList node with the name of the
* course. Sets the students AVL to NULL, its next node to NULL and inserts
* the new node at the front of the linked-list.
*
* Params: CourseList to add the node to, and the course name of the new node.
*
* Returns: void.
*/
void insert_into_linkedlist(CourseList *self, char *course_name)
{
	/* Create a new node for the link-list. */
	CourseList new_node = malloc(sizeof(*new_node));

	/* Allocate memory for 'data' field inside CourseList and assign its name */
	new_node->data.name = malloc((strlen(course_name) + 1) * sizeof(char));
	strcpy(new_node->data.name, course_name);
	new_node->data.students = NULL;
	new_node->next = NULL;

	/* links the new node to the front of the linked list. */
	new_node->next = *self;
	*self = new_node;
}

/* remove_node: Removes a node from the linked-list. Relinks previous->next to
* current's next and frees current node.
*
* Params: CourseList to search for delection. Target (the name of the item to
* remove).
*
* Returns: void.
*/
void remove_node_from_linkedlist(CourseList *self, char *target)
{
	CourseList current = *self;
	CourseList previous = NULL;

	while (current != NULL) {
		if (strcmp(current->data.name, target) == 0) {
			/* Check if the node is the first element in the linked-list. */
			if (previous == NULL) {
				*self = current->next;

				free(current->data.name);
				current->data.name = NULL;

				free(current);
				current = *self;

				/* remove target from n'th position (general case). */
			} else {
				previous->next = current->next;

				free(current->data.name);
				current->data.name = NULL;
				free(current);

				current = previous->next;
			}
			/* Target node not yet fround, traverse, updating current and previous */
		} else {
			previous = current;
			current = current->next;
		}
	}
}

/* search_linkedlist: Searches the link-list to find the 'target' course.
*
* Params: CourseList to search through and the target name to find.
*
* Returns: the CourseList node of the matching course name back to the
* caller, otherwise it returns NULL.
*/
CourseList search_linkedlist(CourseList self, char *target)
{
	while (self != NULL) {
		if (strcmp(self->data.name, target) == 0) {
			return  self;
		} else {
			self = self->next;
		}
	}
	return NULL;
}