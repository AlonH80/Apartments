#include "apartments.h"

aptNode* createAptNode(apt aptData, aptNode* next,aptNode* prev)
{
	// This function create new aptNode
	aptNode *result;

	result = (aptNode *)malloc(sizeof(aptNode));

	result->data = aptData;
	result->next = next;
    result->prev=prev;
	return result;
}

void insertAptDataToEndList(aptList* aptLst, apt aptData)
{
	// This function add aptData to aptNode and then to the end of the list.
	aptNode* newTail = createAptNode(aptData, NULL,aptLst->tail);

	if (isEmptyAptList(*aptLst))
		aptLst->head = aptLst->tail = newTail;

	else
	{
		aptLst->tail->next = newTail;
		aptLst->tail = newTail;
	}
}

int isEmptyAptList(aptList aptLst)
{
	return (aptLst.head == NULL);
}

//Function that gets apartment data and creates an apartment from it
apt createAptFromData(char* aptAddress, int aptPrice, short int numRooms, short int entDay, short int entMonth, short int entYear, aptList apt_list)
{
	apt returnedApt;
    returnedApt.aptCode=findMaxCode(apt_list)+1;
	returnedApt.aptAddress = aptAddress;
	returnedApt.aptPrice = aptPrice;
	returnedApt.numRooms = numRooms;
	returnedApt.entDay = entDay;
	returnedApt.entMonth = entMonth;
	returnedApt.entYear = entYear;

	return returnedApt;
}

cmdList makeEmptyCmdList()
{
	cmdList result;

	result.head = result.tail = NULL;
	return result;
}

int isEmptyCmdList(cmdList lst)
{
	return (lst.head == NULL);
}

cmdNode* createCmdNode(char* command, cmdNode* next, cmdNode* prev, cmdList lst)
{
	cmdNode *result;

	result = (cmdNode *)malloc(sizeof(cmdNode));
	result->command = (char*)malloc(sizeof(char) * (strlen(command)+1));
	strcpy(result->command, command);
	
	if (isEmptyCmdList(lst))
		result->commandId = 1;
	else
		result->commandId = lst.tail->commandId + 1;

	result->next = next;
	result->prev = prev;

	return result;
}

void insertDataToEndList(cmdList* lst, char* command)
{
	cmdNode* newTail = createCmdNode(command, NULL, lst->tail, *lst);

	if (isEmptyCmdList(*lst))
	{
		lst->head = lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

aptList makeEmptyAptList()
{
	aptList result;

	result.head = result.tail = NULL;
	return result;
}

void freeCmdLst(cmdList lst)
{
	cmdNode* currNode = lst.head, *tmp;
	while (currNode) {
		free(currNode->command);
		tmp = currNode;
		currNode = currNode->next;
		free(tmp);
	}
}


void freeApartments(aptList lst)
{
	aptNode* currNode = lst.head, *tmp;
	while (currNode) {
		free(currNode->data.aptAddress);
		tmp = currNode;
		currNode = currNode->next;
		free(tmp);
	}
}