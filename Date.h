#pragma once
#include <time.h>
#include <stdio.h>

typedef struct
{
    int day;
    int month;
    int year;
}Date;
int isValidDate(Date* d);
void initDate(Date* pD);
void printDate(const Date* pD);
int compareDates(const Date* d1, const Date* d2);
int saveDateToBinaryFile(FILE* fp, Date* dateP);
int loadDateFromBinaryFile(FILE* fp, Date* dateP);