#include "grades.h"
#include "linked-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum {SUCCESS=0, FAIL=-1, MAX_GRADE=100, MIN_GRADE=0};


/*structs definitions*/
struct student {
    
    char *student_name;
    int student_id;
    struct list* student_courses_list;
    //we added these two parameters to save running in avg calc.
    int grades_sum;
    int grades_count;

};

struct course {

    char *course_name;
    int course_grade;


};

struct grades {

    struct list *student_list;

} ;


/*grades functions decleration*/
struct grades* grades_init();
void grades_destroy(struct grades *grades);
int grades_add_student(struct grades *grades, const char *name, int id);
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade);
float grades_calc_avg(struct grades *grades, int id, char **out);
int grades_print_student(struct grades *grades, int id);
int grades_print_all(struct grades *grades);


/*student functions decleration*/
struct student* student_create(char *name, int id);
int student_clone(void *element, void **out);
void student_destroy(void *element);
float student_get_avg(struct student *student);
int student_add_grade(struct student *student);
int search_student (struct grades *grades, int id, struct iterator **iterator_out);

/*course functions decleration*/
struct course* course_create(char *name, int grade);
void course_destroy(void *element);
int course_clone(void *element, void **out);
int course_list_copy(struct list *source_list,struct list *dest_list);
int search_course (struct student *student, char *name, struct iterator **iterator_out);





/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init(){

	/*memory allocation*/
    struct grades *curr_grades = (struct grades*)malloc(sizeof(struct grades));
    if(curr_grades == NULL){
        return NULL;
    }

    /*creating pointers to functions destroy and clone*/
    int(*clone_t)(void *element, void **output) =student_clone;
    void(*destroy_t)(void *element)=student_destroy;
    curr_grades->student_list = list_init(clone_t,destroy_t);

    if(curr_grades->student_list == NULL){
        return NULL;
    }
    return curr_grades;
}


/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades){
    list_destroy(grades->student_list);
    free(grades);
}

/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with 
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){

    struct iterator* iterator_out =NULL;

    /*If the student allready exists, return fail.*/
    if(search_student(grades, id, &iterator_out)==SUCCESS){
        return FAIL;
    }

    char *new_name= (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(new_name,name);

    struct student *new_student = student_create(new_name, id);
    free(new_name);
    if (new_student == NULL){
        return FAIL;
    }

    /*Enter new student to the end of the list*/
    if(list_push_back(grades->student_list,new_student)==SUCCESS){
        free(new_student->student_name);
        list_destroy(new_student->student_courses_list);
        free(new_student);
        return SUCCESS;
    }

    free(new_student->student_name);
    list_destroy(new_student->student_courses_list);
    free(new_student);
    return FAIL;
}

/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){

    if(grades == NULL) {
        return FAIL;
    }

    struct iterator *student_iterator =NULL;
    /*If student does not exist, return fail.*/
    if(search_student(grades, id, &student_iterator)!=SUCCESS){
        return FAIL;
    }

    struct student* curr_student = (struct student*)list_get(student_iterator);
    struct iterator *course_iterator =NULL;

    char *new_name= (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(new_name,name);
    if(!search_course(curr_student ,new_name, &course_iterator)){
        free(new_name);
        return FAIL;
    }

    /*If grde is invalid return fail.*/
    if(grade<MIN_GRADE || grade>MAX_GRADE){
        free(new_name);
        return FAIL;
    }

    curr_student->grades_sum += grade;
    curr_student->grades_count++;

    struct course* new_course = course_create(new_name, grade);
    free(new_name);

    /*Add the course to the list*/
    if(list_push_back(curr_student->student_courses_list,new_course)==SUCCESS){
        free(new_course->course_name);
        free(new_course);
        return SUCCESS;
    }

    free(new_course->course_name);
    free(new_course);
    return FAIL;


}

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "*out" to NULL.
 * @note "out" is a pointer to char*. Meaning, your function should
 * allocate memory on the heap and set "*out" to point on that memory.
 * This methodology (of returning values via pointers) is very common in C.
 * An example of C method that returns a value via pointer:
 * void foo(int *out) {
 *   *out = 1;  
 * }
 */
float grades_calc_avg(struct grades *grades, int id, char **out){

    struct iterator *iterator_out =NULL;
    *out =NULL;

    /*If student does not exist, return fail.*/
    if(search_student(grades, id, &iterator_out)){
        return FAIL;
    }

    struct student *curr_student = (struct student*)list_get(iterator_out);
    if(curr_student == NULL){
        return FAIL;
    }

    *out = (char*)malloc(sizeof(char)*(strlen(curr_student->student_name)+1));
    if( *out == NULL){
        return FAIL;
    }
    strcpy(*out,curr_student->student_name);

    /*if student doesnt have any course, we will consider his avg as 0*/
    if(curr_student->grades_count==0){
        return 0;
    }

    float tmp_avg = (float)curr_student->grades_sum/curr_student->grades_count;
    return tmp_avg;

}

/**
*@brief an auxiliary for the printing functions.
*gets a struct of student, and prints his info.
*void function- does not return any value.
*/
void grades_print_student_aux(struct student *student_to_print){

    struct iterator *iterator = list_begin(student_to_print -> student_courses_list);
    struct course *course_to_print;
    printf("%s %d:",student_to_print->student_name, student_to_print->student_id);
    while (iterator != NULL){
        course_to_print = (struct course*)list_get(iterator);
        if(iterator == list_begin(student_to_print->student_courses_list)){
            printf(" %s %d", course_to_print->course_name,course_to_print->course_grade);
        }
        else{
            printf("%s %d", course_to_print->course_name,course_to_print->course_grade);
        }
        iterator = list_next(iterator);
        if(iterator !=NULL){
            printf(", ");
        }
    }
    printf("\n");


}

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id){

    if(grades == NULL){
        return FAIL;
    }
    struct iterator *iterator_out =NULL;

    if(search_student(grades, id, &iterator_out)){
        return FAIL;
    }

    struct student *student_to_print =(struct student*)list_get(iterator_out) ;
    grades_print_student_aux(student_to_print);
    return SUCCESS;

}

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order 
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades){

    if(grades == NULL) {
        return FAIL;
    }

    struct iterator *iterator = list_begin(grades->student_list);
    while(iterator != NULL) {
        grades_print_student_aux(list_get(iterator));
        iterator = list_next(iterator);
    }
    return SUCCESS;
}






/**
*@brief Create a new course struct.
* gets a name and a grade.
* returns a pointer to the new course.
*/
struct course* course_create(char *name, int grade){

    struct course *curr_course = (struct course*)malloc(sizeof(struct course));
    if(curr_course == NULL){
        return NULL;
    }

    curr_course -> course_name = (char*)malloc((strlen(name)+1)*sizeof(char));
    if(curr_course -> course_name == NULL){
        return NULL;
    }
    strcpy(curr_course -> course_name, name);

    curr_course -> course_grade = grade;
    return curr_course;
}


/**
*@brief Frees all the allocated memory of a given course.
*/
void course_destroy(void* element){

    struct course *course = (struct course*)element;
    free(course->course_name);
    free(course);
};


/**
*@brief Gets a course element as void* and clone it for adding a new grade to student
*used for initializing lists.
*gets an element(an iterator) and an output(a course)
*returns 0 for success, -1 for fail.
*/
int course_clone(void *element, void **output){

    struct course *old_course = (struct course*)element;
    struct course *new_course = (struct course*)malloc(sizeof(struct course));
    if(new_course==NULL){
        return FAIL;
    }

    new_course->course_grade = old_course->course_grade;
    new_course->course_name = (char*)malloc((strlen(old_course->course_name)+1) * sizeof(char));
    if(new_course -> course_name == NULL){
        return FAIL;
    }
    strcpy(new_course -> course_name, old_course->course_name);

    *output= new_course;
    return SUCCESS;
}


/**
*@brief Used in clone function. Copies each element on the list to the new struct.
*gets s source and a destination list
*returns 0 for success, -1 for fail.
*/
int course_list_copy(struct list *source_list,struct list *dest_list){

    struct course *cloned_course = (struct course*)malloc(sizeof(struct course));
    if (cloned_course ==NULL){
        return FAIL;
    }

    /*going through the source list, clones each element, 
    and sends it to the new list*/
    struct iterator *iterator = list_begin(source_list);
    while (iterator != NULL){
        if((course_clone(list_get(iterator), (void*)cloned_course))){
            free (cloned_course);
            return FAIL;
        }
        if(list_push_back(dest_list, cloned_course)){
            free (cloned_course);
            return FAIL;
        }
        iterator = list_next(iterator);
    }

    free (cloned_course);
    return SUCCESS;
}

/**
*@brief Checks if a given course exists.
*returns 0 for success, -1 for fail.
*/
int search_course(struct student *student, char *name, struct iterator** iterator_out){

    if(student == NULL){
        return FAIL;
    }

    struct iterator *course_iterator = list_begin(student->student_courses_list);
    if (course_iterator == NULL){
        return FAIL;
    }
    char *course_name;
    /*run through all courses and check for a course with "name"*/
    while (course_iterator != NULL){
        course_name = ((struct course*)list_get(course_iterator))->course_name;
        //if exist
        if (strcmp(name,course_name) == SUCCESS) {
            *iterator_out=course_iterator;
            return SUCCESS;
        }
        course_iterator = list_next(course_iterator);
    }
    /*if finished searching and doesnt exist*/
    return FAIL;
}


/**
*@brief Create a new student struct.
* gets a name and an id.
* returns a pointer to the new student.
*/
struct student* student_create(char *name, int id){

    struct student *curr_student = (struct student*)malloc(sizeof(struct student));
    if(curr_student == NULL){
        return NULL;
    }

    curr_student -> student_name = (char*)malloc((strlen(name)+1)*sizeof(char));
    if(curr_student -> student_name == NULL){
        return NULL;
    }
    strcpy(curr_student -> student_name, name);

    curr_student -> student_id = id;

    int(*clone_t)(void *element, void **output) =course_clone;
    void(*destroy_t)(void *element)=course_destroy;

    curr_student ->student_courses_list = list_init(clone_t,destroy_t);

    curr_student->grades_sum=0;
    curr_student->grades_count=0;

    return curr_student;
}

/**
*@brief Frees all the allocated memory of a given students.
* inclufing his inner lists.
*/
void student_destroy(void* element){
    struct student *student = (struct student*)element;
    list_destroy(student->student_courses_list);
    free(student->student_name);
    free(student);
};

/**
*@brief Gets a student element as void* and clone it for adding a new student to grades
*used for initializing lists.
*gets an element(an iterator) and an output(a student)
*returns 0 for success, -1 for fail.
*/
int student_clone(void *element, void **out){
    struct student *old_student = (struct student*)element;
    struct student *new_student = (struct student*)malloc(sizeof(struct student));
    //if allocated , copy data
    if(new_student ==NULL){
        return FAIL;
    }

    new_student -> student_name = (char*)malloc((strlen(old_student->student_name)+1)*sizeof(char));
    if(new_student -> student_name == NULL){
        return FAIL;
    }

    strcpy(new_student -> student_name, old_student->student_name);
    new_student -> student_id = old_student->student_id;

    new_student->grades_sum = old_student -> grades_sum;
    new_student->grades_count = old_student -> grades_count;

    int(*clone_t)(void *element, void **output) =course_clone;
    void(*destroy_t)(void *element)=course_destroy;

    new_student ->student_courses_list = list_init(clone_t,destroy_t);

    course_list_copy(old_student->student_courses_list, new_student->student_courses_list);

    *out = new_student;
    return SUCCESS;
}


/**
*@brief Checks if a given student exists.
*returns 0 for success, -1 for fail.
*/
int search_student (struct grades *grades, int id, struct iterator** iterator_out){

    if (grades == NULL){
        return FAIL;
    }
    struct iterator *student_iterator = list_begin(grades->student_list);

    while (student_iterator != NULL){
        /* check found correct student*/
        if (((struct student*)list_get(student_iterator))->student_id == id){
            *iterator_out = student_iterator;
            return SUCCESS;
        }

        student_iterator = list_next(student_iterator);
    }

    return FAIL;
}
