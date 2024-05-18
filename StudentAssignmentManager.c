#include "StudentAssignmentManager.h"

#define SELECTION_OFFSET 1

void initStudentAssignmentManager(StudentAssignmentManager* sAMP, CourseDatabase* cDBP, Student* stu)
{
    initAssignmentManager(&sAMP->sAssignmentManager);
    sAMP->signedCoursesCount = 0;
    sAMP->stu = stu;
    sAMP->completeAssignmentCount = 0;
    if (cDBP->courseCount != 0)
        signUpToCourse(sAMP, cDBP);
}

int addAssignmentFromProfessor(StudentAssignmentManager* sAMP, Assignment* aP)
{
    if (sAMP->sAssignmentManager.assignmentCount != 0)
    {
        sAMP->sAssignmentManager.allAssignments = (Assignment*)realloc(sAMP->sAssignmentManager.allAssignments, sizeof(Assignment) * (sAMP->sAssignmentManager.assignmentCount + 1));
    }
    else
    {
        sAMP->sAssignmentManager.allAssignments = (Assignment*)malloc(sizeof(Assignment));
    }
    if (!sAMP->sAssignmentManager.allAssignments)
        return 0;
    sAMP->sAssignmentManager.allAssignments[sAMP->sAssignmentManager.assignmentCount] = *aP;
    sAMP->sAssignmentManager.assignmentCount++;

    return 1;
}

void showUnturnedIn(StudentAssignmentManager* sAMP)
{
    printf("Incomplete assignments:\n");
    for (int i = 0; i < sAMP->sAssignmentManager.assignmentCount; i++)
    {
        if (sAMP->sAssignmentManager.allAssignments[i].isComplete == False)
        {
            printf("%d) ", i + 1);
            printAssignment(&sAMP->sAssignmentManager.allAssignments[i]);
        }
    }
}

void showTurnedIn(StudentAssignmentManager* sAMP)
{
    printf("Complete assignments:\n");
    for (int i = 0; i < sAMP->sAssignmentManager.assignmentCount; i++)
    {
        if (sAMP->sAssignmentManager.allAssignments[i].isComplete == True)
        {
            printf("%d) ", i + 1);
            printAssignment(&sAMP->sAssignmentManager.allAssignments[i]);
        }
    }
}

void turnIn(StudentAssignmentManager* sAMP, Assignment* aP)
{
    time_t now = time(NULL);
    struct tm* current;
    current = localtime(&now);
    printf("Uploading assignment file...");

    Assignment* temp = findAssignment(&sAMP->sAssignmentManager, aP, compareAssignmentsByName);
    if (temp)
    {
        temp->isComplete = True;
        temp->turnInDate.day = current->tm_mday;
        temp->turnInDate.month = current->tm_mon + 1;
        temp->turnInDate.year = current->tm_year + 1900;
        sAMP->completeAssignmentCount++;
        printf("\nAssignment has been turned in.\nUpload date: ");
        printDate(&aP->turnInDate);
        printf("\n");
    }
}

void printStudentAssignmentManager(const StudentAssignmentManager* sAMP)
{
    printf("Assignment manager for student %s:\n", sAMP->stu->userInfo->name);
    printf("Assignment count (completed): %d (%d)\n", sAMP->sAssignmentManager.assignmentCount, sAMP->completeAssignmentCount);
    printf("Signed course count: %d\n", sAMP->signedCoursesCount);
}

int signUpToCourse(StudentAssignmentManager* sAMP, CourseDatabase* cDBP)
{
    char selection[CODE_LEN + 1];
    Course tempSearch;
    const NODE* result;
    if (sAMP->signedCoursesCount < MAX_COURSES_SIGNED) {
        printAllCourses(cDBP);
        while (sAMP->signedCoursesCount < MAX_COURSES_SIGNED || selection[0] != '0') {
            printf("Please enter the codes of the courses you wish to sign up to for the coming semester (maximum of %d), type 0 to stop.:\n",
                MAX_COURSES_SIGNED);
            myGets(selection, CODE_LEN + 1);
            if (selection[0] == '0')
                break;
            if (strlen(selection) != CODE_LEN)
            {
                printf("Invalid code length, code length must be %d.\n", CODE_LEN);
                continue;
            }
            if (sAMP->signedCoursesCount == 0)
            {
                sAMP->allCourses = (Course**)malloc(sizeof(Course*));
            }
            else
            {
                sAMP->allCourses = (Course**)realloc(sAMP->allCourses, sizeof(Course*) * (sAMP->signedCoursesCount + 1));
            }
            if (!sAMP->allCourses)
                return 0;
            strcpy(tempSearch.courseCode, selection);
            result = L_find(cDBP->courseList.head.next, &tempSearch, compareCourseByCode);
            if (result)
            {
                Course* res = (Course*)result->key;
                sAMP->allCourses[sAMP->signedCoursesCount++] = res;
                addStudentToCourse(res, sAMP->stu);
                printf("Successfully signed up to course \"%s\".\n", res->courseName);
            }
            else
            {
                printf("Course code not found.\n");
            }
        }
        return 1;
    }
    else {
        printf("Maximum course capacity reached, please wait until the next semester.\n");
        return 0;
    }
}

int saveStudentAssignmentManagerToBinaryFile(FILE* fp, StudentAssignmentManager* sAMP)
{
    if (!saveAssignmentManagerToBinaryFile(fp, &sAMP->sAssignmentManager))
        return 0;
    if (!saveStudentToBinaryFile(fp, sAMP->stu))
        return 0;
    if (fwrite(&sAMP->signedCoursesCount, sizeof(int), 1, fp) != 1)
        return 0;
    if (!saveCourseCodeArrayToBinaryFile(fp, sAMP->allCourses, sAMP->signedCoursesCount))
        return 0;
    if (fwrite(&sAMP->completeAssignmentCount, sizeof(int), 1, fp) != 1)
        return 0;

    return 1;
}

int saveStudentAssignmentManagerArrayToBinaryFile(char* fileName, StudentAssignmentManager* sAMPArr, int count)
{
    FILE* fp = fopen(fileName, "wb");
    if (!fp)
    {
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        if (!saveStudentAssignmentManagerToBinaryFile(fp, &sAMPArr[i]))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int loadStudentAssignmentManagerFromBinaryFile(FILE* fp, StudentAssignmentManager* sAMP, UserDatabase* db, CourseDatabase* cDBP)
{
    if (!loadAssignmentManagerFromBinaryFile(fp, cDBP, &sAMP->sAssignmentManager))
        return 0;
    sAMP->stu = (Student*)malloc(sizeof(Student));
    if (!sAMP->stu)
        return 0;
    if (!loadStudentFromBinaryFile(fp, sAMP->stu, db))
        return 0;
    if (fread(&sAMP->signedCoursesCount, sizeof(int), 1, fp) != 1)
        return 0;
    if (sAMP->signedCoursesCount != 0)
    {
        sAMP->allCourses = (Course**)malloc(sizeof(Course*) * sAMP->signedCoursesCount);
        if (!sAMP->allCourses)
        {
            freeAssignmentManager(&sAMP->sAssignmentManager);
            free(sAMP->stu);
            return 0;
        }
        if (!loadCourseCodeArrayFromBinaryFile(fp, cDBP, sAMP->allCourses, sAMP->signedCoursesCount))
            return 0;
    }

    if (fread(&sAMP->completeAssignmentCount, sizeof(int), 1, fp) != 1)
        return 0;
    return 1;
}

int loadStudentAssignmentManagerArrayFromBinaryFile(char* fileName, StudentAssignmentManager* sAMPArr, UserDatabase* db, CourseDatabase* cDBP, int count)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        return 0;
    }

    for (int i = 0; i < count; i++)
    {

        if (!loadStudentAssignmentManagerFromBinaryFile(fp, &sAMPArr[i], db, cDBP))
        {
            for (int j = 0; j < i; j++)
            {
                freeStudentAssignmentManager(&sAMPArr[i]);
            }
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

void freeStudentAssignmentManager(StudentAssignmentManager* sAMP)
{
    free(sAMP->stu);
}