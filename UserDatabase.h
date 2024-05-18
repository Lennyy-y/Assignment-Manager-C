#pragma once
#include "User.h"
#include "list.h"
#define MAX_PASS_ATTEMPTS 3

typedef struct
{
    int userCount;
    LIST userList;
}UserDatabase;

void initUserDatabase(UserDatabase* db);
User* signUp(UserDatabase* db);
User* login(UserDatabase* db);
int addUserToDatabase(User* u, UserDatabase* db);
User* findExistingUser(char* email, UserDatabase* db);
int validateMatchingPassword(char* password, User* u);
void printUserDatabase(const UserDatabase* db);
int saveUserDatabaseToBinaryFile(char* fileName, UserDatabase* db);
int loadUserDatabaseFromBinaryFile(char* fileName, UserDatabase* db);
void freeUserDatabase(UserDatabase* db);