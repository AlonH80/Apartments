#include "apartments.h"

void main()
{
    int toExit=FALSE;
    loadHistoryFromFile("history.txt",short_term_history,&cmdLst);
    aptLst=loadAppartments();
    printf("Please enter one of the following commands:\n");
    printf("add-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n");
    printf("For reconstruction commands, please enter:\n");
    printf("!!, !num, history, short_history or !num^str1^str2\n");
    printf("To exit, enter exit.\n");
    while(!toExit){
		char* command=getString();
        toExit=readCommand(command,&aptLst,cmdLst);
        printf("\n");
		free(command);
    }
    saveAppartments(aptLst);
    writeHistoryToFile("history.txt",short_term_history,cmdLst);
	freeCmdLst(cmdLst);
	freeShortTerm(short_term_history);
	freeApartments(aptLst);
}
