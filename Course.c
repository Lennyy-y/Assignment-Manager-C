#include "Course.h"
#include "def.h"
#define MAX_STR_LEN 255
int initCourse(Course* coP)
{
    char bufferCatch[MAX_STR_LEN] = {0};

    coP->assignedStudentCount = 0;
    coP->assignedStudents = (Student**)malloc(sizeof(Student*));
    if (!coP->assignedStudents)
        return 0;
    coP->courseName = getStrExactLength("Please enter a name for the course.");
    printf("Enter a code for the course (code length must be %d)\n", CODE_LEN);
    int validLen = 1;
    do {
        validLen = 1;
        scanf("%s", bufferCatch);
        if (strlen(bufferCatch) != CODE_LEN) {
            validLen = 0;
            printf("Invalid code length, try again.\n");
        }
    } while (validLen == 0);
    strcpy(coP->courseCode, bufferCatch);
    return 1;
}

int addStudentToCourse(Course* coP, Student* stP)
{
    if (coP->assignedStudentCount == 0)
    {
        coP->assignedStudents = (Student**)malloc(sizeof(Student*));
    }
    else{
        coP->assignedStudents = (Student**)realloc(coP->assignedStudents, sizeof(Student*)*(coP->assignedStudentCount + 1));
    }
    if (!coP->assignedStudents)
        return 0;
    coP->assignedStudents[coP->assignedStudentCount++] = stP;
    return 1;
}

void printCourseInfo(const void* p)
{
    Course* coP = (Course*)p;
    printf("Course name: %s\tCourse code: %s\tAssigned student count: %d.\n", coP->courseName, coP->courseCode, coP->assignedStudentCount);
}

void printCourseArray(const Course** cArr, int count)
{
    for (int i = 0 ; i < count ; i++)
    {
        printCourseInfo(cArr[i]);
    }
}

int saveCourseToBinaryFile(FILE* fp, Course* coP)
{
    int nameLen = (int)strlen(coP->courseName) + 1;
    int codeLen = CODE_LEN + 1;
    if (fwrite(&coP->assignedStudentCount, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(coP->courseCode, sizeof(char), codeLen, fp) != codeLen)
        return 0;
    if (fwrite(&nameLen, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(coP->courseName, sizeof(char), nameLen, fp) != nameLen)
        return 0;
    int emailLen;
    for (int i = 0 ; i < coP->assignedStudentCount ; i++)
    {
        emailLen = (int)strlen(coP->assignedStudents[i]->studentEmail) + 1;
        if (fwrite(&emailLen, sizeof(int), 1, fp) != 1)
            return 0;
        char* email = coP->assignedStudents[i]->userInfo->email;
        if (fwrite(email, sizeof(char), emailLen, fp) != emailLen)
            return 0;
    }
    return 1;
}

int loadCourseFromBinaryFile(FILE* fp, Course* coP, UserDatabase* db) {

    if (fread(&coP->assignedStudentCount, sizeof(int), 1, fp) != 1)
        return 0;
    int codeLen = CODE_LEN + 1;
    int nameLen;
    if (fread(&coP->courseCode, sizeof(char), codeLen, fp) != codeLen)
        return 0;
    if (fread(&nameLen, sizeof(int), 1, fp) != 1)
        return 0;
    coP->courseName = (char*)malloc(nameLen* sizeof(char));
    if (!coP->courseName)
        return 0;
    if (fread(coP->courseName, sizeof(char), nameLen, fp) != nameLen) {
        free(coP->courseName);
        return 0;
    }
    char* tempEmail;
    int emailLen;
    if (coP->assignedStudentCount != 0)
    {
        coP->assignedStudents = (Student**)malloc(sizeof(Student*) * coP->assignedStudentCount);
        if (!coP->assignedStudents)
        {
            free(coP->courseName);
            return 0;
        }
    for (int i = 0 ; i < coP->assignedStudentCount ; i++)
    {
        if (fread(&emailLen, sizeof(int), 1, fp) != 1) {
            for(int j = 0 ; j < i ; i++)
            {
                free(coP->assignedStudents[i]);
            }
            free(coP->assignedStudents);
            free(coP->courseName);
            return 0;
        }
        coP->assignedStudents[i] = (Student*)malloc(sizeof(Student));
        if (!coP->assignedStudents[i])
        {
            for(int j = 0 ; j < i ; i++)
            {
                free(coP->assignedStudents[i]);
            }
            free(coP->assignedStudents);
            free(coP->courseName);
            return 0;
        }

        tempEmail = (char*) malloc(sizeof(char)*emailLen);
        if (!tempEmail)
            return 0;
        if (fread(tempEmail, sizeof(char), emailLen, fp) != emailLen)
        {
            for(int j = 0 ; j < i ; i++)
            {
                free(coP->assignedStudents[i]);
            }
            free(coP->assignedStudents);
            free(coP->courseName);
            return 0;
        }
        coP->assignedStudents[i]->userInfo = findExistingUser(tempEmail, db);
        coP->assignedStudents[i]->studentEmail = coP->assignedStudents[i]->userInfo->email;
        free(tempEmail);
        }
    }
    return 1;
}

void freeCourse(Course* coP)
{
    if (coP->assignedStudentCount != 0)
    {
        free(coP->assignedStudents);
    }
    free(coP->courseName);
}

void freeCourseV(void* a)
{
    freeCourse((Course*)a);
}

void freeCourseArr(Course** cArr, int size)
{
    for (int i = 0 ; i < size ; i++)
    {
        freeCourse(cArr[i]);
        free(cArr[i]);
    }
}
