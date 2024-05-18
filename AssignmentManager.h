#pragma once
#include "Assignment.h"
#include "CourseDatabase.h"


typedef struct {
    int assignmentCount;
    Assignment* allAssignments;
}AssignmentManager;

void initAssignmentManager(AssignmentManager* aMP);
int compareAssignmentsByDateTime(const void* a1, const void* a2);
int compareAssignmentsByName(const void* a1, const void* a2);
int compareAssignmentsByCourse(const void* a1, const void* a2);
Assignment* findAssignment(AssignmentManager* aMP, const void* key, int (*compare)(const void*, const void*));
void sortAssignments(AssignmentManager* aMP, int (*compare)(const void*, const void*));
void printAssignmentManager(const AssignmentManager* aMP);
int saveAssignmentManagerToBinaryFile(FILE* fp, AssignmentManager* aMP);
int loadAssignmentManagerFromBinaryFile(FILE* fp, CourseDatabase* cDBP, AssignmentManager* aMP);
void freeAssignmentManager(AssignmentManager* aMP);
