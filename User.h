#pragma once
#include "def.h"
#include "UserAuthentication.h"

typedef enum {
    professor, student, NumOfEnum
} userType;

typedef struct
{
    userType type;
    char* name;
    char* email;
    char* password;
}User;


const char* userTypes[NumOfEnum];
void printUser(const void* u);
int compareUserByEmail(const void* u1, const void* u2);
int saveUserToBinaryFile(FILE* fh, User* u);
int loadUserFromBinaryFile(FILE* fh, User* u);
void freeUser(void* a);