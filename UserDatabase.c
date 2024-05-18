#include "UserDatabase.h"


void initUserDatabase(UserDatabase* db)
{
    db->userCount = 0;
    L_init(&db->userList);
}

User* signUp(UserDatabase* db)
{
    User* temp = (User*)malloc(sizeof(User));
    temp->name = getStrExactLength("Please enter your name:");
    printf("Hi, %s! Are you a student or a professor?\n", temp->name);
    printf("Type 0 for professor or 1 for student.\n");
    int choice;
    scanf("%d", &choice);
    while (choice != 0 && choice != 1)
    {
        printf("Wrong selection, please try again.\n");
        scanf("%d", &choice);
    }
    temp->type = choice;
    int validFormat = 0;
    int validExisting = 0;
    char* emailBuffer = NULL;
    char* passwordBuffer;
    while (validFormat == 0 || validExisting == 0) {

        validFormat = 0;
        validExisting = 0;

        emailBuffer = getStrExactLength("Please enter your email address:");

        if (!validateEmailFormat(emailBuffer))
            printf("Invalid format, email format must be <name@domain.com>\n");
        else
            validFormat = 1;


        if (findExistingUser(emailBuffer, db) != NULL)
            printf("User with this email address already exists, please try a different one.\n");
        else
            validExisting = 1;
    }
    temp->email = myStrdup(emailBuffer);
    validFormat = 0;
    while (validFormat == 0)
    {
        passwordBuffer = getStrExactLength("Please choose a password (Minimum length 8 characters, only alphanumeric characters A-Z, a-z, 0-9):");
        if (!validatePasswordFormat(passwordBuffer)) {
            printf("Invalid format, try again.\n");
        }
        else
            validFormat = 1;

    }
    temp->password = myStrdup(passwordBuffer);
    addUserToDatabase(temp, db);
    return temp;
}

User* login(UserDatabase* db)
{
    if (db->userCount == 0)
    {
        printf("No users found.\n");
        return NULL;
    }
    int validFormat = 0;
    char* buffer;
    while (validFormat == 0) {

        validFormat = 0;

        buffer = getStrExactLength("Please enter your email address:");

        if (!validateEmailFormat(buffer))
            printf("Invalid format, please try again. (email format must be <name@domain.com>)\n");
        else
            validFormat = 1;

    }
    User* tmp = findExistingUser(buffer, db);
    if (!tmp) {
        printf("No user found with this email address, please try again or sign up.\n");
        return NULL;
    }
    for (int i = 0; i < MAX_PASS_ATTEMPTS; i++)
    {
        buffer = getStrExactLength("Please enter your password:");
        if (validateMatchingPassword(buffer, tmp)) {
            printf("Login successful, welcome back %s\n", tmp->name);
            return tmp;
        }
        else
            printf("Incorrect password, try again.\n");
    }
    printf("Too many incorrect attempts, please try again later.\n");
    return NULL;

}

int addUserToDatabase(User* u, UserDatabase* db)
{
    NODE* pNode = &db->userList.head;
    pNode = L_insert(pNode, u);
    if (!pNode)
    {
        L_free(&db->userList, (void*)freeUser);
        return 0;
    }
    db->userCount++;
    return 1;
}

User* findExistingUser(char* email, UserDatabase* db)
{
    if (db->userCount == 0)
        return NULL; // no User to find.
    User tempSearch = { 0, "",email, "" };
    const NODE* pRes = L_find(db->userList.head.next, &tempSearch, compareUserByEmail);
    if (pRes)
        return (User*)pRes->key;
    else
        return NULL;
}

int validateMatchingPassword(char* password, User* u)
{
    if (!strcmp(password, u->password))
        return 1;
    return 0;
}

void printUserDatabase(const UserDatabase* db)
{
    printf("There are %d users in the database.\n", db->userCount);
    L_print(&db->userList, printUser);
}

int saveUserDatabaseToBinaryFile(char* fileName, UserDatabase* db)
{
    FILE* fp = fopen(fileName, "wb");
    if (!fp)
    {
        return 0;
    }
    if (fwrite(&db->userCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    NODE* pNode = &db->userList.head;
    while (pNode->next)
    {
        if (!saveUserToBinaryFile(fp, pNode->next->key))
        {
            fclose(fp);
            return 0;
        }
        pNode = pNode->next;
    }
    fclose(fp);
    return 1;
}

int loadUserDatabaseFromBinaryFile(char* fileName, UserDatabase* db)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        return 0;
    }
    if (fread(&db->userCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    if (!L_init(&db->userList))
    {
        fclose(fp);
        return 0;
    }
    NODE* pNode = &db->userList.head;
    for (int i = 0; i < db->userCount; i++)
    {
        User* temp = (User*)malloc(sizeof(User));
        if (!loadUserFromBinaryFile(fp, temp))
        {
            freeUserDatabase(db);
            fclose(fp);
            return 0;
        }
        pNode = L_insert(pNode, temp);
    }
    fclose(fp);
    return 1;
}

void freeUserDatabase(UserDatabase* db)
{
    L_free(&db->userList, (void*)freeUser);
}