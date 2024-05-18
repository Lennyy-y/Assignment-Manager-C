#pragma once
#include "CourseDatabase.h"
#include "def.h"
#include "Date.h"
#define MAX_DATE_LEN 10
#define LATE_PENALTY 10
typedef struct {
    Course* assignmentCourse;
    Date turnInDate;
    Date dueDate;
    char* assignmentName;
    BOOL isComplete;
    double assignmentGrade;
    BOOL canTurnInLate;
} Assignment;

void initAssignment(Assignment* aP, Course* coP);
void printAssignment(const Assignment* aP);
int saveAssignmentToBinaryFile(FILE* fp, Assignment* aP);
int loadAssignmentFromBinaryFile(FILE* fp, CourseDatabase* cDBP, Assignment* aP);
void freeAssignment(Assignment* aP);
