#ifndef COURSELIST_H
#define COURSELIST_H

#include "bst.h"

typedef struct course
{
    char *name;
    BST students;
} Course;

typedef struct courseNode
{
    Course data;
    struct courseNode *next;
} *CourseList;

CourseList search_linkedlist(CourseList self, char *target);
void insert_into_linkedlist(CourseList *self, char *course_name);
void remove_node_from_linkedlist(CourseList *self, char *target);

#endif
