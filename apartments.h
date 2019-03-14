// apt functions
#include "history.h"
void printApartment(apt apartment);
void getApt(int maxPrice, short int minNumRooms, short int maxNumRooms, char* date, char* sortFlag);
void addApt(aptList* aptLst, apt aptData);
void buyAnApt(aptList* apts, int code);
void deleteASingleApt(aptList* apts, int code);
void deleteAnApt(aptList* apts, int daysRange);
void printApartCode(apt apart);
void callAptFunction(char* command, aptList* apts);
void getAnAptEnter(aptList* apts, int days);
void sortAptLstAsc(aptList* aptLst);
void sortAptLstDesc(aptList* aptLst);
