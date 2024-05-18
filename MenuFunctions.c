#include "MenuFunctions.h"
#include "def.h"
#define ADMIN_PASSWORD "admin"

void mainMenu(System* s)
{
    srand(time(NULL));
    int choice = 1;
    printf("-------------Assignment Management System (A.M.S)-------------\n");
    while (choice != 0)
    {
        printf("1)\tLoad system from file.\n2)\tRun a new system.\n0)\tFree memory and exit.\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (!loadSystemFromBinaryFile(SYSTEM_FILE_NAME, s))
            {
                initSystem(s);
                printf("Failed to read system from \"%s\", type 1 to return or 2 continue to the system with no data.\n", SYSTEM_FILE_NAME);
                do {
                    scanf("%d", &choice);
                } while (!validateInput(choice, 1, 2));
                if (choice == 1)
                {
                    return;
                }
            }
            else
                printf("Successfully loaded system from file.\n");
            runSystem(s);
            break;
        case 2:
            initSystem(s);
            runCourseDB(s);
            runSystem(s);
            break;
        case 0:
            freeSystem(s);
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void runSystem(System* s)
{
    User* user;
    int choice = 10;

    printf("Welcome! Please log in or sign up if this is your first time.\n");
    while (choice != 0)
    {
        printf("1)\tLog into existing account.\n2)\tSign up a new account.\n3)\tAdmin menu\n4)\tSave changes.\n0)\tExit.\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            user = login(&s->userDB);
            if (!user)
            {
                break;
            }
            if (user->type == 0)
            {
                ProfessorAssignmentManager* pAMP = assignAMToUser(s, user);
                professorMenu(s, pAMP);
            }
            else
            {
                StudentAssignmentManager* sAMP = assignAMToUser(s, user);
                studentMenu(s, sAMP);
            }
            break;
        case 2:
            user = signUp(&s->userDB);
            if (user->type == 0)
            {
                ProfessorAssignmentManager* pAMP = assignAMToUser(s, user);
            }
            else
            {
                StudentAssignmentManager* sAMP = assignAMToUser(s, user);
            }
            break;
        case 3:
            adminMenu(s);
            break;
        case 4:
;            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
{
                printf("Changes saved successfully.\n");
}
        case 0:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}
// baked in course DB for running the program smoothly (only an admin can create a new course)
void runCourseDB(System* s)
{
    Course* cArr = (Course*)malloc(sizeof(Course) * 5);
    char* tempName = "Calculus 1";
    char tempCode[CODE_LEN + 1] = "CALC1";
    cArr[0].assignedStudentCount = 0;
    cArr[0].assignedStudents = NULL;
    strcpy(cArr[0].courseCode, tempCode);
    cArr[0].courseName = _strdup(tempName);

    tempName = "Calculus 2";
    strcpy(tempCode, "CALC2");
    cArr[1].assignedStudentCount = 0;
    cArr[1].assignedStudents = NULL;
    strcpy(cArr[1].courseCode, tempCode);
    cArr[1].courseName = _strdup(tempName);

    tempName = "Physics";
    strcpy(tempCode, "PHYS1");
    cArr[2].assignedStudentCount = 0;
    cArr[2].assignedStudents = NULL;
    strcpy(cArr[2].courseCode, tempCode);
    cArr[2].courseName = _strdup(tempName);

    tempName = "Chemistry";
    strcpy(tempCode, "CHEM1");
    cArr[3].assignedStudentCount = 0;
    cArr[3].assignedStudents = NULL;
    strcpy(cArr[3].courseCode, tempCode);
    cArr[3].courseName = _strdup(tempName);

    tempName = "Linear Algebra";
    strcpy(tempCode, "ALGE1");
    cArr[4].assignedStudentCount = 0;
    cArr[4].assignedStudents = NULL;
    strcpy(cArr[4].courseCode, tempCode);
    cArr[4].courseName = _strdup(tempName);

    for (int i = 0; i < 5; i++)
    {
        addCourse(&s->courseDB, &cArr[i]);
    }
}

int validateInput(int input, int minIndex, int maxIndex)
{
    if (input < minIndex || input > maxIndex)
    {
        printf("Invalid input, please try again.\n");
        return 0;
    }
    return 1;
}

void professorMenu(System* s, ProfessorAssignmentManager* pAMP)
{
    printf("--------- Professor Assignment Manager for %s ---------\n\n\n", pAMP->prof->userInfo->name);
    int choice = 0;
    int signedIn = 1;
    printf("Please choose one of the following:\n\n");
    while (signedIn)
    {
        printf("1)\tAssignments.\n2)\tCourses.\n3)\tSave changes.\n4)\tSign out.\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            professorAssignmentManagerMenu(s, pAMP);
            break;
        case 2:
            professorCourseMenu(s, pAMP);
            break;
        case 3:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 4:
            printf("Are you sure you want to sign out?\n1)\tYes\n2)\tNo\n");
            do {
                scanf("%d", &choice);
                if (choice < 1 || choice > 2)
                {
                    printf("Invalid input, please try again.\n");
                }
            } while (choice < 1 || choice > 2);
            if (choice == 1)
            {
                signedIn = 0;
                break;
            }
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void studentMenu(System* s, StudentAssignmentManager* sAMP)
{
    printf("--------- Student Assignment Manager for %s ---------\n\n\n", sAMP->stu->userInfo->name);
    int choice = 0;
    int signedIn = 1;
    printf("Please choose one of the following:\n\n");
    while (signedIn)
    {
        printf("1)\tAssignments.\n2)\tCourses.\n3)\tSave changes.\n4)\tSign out.\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            studentAssignmentManagerMenu(s, sAMP);
            break;
        case 2:
            studentCourseMenu(s, sAMP);
            break;
        case 3:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 4:
            printf("Are you sure you want to sign out?\n1)\tYes\n2)\tNo\n");
            do {
                scanf("%d", &choice);
                if (choice < 1 || choice > 2)
                {
                    printf("Invalid input, please try again.\n");
                }
            } while (choice < 1 || choice > 2);
            if (choice == 1)
            {
                signedIn = 0;
                break;
            }
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void studentAssignmentManagerMenu(System* s, StudentAssignmentManager* sAMP)
{
    printf("---------- Assignment Menu----------\n\n\n");
    int choice = 0;
    char* temp;
    Assignment tempSearch;
    Assignment* res;
    printf("Please choose one from the following:\n\n");
    while (choice != 7)
    {
        printf("1)\tView all assignments.\n2)\tView complete assignments.\n3)\tView incomplete assignments.\n4)\tSearch assignment.\n5)\tSort assignments.\n6)\tSave changes.\n7)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printAssignmentManager(&sAMP->sAssignmentManager);
            int aChoice = 0;
            if (sAMP->sAssignmentManager.assignmentCount > 0)
            {
                printf("Choose from the assignments to view possible actions or type 0 to return.\n");
                do {
                    scanf("%d", &aChoice);
                    if (aChoice < 0 || aChoice >(sAMP->sAssignmentManager.assignmentCount))
                        printf("Invalid input, please try again.\n");
                } while (aChoice < 0 || aChoice >(sAMP->sAssignmentManager.assignmentCount));
                if (aChoice == 0)
                    break;
                studentAssignmentMenu(s, sAMP, &sAMP->sAssignmentManager.allAssignments[aChoice - 1]);
            }
            else
            {
                printf("There are currently no assignments.\n");
            }
            break;
        case 2:
            showTurnedIn(sAMP);
            break;
        case 3:
            showUnturnedIn(sAMP);
            break;
        case 4:
            temp = getStrExactLength("Enter assignment name.");
            tempSearch.assignmentName = temp;
            res = findAssignment(&sAMP->sAssignmentManager, &tempSearch, compareAssignmentsByName);
            if (!res)
            {
                printf("Assignment \"%s\" not found, try a different name.\n", temp);
                break;
            }
            studentAssignmentMenu(s, sAMP, res);
            break;
        case 5:
            sortAssignmentMenu(s, &sAMP->sAssignmentManager);
            break;
        case 6:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 7:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;

        }
    }
}

void studentAssignmentMenu(System* s, StudentAssignmentManager* sAMP, Assignment* aP)
{
    printf("---------- Assignment menu for %s ----------\n", aP->assignmentName);
    int choice = 0;
    printf("Please choose one from the following:\n\n");
    while (choice < 1 || choice > 3) {
        printf("1)\tTurn in assignment.\n2)\tSave changes.\n3)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            turnIn(sAMP, aP);
            syncTurnInWithProfessor(s, aP);
            break;
        case 2:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 3:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }


    }
}

void studentCourseMenu(System* s, StudentAssignmentManager* sAMP)
{
    printf("---------- Course Menu----------\n\n\n");
    int choice = 0;

    printf("Please choose one from the following:\n\n");
    while (choice != 5) {
        printf("1)\tView all courses in database.\n2)\tView courses you are signed up to.\n3)\tSign up to courses.\n4)\tSave changes.\n5)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printAllCourses(&s->courseDB);
            break;
        case 2:
            printCourseArray((const Course**)sAMP->allCourses, sAMP->signedCoursesCount);
            break;
        case 3:
            signUpToCourse(sAMP, &s->courseDB);
            break;
        case 4:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else {
                printf("Changes saved successfully.\n");
            }
            break;
        case 5:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void professorAssignmentManagerMenu(System* s, ProfessorAssignmentManager* pAMP)
{
    printf("---------- Assignment Menu----------\n\n\n");
    int choice = 0;
    char* temp;
    Assignment tempSearch;
    Assignment* res;
    printf("Please choose one from the following:\n\n");
    while (choice != 6)
    {
        printf("1)\tView all assignments.\n2)\tAdd new assignment.\n3)\tSearch assignment.\n4)\tSort assignments.\n5)\tSave changes.\n6)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printAssignmentManager(&pAMP->pAssignmentManager);
            int aChoice = 0;
            if (pAMP->pAssignmentManager.assignmentCount > 0) {
                printf("Choose from the assignments to view possible actions or type 0 to return.\n");
                do {
                    scanf("%d", &aChoice);
                    if (aChoice < 0 || aChoice >(pAMP->pAssignmentManager.assignmentCount))
                        printf("Invalid input, please try again.\n");
                } while (aChoice < 0 || aChoice >(pAMP->pAssignmentManager.assignmentCount));
                if (aChoice == 0)
                    break;
                professorAssignmentMenu(s, pAMP, &pAMP->pAssignmentManager.allAssignments[aChoice - 1]);
            }
            else
            {
                printf("There are currently no assignments.\n");
            }
            break;
        case 2:
            res = addNewAssignment(pAMP, &s->courseDB);
            addAssignmentForAllStudentsInCourse(s, res);
            break;
        case 3:
            temp = getStrExactLength("Enter assignment name.");
            tempSearch.assignmentName = temp;
            res = findAssignment(&pAMP->pAssignmentManager, &tempSearch, compareAssignmentsByName);
            if (!res)
            {
                printf("Assignment \"%s\" not found, try a different name.\n", temp);
                break;
            }
            break;
        case 4:
            sortAssignmentMenu(s, &pAMP->pAssignmentManager);
            break;
        case 5:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 6:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;

        }
    }
}

void professorAssignmentMenu(System* s, ProfessorAssignmentManager* pAMP, Assignment* aP)
{
    printf("---------- Assignment menu for %s ----------\n", aP->assignmentName);
    int choice = 0;
    printf("Please choose one from the following:\n\n");
    while (choice != 3) {
        printf("1)\tGrade assignment.\n2)\tSave changes.\n3)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            gradeAssignment(aP);
            syncGradeWithStudent(s, aP);
            break;
        case 2:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else
            {
                printf("Changes saved successfully.\n");
            }
            break;
        case 3:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }


    }
}

void professorCourseMenu(System* s, ProfessorAssignmentManager* pAMP)
{
    printf("---------- Course Menu----------\n\n\n");
    int choice = 0;


    printf("Please choose one from the following:\n\n");
    while (choice != 5) {
        printf("1)\tView all courses in database.\n2)\tView courses you teach.\n3)\tAdd authored courses.\n4)\tSave changes.\n5)\tReturn.\n");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printAllCourses(&s->courseDB);
            break;
        case 2:
            printCourseArray((const Course**)pAMP->authoredCourses, pAMP->authoredCourseCount);
            break;
        case 3:
            addAuthoredCourses(pAMP, &s->courseDB);
            break;
        case 4:
            if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                printf("Error saving changes to file.\n");
            else {
                printf("Changes saved successfully.\n");
            }
            break;
        case 5:
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void sortAssignmentMenu(System* s, AssignmentManager* aMP)
{
    printf("Sort by:\n1)\tName\n2)\tDate\n3)\tCourse code\n");
    int choice = 0;
    while (choice < 1 || choice > 3)
    {
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            sortAssignments(aMP, compareAssignmentsByName);
            break;
        case 2:
            sortAssignments(aMP, compareAssignmentsByDateTime);
            break;
        case 3:
            sortAssignments(aMP, compareAssignmentsByCourse);
            break;
        default:
            printf("Invalid input, please try again.\n");
            break;
        }
    }
}

void adminMenu(System* s)
{
    char* pass;
    int attemptCount = 0;
    int choice = 0;
    Course* coP;
    User* u;
    for(attemptCount = 0 ; attemptCount < MAX_PASS_ATTEMPTS ; attemptCount++)
    {
        pass = getStrExactLength("Please enter admin password.");
        if (strcmp(pass, ADMIN_PASSWORD) == 0)
            break;
        else
        {
            printf("Incorrect password, please try again.\n");
        }
    }
    if (attemptCount >= MAX_PASS_ATTEMPTS)
    {
        printf("Maximum attempts reached, please try again later.\n");
        return;
    }
    else
    {
        printf("---------- Admin Menu ----------\n\n\n");
        while (choice != 3)
        {
            printf("Please choose one of the following:\n1)\tAdd new course to course database.\n2)\tSave Changes.\n3)\tReturn.\n");
            scanf("%d", &choice);
            switch (choice)
            {
            case 1:
                coP = (Course*)malloc(sizeof(Course));
                if (!coP)
                {
                    printf("Memory error, please try again.\n");
                    break;
                }
                initCourse(coP);
                if (!addCourse(&s->courseDB, coP))
                {
                    free(coP);
                    printf("Course with the same code already exists, please try again.\n");
                }
                else
                    printf("Course \"%s\" has been added to the database successfully.\n", coP->courseName);
                break;
            case 2:
                if (!saveSystemToBinaryFile(SYSTEM_FILE_NAME, s))
                    printf("Error saving changes to file.\n");
                else {
                    printf("Changes saved successfully.\n");
                }
                break;
            case 3:
                break;
            default:
                printf("Invalid input, please try again.\n");
                break;
            }
        } 
    }

}
