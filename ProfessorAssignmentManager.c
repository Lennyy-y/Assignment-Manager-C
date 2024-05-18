#include "ProfessorAssignmentManager.h"
#define SELECTION_OFFSET 1

void initProfessorAssignmentManager(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP, Professor* p)
{
    initAssignmentManager(&pAMP->pAssignmentManager);
    pAMP->authoredCourseCount = 0;
    pAMP->prof = p;
    if (pAMP->authoredCourseCount == 0) {
        addAuthoredCourses(pAMP, cDBP);
    }

}

void addAuthoredCourses(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP)
{
    char tempCode[CODE_LEN + 1];
    printAllCourses(cDBP);
    printf("Please type the codes of the courses you teach from the list (maximum of %d) or type 0 to stop:\n",
        MAX_AUTHORED_COURSES);
    while ((tempCode[0] != '0') && (pAMP->authoredCourseCount < MAX_AUTHORED_COURSES)) 
    {
        scanf("%s", tempCode);
        if (tempCode[0] == '0')
            break;
        if (strlen(tempCode) != CODE_LEN)
        {
            printf("Invalid code length, code length must be %d.\n", CODE_LEN);
            continue;
        }
        Course tempSearch;
        strcpy(tempSearch.courseCode, tempCode);
        const NODE* pRes;
        pRes = L_find(cDBP->courseList.head.next, &tempSearch, compareCourseByCode);

        if (!pRes)
        {
            printf("Course code not found.\n");
        }
        else
        {
            if (pAMP->authoredCourseCount == 0)
            {
                pAMP->authoredCourses = (Course**)malloc(sizeof(Course*));
            }
            else
            {
                pAMP->authoredCourses = (Course**)realloc(pAMP->authoredCourses, sizeof(Course*) * (pAMP->authoredCourseCount + 1));
            }
            if (!pAMP->authoredCourses)
                return;
            Course* coP = (Course*)pRes->key;
            pAMP->authoredCourses[pAMP->authoredCourseCount++] = coP;
            printf("Added course \"%s\" to %s's authored course list.\n", coP->courseName, pAMP->prof->userInfo->name);
            if (pAMP->authoredCourseCount == MAX_AUTHORED_COURSES)
            {
                printf("Max authored course count reached.\n");
            }
        }
    }
}

void gradeAssignment(Assignment* aP)
{
    time_t now = time(NULL);
    struct tm current;
    current = *localtime(&now);
    Date currentDate;
    currentDate.year = current.tm_year + 1900;
    currentDate.month = current.tm_mon + 1;
    currentDate.day = current.tm_mday;
    if (aP->canTurnInLate == 0 && ((aP->isComplete && compareDates(&aP->turnInDate, &aP->dueDate) > 0) || (!aP->isComplete && compareDates(&currentDate, &aP->dueDate) > 0)))
    {
        aP->assignmentGrade = 0.0;
        printf("Assignment due date has passed, grade has been set to 50.\n");
        return;

        // turn in = 2026, due = 2025
    }
    else if (aP->isComplete) {
        double grade;
        int valid;
        printf("Enter a grade for the assignment (must be from 0 to 100):\n");
        do {
            valid = 1;
            scanf("%lf", &grade);
            if (grade < 0.0 || grade > 100.0) {
                printf("Invalid input, try again.\n");
                valid = 0;
            }
        } while (valid == 0);
        if (aP->canTurnInLate && compareDates(&aP->turnInDate, &aP->dueDate) > 0)
        {
            printf("Deducting %d points from the grade because the assignment was turned in late.\n", LATE_PENALTY);
            aP->assignmentGrade = (grade - (double)LATE_PENALTY);
        }
        else
            aP->assignmentGrade = grade;
    }
    else if (compareDates(&aP->dueDate, &currentDate) >= 0)
    {
        printf("Assignment due date did not pass, cannot grade it yet.\n");
    }

}
Assignment* addNewAssignment(ProfessorAssignmentManager* pAMP, CourseDatabase* cDBP)
{
    Assignment* newAssignment = (Assignment*)malloc(sizeof(Assignment));
    if (!newAssignment)
        return NULL;
    char courseCode[CODE_LEN + 1] = { 0 };
    Course* res;
    printCourseArray((const Course**)pAMP->authoredCourses, pAMP->authoredCourseCount);
    printf("Please type the code of the course for the assignment.\n");
    do
    {
        myGets(courseCode, CODE_LEN + 1);
        if (strlen(courseCode) != CODE_LEN) {
            printf("Invalid code length, code length must be %d", CODE_LEN);
        }
        res = findCourseInArray(pAMP->authoredCourses, pAMP->authoredCourseCount, courseCode);
        if (!res)
        {
            printf("Course %s not found, please try again.\n", courseCode);
        }
    } while (!res);
    initAssignment(newAssignment, res);
    if (pAMP->pAssignmentManager.assignmentCount == 0)
    {
        pAMP->pAssignmentManager.allAssignments = (Assignment*)malloc(sizeof(Assignment));
    }
    else
    {
        pAMP->pAssignmentManager.allAssignments = (Assignment*)realloc(pAMP->pAssignmentManager.allAssignments, sizeof(Assignment) * (pAMP->pAssignmentManager.assignmentCount + 1));
    }
    if (!pAMP->pAssignmentManager.allAssignments)
        return NULL;

    pAMP->pAssignmentManager.allAssignments[pAMP->pAssignmentManager.assignmentCount++] = *newAssignment;
    free(newAssignment);
    return &pAMP->pAssignmentManager.allAssignments[pAMP->pAssignmentManager.assignmentCount - 1];
}

void printProfessorAssignmentManager(const ProfessorAssignmentManager* pAMP)
{
    printf("Assignment manager for professor %s:\n", pAMP->prof->userInfo->name);
    printf("Assignments created: %d\n", pAMP->pAssignmentManager.assignmentCount);
    printf("Courses taught this semester: %d\n", pAMP->authoredCourseCount);
}

int saveProfessorAssignmentManagerToBinaryFile(FILE* fp, ProfessorAssignmentManager* pAMP)
{
    if (!saveAssignmentManagerToBinaryFile(fp, &pAMP->pAssignmentManager))
        return 0;
    if (!saveProfessorToBinaryFile(fp, pAMP->prof))
        return 0;
    if (fwrite(&pAMP->authoredCourseCount, sizeof(int), 1, fp) != 1)
        return 0;
    if (pAMP->authoredCourseCount != 0)
    {
        if (!saveCourseCodeArrayToBinaryFile(fp, pAMP->authoredCourses, pAMP->authoredCourseCount))
            return 0;
    }

    return 1;
}

int saveProfessorAssignmentManagerArrayToBinaryFile(char* fileName, ProfessorAssignmentManager* pAMPArr, int count)
{
    FILE* fp = fopen(fileName, "wb");
    if (!fp)
    {
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        if (!saveProfessorAssignmentManagerToBinaryFile(fp, &pAMPArr[i]))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int loadProfessorAssignmentManagerFromBinaryFile(FILE* fp, ProfessorAssignmentManager* pAMP, UserDatabase* db, CourseDatabase* cDBP)
{
    if (!loadAssignmentManagerFromBinaryFile(fp, cDBP, &pAMP->pAssignmentManager))
        return 0;
    pAMP->prof = (Professor*)malloc(sizeof(Professor));
    if (!pAMP->prof)
        return 0;
    if (!loadProfessorFromBinaryFile(fp, pAMP->prof, db))
        return 0;
    if (fread(&pAMP->authoredCourseCount, sizeof(int), 1, fp) != 1)
        return 0;
    if (pAMP->authoredCourseCount != 0)
    {
        pAMP->authoredCourses = (Course**)malloc(sizeof(Course*) * pAMP->authoredCourseCount);
        if (!pAMP->authoredCourses)
        {
            freeAssignmentManager(&pAMP->pAssignmentManager);
            freeProfessor(pAMP->prof);
        }
        if (!loadCourseCodeArrayFromBinaryFile(fp, cDBP, pAMP->authoredCourses, pAMP->authoredCourseCount))
            return 0;
    }

    return 1;
}

int loadProfessorAssignmentManagerArrayFromBinaryFile(char* fileName, ProfessorAssignmentManager* pAMPArr, UserDatabase* db, CourseDatabase* cDBP, int count)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        if (!loadProfessorAssignmentManagerFromBinaryFile(fp, &pAMPArr[i], db, cDBP))
        {
            for (int j = 0; j < i; j++)
            {
                freeProfessorAssignmentManager(&pAMPArr[i]);
            }
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

void freeProfessorAssignmentManager(ProfessorAssignmentManager* pAMP)
{
    freeAssignmentManager(&pAMP->pAssignmentManager);
    free(pAMP->prof);
}
