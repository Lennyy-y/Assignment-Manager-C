#pragma once
#include "System.h"



void mainMenu(System* s);
void runSystem(System* s);
void runCourseDB(System* s);


int validateInput(int input, int minIndex, int maxIndex);
void professorMenu(System* s, ProfessorAssignmentManager* pAMP);
void studentMenu(System* s, StudentAssignmentManager* sAMP);
void studentAssignmentManagerMenu(System* s, StudentAssignmentManager* sAMP);
void studentAssignmentMenu(System* s, StudentAssignmentManager* sAMP, Assignment* aP);
void studentCourseMenu(System* s, StudentAssignmentManager* sAMP);
void professorAssignmentManagerMenu(System* s, ProfessorAssignmentManager* pAMP);
void professorAssignmentMenu(System* s, ProfessorAssignmentManager* pAMP, Assignment* aP);
void professorCourseMenu(System* s, ProfessorAssignmentManager* pAMP);
void sortAssignmentMenu(System* s, AssignmentManager* aMP);
void adminMenu(System* s);
