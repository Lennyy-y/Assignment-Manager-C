#pragma once
#define USER_DB_FILE_NAME "user_db.bin"
#define COURSE_DB_FILE_NAME "course_db.bin"
#define STUDENT_FILE_NAME "students.bin"
#define PROFESSOR_FILE_NAME "professors.bin"
#define SAM_FILE_NAME "sAM.bin"
#define PAM_FILE_NAME "pAM.bin"
#define SYSTEM_FILE_NAME "system.bin"
#include "StudentAssignmentManager.h"
#include "ProfessorAssignmentManager.h"
#include "CourseDatabase.h"

typedef struct {
    UserDatabase userDB;
    CourseDatabase courseDB;
    int studentManagerCount;
    int professorManagerCount;
    StudentAssignmentManager* allStudentManagers;
    ProfessorAssignmentManager* allProfessorManagers;
} System;

void initSystem(System* s);
void* assignAMToUser(System* s, User* u);
int saveSystemToBinaryFile(char* fileName, System* s);
int loadSystemFromBinaryFile(char* fileName, System* s);
void addAssignmentForAllStudentsInCourse(System* s, Assignment* aP);
void syncTurnInWithProfessor(System* s, Assignment* aP);
void syncGradeWithStudent(System* s, Assignment* aP);
void freeSystem(System* s);