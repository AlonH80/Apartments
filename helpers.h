#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define N 7
#define TRUE 1
#define FALSE 0
#define LEGAL_COMMAND 1
#define ILLEGAL_COMMAND 0
typedef int BOOL;

typedef struct timeit{
    short int year;
    short int month;
    short int day;
}Time;

typedef struct appartment {
	short int aptCode;
	char* aptAddress;
	int aptPrice;
	short int numRooms;
	short int entDay;
	short int entMonth;
	short int entYear;
	Time savedDate;
}apt;

typedef struct apartNode {
	apt data;
	struct apartNode* next;
	struct apartNode* prev;
}aptNode;

typedef struct apartList {
	aptNode* head;
	aptNode* tail;
}aptList;

typedef struct commandNode {
	char* command;
	int commandId;
	struct commandNode* next;
	struct commandNode* prev;
}cmdNode;

typedef struct commandList {
	cmdNode* head;
	cmdNode* tail;
}cmdList;

char *short_term_history[N];
cmdList cmdLst;
aptList aptLst;

// structs_functions
void insertAptDataToEndList(aptList* aptLst, apt aptData);
aptNode* createAptNode(apt aptData, aptNode* next,aptNode* prev);
apt createAptFromData(char* aptAddress, int aptPrice, short int numRooms, short int entDay, short int entMonth, short int entYear, aptList apt_list);
aptList makeEmptyAptList();
cmdList makeEmptyCmdList();
int isEmptyCmdList(cmdList lst);
int isEmptyAptList(aptList aptLst);
cmdNode* createCmdNode(char* command, cmdNode* next, cmdNode* prev, cmdList lst);
void insertDataToEndList(cmdList* lst, char* command);
aptList makeEmptyAptList();
void freeApartments(aptList lst);
void freeCmdLst(cmdList lst);

// strings_functions
char * replaceSubStr(char* original,char* pattern,char* replacement);
void prinRev(cmdList lst);
int convChToInt(char* str);
int readCommand(char* command,aptList* apts,cmdList hstList);
char* getSubString(char* str,int start,int end);
int seekChar (char* str,int startSeek,char ch);
int readGetAptParameters(char** parameters,int* minimumRooms,int* maximumRooms,int* minimumPrice,int* maximumPrice,char** date,char** sort);
char** splitString(char* str,char ch,int* numOfWords);
void readAddAptParameters(char* str,char** address,int* price,int* numRooms,short int* day,short int* month,short int* year);
char* getString();
void freeArOfStrings(char** arOfStrings, int size);

// time_functions
int isFirstDateLater(char* firstDate, short int secDateDay, short int secDateMon, short int secDateYear);
int getMonth(char* monStr);
void convToDate(char* str,Time* t);
Time substractTime(Time currTime,int days);
BOOL isLeapYear(int year);
int daysInMonth(int month,int year);
int compareTime(Time t1,Time t2);
Time getTime(time_t timeSecs);
void copyTime(Time* to,Time from);

// other_func
int getNumOfAparts(aptList* apts);
void freeAppartment(aptNode* apart);
int findMaxCode(aptList aptlst);
void swap(aptNode *a, aptNode *b);

// files functions
unsigned char* convertNumRoomsDate(short int numRooms, short int day, short int month, short int year);
unsigned short int convSavedDate(Time savedDate);
void saveAptInFile(FILE* binFile,apt apart);
void saveAppartments(aptList appartments);
void unpackRoomsEntDateData(unsigned char* compData, apt* apart);
void unpackSavedDate(unsigned short int compDate,apt* apart);
int loadApt(FILE* binFile,apt* currApt);
aptList loadAppartments();
void writeShortTermToFile(char* fileName, char** short_term_historyArr);
void writeCmdLstToFile(char* fileName, cmdList lst);
void writeHistoryToFile(char* fileName, char** short_term_historyArr, cmdList lst);
void loadHistoryFromFile(char* fileName, char** short_term_historyArr, cmdList* lst);
void insertDataToStartList(cmdList* lst, char* command);
int getNumOfLines(char* fileName);
