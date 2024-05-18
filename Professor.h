#pragma once
#include "UserDatabase.h"


typedef struct {
    User* userInfo;
    char* professorEmail;
} Professor;

int initProfessor(Professor* prP, User* u);
int saveProfessorToBinaryFile(FILE* fp, Professor* prP);
int loadProfessorFromBinaryFile(FILE* fp, Professor* prP, UserDatabase* db);
void printProfessorInfo(Professor* prP);
void freeProfessor(Professor* prP);
