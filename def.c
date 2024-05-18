#include "def.h"


char* myStrdup(const char* c)
{
    char* dup = malloc(strlen(c) + 1);

    if (dup != NULL)
        strcpy(dup, c);

    return dup;
}

char* getStrExactLength(const char* msg)
{
    char* theStr = NULL;
    char inpStr[MAX_LENGTH]; //variable to hold the string that the user gives

    //Ask for a string from the user
    puts(msg);
    myGets(inpStr, sizeof(inpStr));
    theStr = _strdup(inpStr);
    return theStr;
}

char* myGets(char* buffer, int size)
{
    char* ok;
    if (buffer != NULL && size > 0)
    {
        do {
            ok = fgets(buffer, size, stdin);
        } while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
        if (ok)
        {
            char* back = buffer + strlen(buffer);
            //trim end spaces
            while (isspace(*--back));
            *(back + 1) = '\0';
            return buffer;
        }
        buffer[0] = '\0';
    }
    return NULL;
}
void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element))
{
    for (int i = 0; i < size; i++)
        func((char*)(arr)+i * typeSize);

}