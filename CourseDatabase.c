#include "CourseDatabase.h"
#include <stdlib.h>

void initCourseDatabase(CourseDatabase* cDBP)
{
    cDBP->courseCount = 0;
    L_init(&cDBP->courseList);
}
int addCourse(CourseDatabase* cDbP, Course* coP)
{
    const NODE* pSearch = L_find(cDbP->courseList.head.next, coP, compareCourseByCode);
    if (pSearch)
        return 0;
    NODE* pNode = &cDbP->courseList.head;
    pNode = L_insert(pNode, coP);
    if (!pNode)
    {
        L_free(&cDbP->courseList, freeCourse);
        return 0;
    }
    cDbP->courseCount++;
    return 1;
}
void printAllCourses(CourseDatabase* cDbP)
{
    printf("There are %d courses in the database.\n", cDbP->courseCount);
    L_print(&cDbP->courseList, printCourseInfo);
}

int compareCourseByName(const void* c1, const void* c2)
{
    Course* course1 = (Course*)c1;
    Course* course2 = (Course*)c2;
    return strcmp(course1->courseName, course2->courseName);
}

int compareCourseByCode(const void* c1, const void* c2)
{
    Course* course1 = (Course*)c1;
    Course* course2 = (Course*)c2;
    return strcmp(course1->courseCode, course2->courseCode);
}

Course* findCourse(CourseDatabase* cDBP, char* code)
{
    if (cDBP->courseCount == 0)
        return NULL;
    Course tempSearch;
    strcpy(tempSearch.courseCode, code);
    const NODE* pRes = L_find(cDBP->courseList.head.next, &tempSearch, compareCourseByCode);
    if (pRes->key)
        return (Course*)pRes->key;
    else
        return NULL;
}

Course* findCourseInArray(Course** cArr, int count, char* courseCode)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(cArr[i]->courseCode, courseCode) == 0)
            return cArr[i];
    }
    return NULL;
}

void deleteCourse(CourseDatabase* cDbP, Course* coP, void (*freeFunc)(void*))
{
    NODE* pNode = &cDbP->courseList.head;
    while (pNode->next)
    {
        if (pNode->next->key == coP)
        {
            L_delete(pNode, freeCourse);
        }
    }

}

int saveCourseDatabaseToBinaryFile(char* fileName, CourseDatabase* cDB)
{
    FILE* fp = fopen(fileName, "wb");
    if (!fp)
    {
        return 0;
    }
    if (fwrite(&cDB->courseCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    NODE* pNode = &cDB->courseList.head;
    while (pNode->next)
    {
        if (!saveCourseToBinaryFile(fp, pNode->next->key))
        {
            fclose(fp);
            return 0;
        }
        pNode = pNode->next;
    }
    fclose(fp);
    return 1;
}

int saveCourseCodeArrayToBinaryFile(FILE* fp, Course** cArr, int count)
{
    int codeLen = CODE_LEN + 1;
    for (int i = 0; i < count; i++)
    {
        if (fwrite(cArr[i]->courseCode, sizeof(char), codeLen, fp) != codeLen)
        {
            return 0;
        }
    }
    return 1;
}

int loadCourseDatabaseFromBinaryFile(char* fileName, CourseDatabase* cDB, UserDatabase* db)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        return 0;
    }
    if (fread(&cDB->courseCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    NODE* pNode = &cDB->courseList.head;
    for (int i = 0; i < cDB->courseCount; i++)
    {
        Course* temp = (Course*)malloc(sizeof(Course));
        if (!loadCourseFromBinaryFile(fp, temp, db))
        {
            freeCourseDatabase(cDB);
            fclose(fp);
            return 0;
        }
        pNode = L_insert(pNode, temp);
    }
    fclose(fp);
    return 1;
}

int loadCourseCodeArrayFromBinaryFile(FILE* fp, CourseDatabase* cDBP, Course** cArr, int count)
{
    char tempCode[CODE_LEN + 1];
    Course* res;
    for (int i = 0; i < count; i++)
    {
        if (fread(tempCode, sizeof(char), CODE_LEN + 1, fp) != CODE_LEN + 1)
        {
            freeCourseArr(cArr, count);
            return 0;
        }
        res = findCourse(cDBP, tempCode);
        if (!res)
            return 0;
        cArr[i] = (Course*)malloc(sizeof(Course));
        if (!cArr[i])
        {
            freeCourseArr(cArr, count);
            return 0;
        }
        cArr[i] = res;
    }
    return 1;
}

void freeCourseDatabase(CourseDatabase* cDBP)
{
    L_free(&cDBP->courseList, freeCourseV);
}
