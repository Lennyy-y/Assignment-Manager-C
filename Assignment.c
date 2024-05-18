#include "Assignment.h"

void initAssignment(Assignment* aP, Course* coP) {
    char temp[MAX_DATE_LEN + 1] = { 0 };
    int choice;
    int valid;
    aP->isComplete = False;
    aP->assignmentGrade = -1.0;
    aP->assignmentCourse = coP;
    aP->assignmentName = getStrExactLength("Enter a name for the assignment:");
    printf("Enter due date for the assignment in dd/mm/yyyy format:\n");
    do {
        valid = 1;
        scanf("%s", temp);
        sscanf(temp, "%d/%d/%d", &aP->dueDate.day, &aP->dueDate.month, &aP->dueDate.year);
        if (!isValidDate(&aP->dueDate)) {
            valid = 0;
            printf("Invalid date, date must be in dd/mm/yyyy format and be after today's date.\n");
        }
    } while (valid == 0);
    printf("Can the assignment be turned in late?\n1) Yes.\n2) No.\n");
    do {
        choice = 0;
        scanf("%d", &choice);
        if (choice == 1) {
            aP->canTurnInLate = True;
            break;
        }
        else if (choice == 2) {
            aP->canTurnInLate = False;
            break;
        }
        else
            printf("Invalid selection, please try again.\n");
    } while (choice != 1 && choice != 2);
}

int saveAssignmentToBinaryFile(FILE* fp, Assignment* aP)
{
    BYTE data[2];
    data[0] ^= data[0];
    data[1] ^= data[1];
    int gradeLeftOfDecimalPoint = (int)aP->assignmentGrade;
    int gradeRightOfDecimalPoint = (int)((aP->assignmentGrade - (int)aP->assignmentGrade) * 10);
    data[0] = aP->isComplete << 7 | aP->canTurnInLate << 6;
    if (aP->isComplete)
    {
        if (aP->assignmentGrade >= 0)
        {
            data[0] = data[0] | gradeRightOfDecimalPoint << 2;
            data[1] = gradeLeftOfDecimalPoint;
        }
    }
    int nameLen = (int)strlen(aP->assignmentName) + 1;
    int courseLen = CODE_LEN + 1; // accommodating  for \0 at the end
    if (fwrite(&nameLen, sizeof(int), 1, fp) != 1)
        return 0;
    if (fwrite(aP->assignmentName, sizeof(char), nameLen, fp) != nameLen)
        return 0;
    if (fwrite(aP->assignmentCourse->courseCode, sizeof(char), courseLen, fp) != courseLen)
        return 0;
    if (!saveDateToBinaryFile(fp, &aP->dueDate))
        return 0;
    if (fwrite(data, sizeof(BYTE), 2, fp) != 2)
        return 0;
    if (aP->isComplete)
    {
        if (!saveDateToBinaryFile(fp, &aP->turnInDate))
            return 0;
    }
    return 1;
}

int loadAssignmentFromBinaryFile(FILE* fp, CourseDatabase* cDBP, Assignment* aP)
{
    BYTE data[2];
    int nameLen;
    char tempCode[CODE_LEN + 1];

    if (fread(&nameLen, sizeof(int), 1, fp) != 1)
        return 0;

    aP->assignmentName = (char*)malloc(sizeof(char) * nameLen);
    if (!aP->assignmentName)
        return 0;
    if (fread(aP->assignmentName, sizeof(char), nameLen, fp) != nameLen)
    {
        free(aP->assignmentName);
        return 0;
    }

    if (fread(tempCode, sizeof(char), CODE_LEN + 1, fp) != CODE_LEN + 1)
    {
        free(aP->assignmentName);
        return 0;
    }
    Course* assignmentCourse = findCourse(cDBP, tempCode);
    aP->assignmentCourse = assignmentCourse;
    if (!loadDateFromBinaryFile(fp, &aP->dueDate))
    {
        free(aP->assignmentName);
        return 0;
    }
    if (fread(data, sizeof(BYTE), 2, fp) != 2)
    {
        free(aP->assignmentName);
        return 0;
    }
    aP->isComplete = (data[0] >> 7) & 1;
    aP->canTurnInLate = (data[0] >> 6) & 1;

    if (aP->isComplete == 1)
    {
        int rightOfDecimal = (data[0] & 0x3C) >> 2;
        int leftOfDecimal = data[1];
        double grade = leftOfDecimal + (double)rightOfDecimal / 10;
        aP->assignmentGrade = grade;
        if (!loadDateFromBinaryFile(fp, &aP->turnInDate))
        {
            free(aP->assignmentName);
            return 0;
        }
    }
    else
        aP->assignmentGrade = -2;
    return 1;
}

void printAssignment(const Assignment* aP)
{
    char status[11];
    if (aP->isComplete == True)
        strcpy(status, "Complete");
    else
        strcpy(status, "Incomplete");

    printf("Assignment name: %s.\nCourse: %s.\nStatus: %s.\nDue date: %d/%d/%d\n", aP->assignmentName, aP->assignmentCourse->courseName, status, aP->dueDate.day, aP->dueDate.month, aP->dueDate.year);
    if (aP->isComplete == 1)
    {
        printf("Turn in date: %d/%d/%d\n", aP->turnInDate.day, aP->turnInDate.month, aP->turnInDate.year);
    }
    if (aP->assignmentGrade < 0.0)
        printf("Assignment not graded yet.\n");
    else
        printf("Grade: %.1lf\n", aP->assignmentGrade);


    if (aP->canTurnInLate)
    {
        printf("Assignment can be turned in late. (%d points will be deducted from the grade)\n", LATE_PENALTY);
    }
    else
        printf("Assignment cannot be turned in late.\n");
}

void freeAssignment(Assignment* aP)
{
    free(aP->assignmentCourse);
    free(aP->assignmentName);
}
