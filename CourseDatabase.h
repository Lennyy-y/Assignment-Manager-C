#pragma once
#include "Course.h"
#include "list.h"

typedef struct {
    int courseCount;
    LIST courseList;
}CourseDatabase;

void initCourseDatabase(CourseDatabase* cDBP);
int addCourse(CourseDatabase* cDBP, Course* coP);
void printAllCourses(CourseDatabase* cDBP);
int compareCourseByName(const void* c1, const void* c2);
int compareCourseByCode(const void* c1, const void* c2);
Course* findCourse(CourseDatabase* cDBP, char* code);
Course* findCourseInArray(Course** cArr, int count, char* courseCode);
void deleteCourse(CourseDatabase* cDbP, Course* coP, void (*freeFunc)(void*));
int saveCourseDatabaseToBinaryFile(char* fileName, CourseDatabase* cDB);
int saveCourseCodeArrayToBinaryFile(FILE* fp, Course** cArr, int count);
int loadCourseDatabaseFromBinaryFile(char* fileName, CourseDatabase* cDB, UserDatabase* db);
int loadCourseCodeArrayFromBinaryFile(FILE* fp, CourseDatabase* cDBP, Course** cArr, int count);

void freeCourseDatabase(CourseDatabase* cDBP);