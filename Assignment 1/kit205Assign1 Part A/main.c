/* File name: main.c
 * Purpose: This is the driver (or harness) file for 205 Assignment 1 Part A. 
 * This interacts with only the student_database.c.
 *
 * Author: Daniel Richardson
 */

#include "courselist.h"

#define BUFFER_SIZE 20
#define LOWEST_CHOICE 0
#define HEIGHEST_CHOICE 7
#define END_SESSION 0
//#define DISPLAY_TEXT	/* comment/uncomment this to toggle text prompts. */

/* Function prototypes */
void display_options();
int choice();
void selected_task(CourseList *self, int user_choice);
void mutable_print(char *msg);

static void print_information(long student_id);
void student_enrolled_courses(CourseList self, long student_id);
void enrol_student(CourseList *self, char *course, long student_id);
void unenrol_student(CourseList self, char *course, long student_id);
void traverse_students_enrolled(CourseList self, char *target);
void course_summary(CourseList self);
void destroy_all(CourseList *self);

/* Possible oporations that the program can do. Used in conjuction with switch */
enum {
    ADD_COURSE = 1,
    REMOVE_COURSE = 2,
    ENROL_STUDENT = 3,
    UN_ENROL_STUDENT = 4,
    COURSE_SUMMARY = 5,
    STUDENTS_ENROLLED_IN_COURSE = 6,
    LIST_OF_COURSES_ENROLED = 7
};

int main()
{
    CourseList courses = NULL;

    /* Assigned by choice(). Stores a value between 0-7 selected by user. */
    unsigned short user_choice;

    do {
        display_options();
        user_choice = choice();
        selected_task(&courses, user_choice);

    } while(user_choice != END_SESSION);

    if (courses != NULL) {
        destroy_all(&courses);
    }

    return EXIT_SUCCESS;
}

/* display_options: Displays the options that a user can pick and a small
 * description of each. NOTE: this can be toggled on and off by commenting out
 * or on by uncommenting the MACRO constant DISPLAY_TEXT.
 *
 * Params: none.
 *
 * Returns: void.
 */
void display_options()
{
#ifdef DISPLAY_TEXT

    printf("Please pick an option(1-7). 0 to exit.\n");
    printf("1. Add course\n");
    printf("2. Remove course\n");
    printf("3. Enrole student\n");
    printf("4. Un-enrol student\n");
    printf("5. Print summary of courses and the number of students enrolled");
    printf(" in each course\n");//
    printf("6. Print an ordered list of students enrolled in a course\n");
    printf("7. Print a list of courses that a given stuident is enrolled in\n");
#endif
}

/* choice: gets an int value between(0-7) from the user.
 *
 * Params: none.
 *
 * Returns: an int value between 0-7.
 */
int choice()
{
    int option_choice;

    do {
        scanf("%d", &option_choice);

    } while(option_choice < LOWEST_CHOICE || option_choice > HEIGHEST_CHOICE);

    return option_choice;
}

/* selected_task: This handles the option selected by the user. It checks the
 * choice of the user (an int between 1-7) inside a switch statement. Each case
 * gets more input from the user (student id and course name) then calls the
 * appropiate functions.
 *
 * Params: CourseList to carryout operation on. the number of the operation
 * to be carried out.
 *
 * Returns: void.
 */
void selected_task(CourseList *self, int user_choice)
{
    char buffer[BUFFER_SIZE];
    long student_id;

    switch (user_choice) {
        case ADD_COURSE:
            mutable_print("Please enter course name: ");
            scanf("%s", buffer);
            insert_into_linkedlist(self, buffer);
            break;

        case REMOVE_COURSE:
            mutable_print("Name of the course you'd like to remove? ");
            scanf("%s", buffer);
            remove_node_from_linkedlist(self, buffer);
            break;

        case ENROL_STUDENT:
            mutable_print("Enter course name: ");
            scanf("%s", buffer);
            mutable_print("Enter the student's id: ");
            scanf("%ld", &student_id);
            enrol_student(self, buffer, student_id);
            break;

        case UN_ENROL_STUDENT:
            mutable_print("Enter the course name to un-enrol student from: ");
            scanf("%s", buffer);
            mutable_print("Enter the student id: ");
            scanf("%ld", &student_id);
            unenrol_student(*self, buffer, student_id);
            break;

        case COURSE_SUMMARY:
            course_summary(*self);
            break;

        case STUDENTS_ENROLLED_IN_COURSE:
            mutable_print("Enter the course name you'd like students from: ");
            scanf("%s", buffer);
            traverse_students_enrolled(*self, buffer);
            break;

        case LIST_OF_COURSES_ENROLED:
            mutable_print("Please enter student's id number: ");
            scanf("%ld", &student_id);
            student_enrolled_courses(*self, student_id);
            break;
    }
}

/* mutable_print: this function is one of convinence. Rather than having a lot of
 * #ifdef inside the select_task function, I've decided it would be best to do it
 * this way. NOTE: whether or not a message will actually print depends on whether
 * or not DISPLAY_TEXT has been commented out or not.
 *
 * Params, msg is the message to display.
 *
 * Returns: void.
 */
void mutable_print(char *msg)
{
#ifdef DISPLAY_TEXT
    printf("%s\n", msg);
#endif
}


static void print_information(long student_id);

/* student_enrolled_courses: searches all the CourseList node's AVL trees for all
 * occurences of a target student's id and displays the course names where found.
 *
 * Params: CourseList to search through, and the student_id to find within each.
 *
 * Returns: void.
 */
void student_enrolled_courses(CourseList self, long student_id)
{
    while (self != NULL) {
        if (search_BST(self->data.students, student_id)) {
            printf("%s\n", self->data.name);
        }
        self = self->next;
    }
}

/* enrol_student: Adds a student to a course by finding the node in the linked
 * list by course name. Once the course is found, the student is added to the AVL
 * for that course.

 * Params: CourseList to search, the course name to find in the linked-list,
 * the student's id to add to the AVL once the course has been found.
 *
 * Returns: void.
 */
void enrol_student(CourseList *self, char *course, long student_id)
{
    /* Finds the course to add the student to. */
    CourseList temp = search_linkedlist(*self, course);

    /* once the CourseList node has been found, add student_id to it's AVL. */
    temp->data.students = add_element_to_BST(temp->data.students, student_id);
}


/* unenrol_student: Finds the CourseList node that contains course name that
 * student is to be removed from. Then removes the student_id from its AVL.
 *
 * Params: CourseList to search for course name in which the student is currently
 * enrolled. Then the student's id toto remove from the course's AVL of students.
 *
 * Returns: void.
 */
void unenrol_student(CourseList self, char *course, long student_id)
{
    CourseList tmp = search_linkedlist(self, course);
    tmp->data.students = delete_element_from_BST(tmp->data.students, student_id);
}

/* traverse_students_enrolled: This function finds the course with the same name
 * as the target. Then traverses through the target's AVL tree of students.
 *
 * Params: CourseList to search, the target course name to display all students
 * from.
 *
 * Returns: void.
 */
void traverse_students_enrolled(CourseList self, char *target)
{
    CourseList tmp = search_linkedlist(self, target);
    inorder(tmp->data.students, &print_information);
}

/* course_summary: Displays all the course names (from the linked list) and counts
 * the number of nodes inside the AVL representing students.
 *
 * Params: the CourseList to display summary of.
 *
 * Returns: void.
 */
void course_summary(CourseList self)
{
    int number_of_students;

    while (self != NULL) {
        number_of_students = number_of_elements_in_BST(self->data.students);
        printf("%s %d\n", self->data.name, number_of_students);
        self = self->next;
    }
}

/* print_information: this function is passed back to the AVL.c inorder() function
 * to print all students inside the AVL.
 *
 * Params: student id.
 *
 * Returns: void.
 */
static void print_information(long student_id)
{
    printf("%ld\n", student_id);
}


/* destory_all: destroys all courselist nodes and the AVL tree of students.
 *
 * Params: CourseList to destroy.
 *
 * Returns: void.
 */
void destroy_all(CourseList *self)
{
    CourseList current = NULL;

    while (*self != NULL) {
        current = (*self);

        destroy_tree(&current->data.students);
        *self = current->next;

        remove_node_from_linkedlist(&current, current->data.name);
    }
}