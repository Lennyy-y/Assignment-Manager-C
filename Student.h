#pragma once
#include "UserDatabase.h"
#define MAX_COURSES_SIGNED 7
typedef struct {
    int id;
    char* studentEmail;
    User* userInfo;
} Student;

void initStudent(Student* stP, User* u);
void printStudentInfo(Student* stP);
int saveStudentToBinaryFile(FILE* fp, Student* sP);
int saveStudentArrayToBinaryFile(FILE* fp, Student* sArr, int count);
int loadStudentFromBinaryFile(FILE* fp, Student* sP, UserDatabase* db);
int loadStudentArrFromBinaryFile(FILE* fp, Student* sArr, int* pCount, UserDatabase* db);
void freeStudent(Student* stP);
void freeStudentArr(Student** sArr, int count);