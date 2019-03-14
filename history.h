#include "helpers.h"
// history
int isShortTermFull(char* short_term_arr[]);
void insertShortTerm(char* command, cmdList* cmdLst);
void printShortTerm(cmdList lst);
void printCmdList(cmdList lst);
void printAllHistory(cmdList lst);
char* getLastCmdShortTerm(char* short_term_arr[]);
char* getCmdWithNum(int cmdNum, cmdList lst, char* short_term_arr[]);
void freeShortTerm(char** shortTermHistory);