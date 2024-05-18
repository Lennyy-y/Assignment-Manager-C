#pragma once
#include "AssignmentManager.h"
#include "Professor.h"
#include "CourseDatabase.h"
#define MAX_AUTHORED_COURSES 3

typedef struct {
    AssignmentManager pAssignmentManager;
    Professor* prof;
    Course** authoredCourses;
    int authoredCourseCount;
}ProfessorAssignmentManager;

void initProfessorAssignmentManager(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP, Professor* p);
void addAuthoredCourses(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP);
void gradeAssignment(Assignment* aP);
Assignment* addNewAssignment(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP);
void printProfessorAssignmentManager(const ProfessorAssignmentManager* pAMP);
int saveProfessorAssignmentManagerToBinaryFile(FILE* fp, ProfessorAssignmentManager* pAMP);
int saveProfessorAssignmentManagerArrayToBinaryFile(char* fileName, ProfessorAssignmentManager* pAMPArr, int count);
int loadProfessorAssignmentManagerFromBinaryFile(FILE* fp, ProfessorAssignmentManager* pAMP, UserDatabase* db, CourseDatabase* cDBP);
int loadProfessorAssignmentManagerArrayFromBinaryFile(char* fileName, ProfessorAssignmentManager* pAMPArr, UserDatabase* db, CourseDatabase* cDBP, int count);
void freeProfessorAssignmentManager(ProfessorAssignmentManager* pAMP);