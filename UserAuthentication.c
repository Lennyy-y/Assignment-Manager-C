#include "UserAuthentication.h"
#include <string.h>
#include <ctype.h>


int validateEmailFormat(char* email)
{
    int emailLen = (int)strlen(email);
    int atCount = 0;
    for (int i = 0; i < emailLen; i++)
    {
        if (email[i] == '@')
            atCount++;
        if (email[i] == ' ' || email[i] == '/' || email[i] == ':'
            || email[i] == ';' || email[i] == '<' || email[i] == '>'
            || email[i] == ',' || email[i] == '[' || email[i] == ']')
            return 0;
    }
    if (atCount == 1)
    {
        char* domain_suffix = strchr(email, '@');
        char* dot = strchr(domain_suffix, '.');
        if (dot == NULL)
            return 0;
        int suffix = (int)strlen(dot);
        if (dot > (strchr(email, '@') + 1) && suffix > 3)
            return 1;
    }
    return 0;
}

int validatePasswordFormat(char* password)
{
    int passLen = (int)strlen(password);
    if (passLen < 8)
        return 0;
    for (int i = 0; i < passLen; i++)
    {
        if (!isalnum(password[i]))
            return 0;
    }
    return 1;
}
