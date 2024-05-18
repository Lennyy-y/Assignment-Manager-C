#include "User.h"

#include <string.h>
const char* userTypes[NumOfEnum] = { "Professor", "Student" };
void printUser(const void* u)
{
    const User* pUser = (const User*)u;

    printf("Name: %s\tType: %s\tEmail: %s\tPassword: %s\n", pUser->name, userTypes[pUser->type], pUser->email, "********");
}
int compareUserByEmail(const void* u1, const void* u2)
{
    const User* pU1 = (const User*)u1;
    const User* pU2 = (const User*)u2;
    return (strcmp(pU1->email, pU2->email));
}

int saveUserToBinaryFile(FILE* fh, User* u)
{
    int nameLen = (int)strlen(u->name) + 1;
    int emailLen = (int)strlen(u->email) + 1;
    int passLen = (int)strlen(u->password) + 1;
    if (fwrite(&u->type, sizeof(int), 1, fh) != 1)
        return 0;
    if (fwrite(&nameLen, sizeof(int), 1, fh) != 1)
        return 0;
    if (fwrite(u->name, sizeof(char), nameLen, fh) != nameLen)
        return 0;
    if (fwrite(&emailLen, sizeof(int), 1, fh) != 1)
        return 0;
    if (fwrite(u->email, sizeof(char), emailLen, fh) != emailLen)
        return 0;
    if (fwrite(&passLen, sizeof(int), 1, fh) != 1)
        return 0;
    if (fwrite(u->password, sizeof(char), passLen, fh) != passLen)
        return 0;
    return 1;
}

int loadUserFromBinaryFile(FILE* fh, User* u)
{
    int nameLen, emailLen, passLen;
    if (fread(&u->type, sizeof(int), 1, fh) != 1)
        return 0;
    if (fread(&nameLen, sizeof(int), 1, fh) != 1)
        return 0;
    u->name = (char*)malloc(nameLen * sizeof(char));
    if (!u->name)
        return 0;
    if (fread(u->name, sizeof(char), nameLen, fh) != nameLen)
    {
        free(u->name);
        return 0;
    }
    if (fread(&emailLen, sizeof(int), 1, fh) != 1)
    {
        free(u->name);
        return 0;
    }
    u->email = (char*)malloc(emailLen * sizeof(char));
    if (!u->email)
    {
        free(u->name);
        return 0;
    }
    if (fread(u->email, sizeof(char), emailLen, fh) != emailLen)
    {
        free(u->name);
        free(u->email);
        return 0;
    }

    if (fread(&passLen, sizeof(int), 1, fh) != 1)
    {
        free(u->name);
        free(u->email);
        return 0;
    }
    u->password = (char*)malloc(passLen * sizeof(char));
    if (!u->password)
    {
        free(u->name);
        free(u->email);
        return 0;
    }
    if (fread(u->password, sizeof(char), passLen, fh) != passLen)
    {
        free(u->name);
        free(u->email);
        free(u->password);
        return 0;
    }

    return 1;
}

void freeUser(void* a)
{
    User* u = (User*)a;
    free(u->email);
    free(u->name);
    free(u->password);
    free(a);
}