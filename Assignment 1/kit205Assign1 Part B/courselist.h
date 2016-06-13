
#ifndef COURSELIST_H
#define COURSELIST_H

#include "avl.h"

typedef struct course
{
	char *name;
	AVL students;
} Course;

typedef struct courseNode
{
	Course data;
	struct courseNode *next;
} *CourseList;

void insert_into_linkedlist(CourseList *self, char *course_name);
void remove_node_from_linkedlist(CourseList *self, char *target);
CourseList search_linkedlist(CourseList self, char *target);

#endif