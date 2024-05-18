#include "System.h"


void initSystem(System* s)
{
    s->studentManagerCount = 0;
    s->professorManagerCount = 0;
    initCourseDatabase(&s->courseDB);
    initUserDatabase(&s->userDB);
}

void* assignAMToUser(System* s, User* u)
{

    if (u->type == 0)
    {
        if (s->professorManagerCount != 0)
        {
            for (int i = 0; i < s->professorManagerCount; i++)
            {
                if (s->allProfessorManagers[i].prof->userInfo == u)
                    return &s->allProfessorManagers[i];
            }
        }
        Professor* prof = (Professor*)malloc(sizeof(Professor));
        if (!prof)
            return NULL;
        initProfessor(prof, u);
        if (s->professorManagerCount == 0)
            s->allProfessorManagers = (ProfessorAssignmentManager*)malloc(sizeof(ProfessorAssignmentManager));
        else
            s->allProfessorManagers = (ProfessorAssignmentManager*)realloc(s->allProfessorManagers, sizeof(ProfessorAssignmentManager) * (s->professorManagerCount + 1));

        if (!s->allProfessorManagers)
        {
            freeProfessor(prof);
            return NULL;
        }
        initProfessorAssignmentManager(&s->allProfessorManagers[s->professorManagerCount], &s->courseDB, prof);
        s->professorManagerCount++;
        return &s->allProfessorManagers[s->professorManagerCount - 1];
    }
    else
    {
        if (s->studentManagerCount != 0)
        {
            for (int i = 0; i < s->studentManagerCount; i++) {
                if (!strcmp(s->allStudentManagers[i].stu->studentEmail, u->email))
                    return &s->allStudentManagers[i];
            }
        }
        Student* stu = (Student*)malloc(sizeof(Student));
        if (!stu)
        {
            return NULL;
        }
        initStudent(stu, u);
        if (s->studentManagerCount == 0)
            s->allStudentManagers = (StudentAssignmentManager*)malloc(sizeof(StudentAssignmentManager));
        else
            s->allStudentManagers = (StudentAssignmentManager*)realloc(s->allStudentManagers, sizeof(StudentAssignmentManager) * (s->studentManagerCount + 1));
        if (!s->allStudentManagers)
        {
            freeStudent(stu);
            return NULL;
        }
        initStudentAssignmentManager(&s->allStudentManagers[s->studentManagerCount], &s->courseDB, stu);
        s->studentManagerCount++;
        return &s->allStudentManagers[s->studentManagerCount - 1];
    }
    return NULL;
}

int saveSystemToBinaryFile(char* fileName, System* s)
{
    FILE* fp = fopen(fileName, "wb");
    if (!fp)
        return 0;

    if (!saveUserDatabaseToBinaryFile(USER_DB_FILE_NAME, &s->userDB))
    {
        fclose(fp);
        return 0;
    }
    if (!saveCourseDatabaseToBinaryFile(COURSE_DB_FILE_NAME, &s->courseDB))
    {
        fclose(fp);
        return 0;
    }
    if (fwrite(&s->studentManagerCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    if (!saveStudentAssignmentManagerArrayToBinaryFile(SAM_FILE_NAME, s->allStudentManagers, s->studentManagerCount))
    {
        fclose(fp);
        return 0;
    }
    if (fwrite(&s->professorManagerCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }
    if (!saveProfessorAssignmentManagerArrayToBinaryFile(PAM_FILE_NAME, s->allProfessorManagers, s->professorManagerCount))
    {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int loadSystemFromBinaryFile(char* fileName, System* s)
{
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
        return 0;
    if (!loadUserDatabaseFromBinaryFile(USER_DB_FILE_NAME, &s->userDB))
    {
        fclose(fp);
        return 0;
    }
    if (!loadCourseDatabaseFromBinaryFile(COURSE_DB_FILE_NAME, &s->courseDB, &s->userDB))
    {
        freeUserDatabase(&s->userDB);
        fclose(fp);
        return 0;
    }
    if (fread(&s->studentManagerCount, sizeof(int), 1, fp) != 1)
    {
        freeUserDatabase(&s->userDB);
        freeCourseDatabase(&s->courseDB);
        fclose(fp);
        return 0;
    }
    if (s->studentManagerCount != 0)
    {
        s->allStudentManagers = (StudentAssignmentManager*)malloc(
            sizeof(StudentAssignmentManager) * s->studentManagerCount);
        if (!loadStudentAssignmentManagerArrayFromBinaryFile(SAM_FILE_NAME, s->allStudentManagers, &s->userDB, &s->courseDB,
            s->studentManagerCount)) {
            freeUserDatabase(&s->userDB);
            freeCourseDatabase(&s->courseDB);
            fclose(fp);
            return 0;
        }
    }
    if (fread(&s->professorManagerCount, sizeof(int), 1, fp) != 1)
    {
        freeUserDatabase(&s->userDB);
        freeCourseDatabase(&s->courseDB);
        for (int i = 0; i < s->studentManagerCount; i++)
            freeStudentAssignmentManager(&s->allStudentManagers[i]);
        fclose(fp);
        return 0;
    }
    if (s->professorManagerCount != 0)
    {
        s->allProfessorManagers = (ProfessorAssignmentManager*)malloc(sizeof(ProfessorAssignmentManager) * s->professorManagerCount);
        if (!loadProfessorAssignmentManagerArrayFromBinaryFile(PAM_FILE_NAME, s->allProfessorManagers, &s->userDB, &s->courseDB,
            s->professorManagerCount)) {
            freeUserDatabase(&s->userDB);
            freeCourseDatabase(&s->courseDB);
            for (int i = 0; i < s->studentManagerCount; i++)
                freeStudentAssignmentManager(&s->allStudentManagers[i]);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

void addAssignmentForAllStudentsInCourse(System* s, Assignment* aP)
{
    Course* temp = aP->assignmentCourse;

    for (int i = 0; i < s->studentManagerCount; i++)
    {
        if (s->allStudentManagers[i].signedCoursesCount != 0)
        {
            Course* res = findCourseInArray(s->allStudentManagers[i].allCourses, s->allStudentManagers[i].signedCoursesCount, temp->courseCode);
            if (res)
            {
                addAssignmentFromProfessor(&s->allStudentManagers[i], aP);
            }
        }
    }
}

void syncTurnInWithProfessor(System* s, Assignment* aP)
{
    time_t now = time(NULL);
    struct tm* current;
    current = localtime(&now);
    Course* tempCourse = aP->assignmentCourse;
    char* tempAssignmentName = aP->assignmentName;
    for (int i = 0; i < s->professorManagerCount; i++)
    {
        if (findCourseInArray(s->allProfessorManagers[i].authoredCourses, s->allProfessorManagers[i].authoredCourseCount, tempCourse->courseCode))
        {
            for (int j = 0; j < s->allProfessorManagers[i].pAssignmentManager.assignmentCount; j++)
            {
                if (!strcmp(s->allProfessorManagers[i].pAssignmentManager.allAssignments[i].assignmentName, tempAssignmentName))
                {
                    s->allProfessorManagers[i].pAssignmentManager.allAssignments[i].isComplete = 1;
                    s->allProfessorManagers[i].pAssignmentManager.allAssignments[i].turnInDate.day = current->tm_mday;
                    s->allProfessorManagers[i].pAssignmentManager.allAssignments[i].turnInDate.month = current->tm_mon + 1;
                    s->allProfessorManagers[i].pAssignmentManager.allAssignments[i].turnInDate.year = current->tm_year + 1900;
                }
            }
        }
    }
}

void syncGradeWithStudent(System* s, Assignment* aP)
{
    Course* tempCourse = aP->assignmentCourse;
    char* tempAssignmentName = aP->assignmentName;
    for (int i = 0; i < s->studentManagerCount; i++)
    {
        if (findCourseInArray(s->allStudentManagers[i].allCourses, s->allStudentManagers[i].signedCoursesCount, tempCourse->courseCode))
        {
            for (int j = 0; j < s->allStudentManagers[i].sAssignmentManager.assignmentCount; j++)
            {
                if (!strcmp(s->allStudentManagers[i].sAssignmentManager.allAssignments[i].assignmentName, tempAssignmentName))
                {
                    s->allStudentManagers[i].sAssignmentManager.allAssignments[i].assignmentGrade = aP->assignmentGrade;
                }
            }
        }
    }
}

void freeSystem(System* s)
{
    for (int i = 0; i < s->studentManagerCount; i++)
    {
        freeStudentAssignmentManager(&s->allStudentManagers[i]);
    }
    for (int i = 0; i < s->professorManagerCount; i++)
    {
        freeProfessorAssignmentManager(&s->allProfessorManagers[i]);
    }
    freeCourseDatabase(&s->courseDB);
    freeUserDatabase(&s->userDB);
}