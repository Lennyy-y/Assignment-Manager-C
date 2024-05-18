#include "Student.h"
#include <time.h>
#include <stdlib.h>

void initStudent(Student* stP, User* u)
{
    int r = rand() % 9000 + 1000;
    stP->userInfo = u;
    stP->studentEmail = u->email;
    stP->id = r;
}



void printStudentInfo(Student* stP)
{
    printf("Student name: %s\t\tID: %d\n", stP->userInfo->name, stP->id);
}

int saveStudentToBinaryFile(FILE* fp, Student* sP)
{
    if (fwrite(&sP->id, sizeof(int), 1, fp) != 1)
        return 0;
    int emailLen = (int)(strlen(sP->studentEmail) + 1);
    if (fwrite(&emailLen, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(sP->studentEmail, sizeof(char), emailLen, fp) != emailLen)
        return 0;
    return 1;
}

int saveStudentArrayToBinaryFile(FILE* fp, Student* sArr, int count)
{
    if (fwrite(&count, sizeof(int), 1, fp) != 1)
    {
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        if (!saveStudentToBinaryFile(fp, &sArr[i]))
        {
            return 0;
        }
    }
    return 1;
}

int loadStudentFromBinaryFile(FILE* fp, Student* sP, UserDatabase* db)
{
    if (fread(&sP->id, sizeof(int), 1, fp) != 1)
        return 0;
    int emailLen;
    if (fread(&emailLen, sizeof(int), 1, fp) != 1)
        return 0;
    sP->studentEmail = (char*)malloc(sizeof(char) * emailLen);
    if (!sP->studentEmail)
        return 0;
    if (fread(sP->studentEmail, sizeof(char), emailLen, fp) != emailLen)
        return 0;
    sP->userInfo = (User*)malloc(sizeof(User));
    if (!sP->userInfo)
    {
        free(sP->studentEmail);
        return 0;
    }
    sP->userInfo = findExistingUser(sP->studentEmail, db);
    return 1;
}

int loadStudentArrFromBinaryFile(FILE* fp, Student* sArr, int* pCount, UserDatabase* db)
{
    if (fread(pCount, sizeof(int), 1, fp) != 1)
    {
        return 0;
    }
    for (int i = 0; i < *pCount; i++)
    {
        if (!loadStudentFromBinaryFile(fp, &sArr[i], db))
        {
            freeStudentArr(sArr, i);
            return 0;
        }
    }
    return 1;
}

void freeStudent(Student* stP)
{
    freeUser(stP->userInfo);
    free(stP->studentEmail);
}

void freeStudentArr(Student* sArr, int count)
{
    for (int i = 0; i < count; i++)
        freeStudent(&sArr[i]);
}