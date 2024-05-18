#include "Professor.h"
#define printProf(str) printf("Professor name:%s\n",str)

int initProfessor(Professor* prP, User* u) {
    prP->userInfo = u;
    prP->professorEmail = u->email;
    return 1;

}

int saveProfessorToBinaryFile(FILE* fp, Professor* prP)
{
    int emailLen = (int)strlen(prP->professorEmail) + 1;
    if (fwrite(&emailLen, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(prP->professorEmail, sizeof(char), emailLen, fp) != emailLen)
        return 0;
    return 1;
}

int loadProfessorFromBinaryFile(FILE* fp, Professor* prP, UserDatabase* db)
{
    int emailLen;
    if (fread(&emailLen, sizeof(int), 1, fp) != 1)
        return 0;
    prP->professorEmail = (char*)malloc(sizeof(char) * emailLen);
    if (!prP->professorEmail)
        return 0;
    if (fread(prP->professorEmail, sizeof(char), emailLen, fp) != emailLen)
        return 0;
    prP->userInfo = (User*)malloc(sizeof(User));
    if (!prP->userInfo)
    {
        free(prP->professorEmail);
        return 0;
    }
    prP->userInfo = findExistingUser(prP->professorEmail, db);
    return 1;
}

void printProfessorInfo(Professor* prP)
{
    printProf(prP->userInfo->name);
}

void freeProfessor(Professor* prP)
{
    freeUser(prP->userInfo);
    free(prP->professorEmail);
}
