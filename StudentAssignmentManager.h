#pragma once
#include "AssignmentManager.h"
#include "CourseDatabase.h"


typedef struct {
    AssignmentManager sAssignmentManager;
    Student* stu;
    Course** allCourses;
    int signedCoursesCount;
    int completeAssignmentCount;
}StudentAssignmentManager;

void initStudentAssignmentManager(StudentAssignmentManager* sAMP, CourseDatabase* cDBP, Student* stu);
int addAssignmentFromProfessor(StudentAssignmentManager* sAMP, Assignment* aP);
void showUnturnedIn(StudentAssignmentManager* sAMP);
void showTurnedIn(StudentAssignmentManager* sAMP);
void turnIn(StudentAssignmentManager* sAMP, Assignment* aP);
void printStudentAssignmentManager(const StudentAssignmentManager* sAMP);
double calculateAvg(StudentAssignmentManager* sAMP);
int signUpToCourse(StudentAssignmentManager* sAMP, CourseDatabase* cDBP);
int saveStudentAssignmentManagerToBinaryFile(FILE* fp, StudentAssignmentManager* sAMP);
int saveStudentAssignmentManagerArrayToBinaryFile(char* fileName, StudentAssignmentManager* sAMPArr, int count);
int loadStudentAssignmentManagerFromBinaryFile(FILE* fp, StudentAssignmentManager* sAMP, UserDatabase* db, CourseDatabase* cDBP);
int loadStudentAssignmentManagerArrayFromBinaryFile(char* fileName, StudentAssignmentManager* sAMPArr, UserDatabase* db, CourseDatabase* cDBP, int count);
void freeStudentAssignmentManager(StudentAssignmentManager* sAMP);