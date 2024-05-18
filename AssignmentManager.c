#include "AssignmentManager.h"

void initAssignmentManager(AssignmentManager* aMP)
{
    aMP->assignmentCount = 0;
}

int compareAssignmentsByDateTime(const void* a1, const void* a2)
{
    Assignment* tmp1 = (Assignment*)a1;
    Assignment* tmp2 = (Assignment*)a2;
    if (tmp1->dueDate.year != tmp2->dueDate.year)
        return tmp1->dueDate.year - tmp2->dueDate.year;
    else if (tmp1->dueDate.month != tmp2->dueDate.month)
        return tmp1->dueDate.month - tmp2->dueDate.month;
    else if (tmp1->dueDate.day != tmp2->dueDate.day)
        return tmp1->dueDate.day - tmp2->dueDate.day;
    return 0;
}

int compareAssignmentsByName(const void* a1, const void* a2)
{
    Assignment* tmp1 = (Assignment*)a1;
    Assignment* tmp2 = (Assignment*)a2;

    return (strcmp(tmp1->assignmentName, tmp2->assignmentName));
}

int compareAssignmentsByCourse(const void* a1, const void* a2)
{
    Assignment* tmp1 = (Assignment*)a1;
    Assignment* tmp2 = (Assignment*)a2;
    return (strcmp(tmp1->assignmentCourse->courseCode, tmp2->assignmentCourse->courseCode));
}

Assignment* findAssignment(AssignmentManager* aMP, const void* key, int (*compare)(const void*, const void*))
{
    qsort(aMP->allAssignments, aMP->assignmentCount, sizeof(Assignment), *compare);
    return (Assignment*)bsearch(key, aMP->allAssignments, aMP->assignmentCount, sizeof(Assignment), *compare);
}

void sortAssignments(AssignmentManager* aMP, int (*compare)(const void*, const void*))
{
    qsort(aMP->allAssignments, aMP->assignmentCount, sizeof(Assignment), *compare);
}

void printAssignmentManager(const AssignmentManager* aMP)
{
    for (int i = 0; i < aMP->assignmentCount; i++)
    {
        printf("%d) ", i + 1);
        printAssignment(&aMP->allAssignments[i]);
        printf("\n");
    }
}

int saveAssignmentManagerToBinaryFile(FILE* fp, AssignmentManager* aMP)
{
    if (fwrite(&aMP->assignmentCount, sizeof(int), 1, fp) != 1)
    {
        return 0;
    }
    for (int i = 0; i < aMP->assignmentCount; i++)
    {
        if (!saveAssignmentToBinaryFile(fp, &aMP->allAssignments[i]))
        {
            return 0;
        }
    }
    return 1;
}

int loadAssignmentManagerFromBinaryFile(FILE* fp, CourseDatabase* cDBP, AssignmentManager* aMP)
{
    if (fread(&aMP->assignmentCount, sizeof(int), 1, fp) != 1)
    {
        return 0;
    }
    if (aMP->assignmentCount != 0)
    {
        aMP->allAssignments = (Assignment*)malloc(aMP->assignmentCount * sizeof(Assignment));
        if (!aMP->allAssignments)
            return 0;

        for (int i = 0; i < aMP->assignmentCount; i++) {
            if (!loadAssignmentFromBinaryFile(fp, cDBP, &aMP->allAssignments[i])) {
                freeAssignmentManager(aMP);
                return 0;
            }
        }
    }
    return 1;
}

void freeAssignmentManager(AssignmentManager* aMP)
{
    if (aMP->assignmentCount != 0)
    {
        for (int i = 0; i < aMP->assignmentCount; i++)
        {
            freeAssignment(&aMP->allAssignments[i]);

        }
        free(aMP->allAssignments); 
    }
}