#include "Date.h"
#define MAX_INPUT_SIZE 12
#define Substraction(num1,num2) (num1-num2)


int isValidDate(Date* d)
{
    time_t now = time(NULL);
    struct tm current;
    current = *localtime(&now);
    if (d->day < 0)
        return 0;
    switch (d->month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        if (d->day > 31)
            return 0;
        break;
    case 4: case 6: case 9: case 11:
        if (d->day > 30)
            return 0;
        break;
    case 2:
        if (d->day > 28)
            return 0;
        break;
    default:
        return 0;

    }
    if (d->year < (current.tm_year + 1900))
        return 0;
    else if ((d->year == (current.tm_year + 1900)) && (d->month < (current.tm_mon + 1)))
        return 0;
    else if ((d->year == (current.tm_year + 1900)) && (d->month == (current.tm_mon + 1)) && (d->day < current.tm_mday))
        return 0;

    return 1;
}

void initDate(Date* pD)
{
    time_t now = time(NULL);
    struct tm* current;
    current = localtime(&now);
    int valid;
    do {
        valid = 1;
        char date[MAX_INPUT_SIZE + 1] = { 0 };
        printf("Enter date in dd/mm/yyyy format:\n");
        scanf("%s", date);
        sscanf(date, "%d/%d/%d", &pD->day, &pD->month, &pD->year);
        if (!isValidDate(pD))
        {
            printf("Invalid date, date must be in dd/mm/yyyy format and be after today's date.\n");
            valid = 0;
        }
    } while (valid == 0);
}

void printDate(const Date* pD)
{
    printf("%d/%d/%d ", pD->day, pD->month, pD->year);
}

int compareDates(const Date* d1, const Date* d2)
{
    if (d1->year != d2->year)
        return (d1->year - d2->year);
    else if (d1->month != d2->month)
        return (d1->month - d2->month);
    else if (d1->day != d2->day)
        return (d1->day - d2->day);
    return 0;
}

int saveDateToBinaryFile(FILE* fp, Date* dateP)
{
    if (fwrite(&dateP->day, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(&dateP->month, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(&dateP->year, sizeof(int), 1, fp) != 1)
        return 0;
    return 1;
}

int loadDateFromBinaryFile(FILE* fp, Date* dateP)
{
    if (fread(&dateP->day, sizeof(int), 1, fp) != 1)
        return 0;
    if (fread(&dateP->month, sizeof(int), 1, fp) != 1)
        return 0;
    if (fread(&dateP->year, sizeof(int), 1, fp) != 1)
        return 0;
    return 1;
}