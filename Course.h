#pragma once
#include "Student.h"
#define MAX_STR_LEN 255


#define CODE_LEN 5
typedef struct {
    int assignedStudentCount;
    char courseCode[CODE_LEN + 1];
    Student** assignedStudents;
    char* courseName;
}Course;

int initCourse(Course* coP);
void printCourseInfo(const void* p);
void printCourseArray(const Course** cArr, int count);
int addStudentToCourse(Course* coP, Student* stP);
int saveCourseToBinaryFile(FILE* fp, Course* coP);
int loadCourseFromBinaryFile(FILE* fp, Course* coP, UserDatabase* db);
void freeCourse(Course* coP);
void freeCourseV(void* a);
void freeCourseArr(Course** cArr, int size);