#include "apartments.h"

//This function goes over the commands list and returns command numbered with input cmdNum
char* getCmdWithNum(int cmdNum, cmdList lst, char* short_term_arr[])
{
	int numToInd = cmdNum - 1, cmdLstSize;
	cmdNode *p;

    if (isShortTermFull(short_term_arr)){
		if (lst.tail)
			cmdLstSize = lst.tail->commandId;
		else
			cmdLstSize = 1;
		if (cmdNum > cmdLstSize)	//the command is in short term arr and not in the list
			return short_term_arr[numToInd - cmdLstSize];
		else
		{
			p = lst.head;
			while (p != NULL && p->commandId != cmdNum)
				p = p->next;
			return p->command;
		}
	}

	else
		return short_term_arr[numToInd];
}

//Returns last command entered to short term arr
char* getLastCmdShortTerm(char* short_term_arr[])
{
	int i = 0;
	while (short_term_arr[i] != NULL)
		i++;
	if (i == 0)
		return NULL;
	return short_term_arr[i - 1];
}

void printCmdList(cmdList lst)
{
	cmdNode *p;

	for (p = lst.head; p != NULL; p = p->next)
		printf("%d. %s\n",p->commandId, p->command);
}

void printAllHistory(cmdList lst)
{
	printCmdList(lst);
	printShortTerm(lst);
}

int isShortTermFull(char* short_term_arr[])
{
	return (short_term_arr[N - 1] != NULL);

}

//Function to insert a new command to short term arr
//if short term arr is full, it acts as a queue, moving the first element to the list and shifting all other elements one index back
//then inserting new command
void insertShortTerm(char* command, cmdList* cmdLst)
{
	int i = 0;

	if (isShortTermFull(short_term_history))
	{
		insertDataToEndList(cmdLst, short_term_history[0]);
		for (int j = 1; j < N; j++) //dequeue and enqueue new command
		{
			short_term_history[j - 1] = NULL;
			short_term_history[j - 1] = (char*)realloc(short_term_history[j - 1], sizeof(char) * (strlen(short_term_history[j]) + 1));
			strcpy(short_term_history[j - 1], short_term_history[j]);
		}
		short_term_history[N - 1] = NULL;
		short_term_history[N - 1] = (char*)realloc(short_term_history[N - 1], sizeof(char) * (strlen(command) + 1));
		strcpy(short_term_history[N - 1], command);


	}

	else //short term arr is not full
	{
		while (short_term_history[i] != NULL)
			i++;
		short_term_history[i] = (char*)malloc(sizeof(char) * strlen(command)); //released in main
		strcpy(short_term_history[i], command);
	}
}

void printShortTerm(cmdList lst)
{
	if (isEmptyCmdList(lst))
	{
		for (int i = 0; i < N; i++)
            if (short_term_history[i])
                printf("%d. %s\n", i + 1, short_term_history[i]);
	}
	else
	{
		for (int i = 0, j=lst.tail->commandId+1; i < N; i++, j++)
                printf("%d. %s\n", j, short_term_history[i]);
	}
}

void freeShortTerm(char** shortTermHistory)
{
	int i=0;
	while (short_term_history[i]) {	//find actual size of short term arr
		free(shortTermHistory[i]);
		i++;
	}
}