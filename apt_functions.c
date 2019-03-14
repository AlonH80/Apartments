#include "apartments.h"

//Function to print apartment info
void printApartment(apt apartment)
{
	printf("Apt details: \n");
	printf("Code: %d: \n", apartment.aptCode);
	printf("Address: %s \n", apartment.aptAddress);
	printf("Number of rooms: %d \n", apartment.numRooms);
	printf("Price: %d \n", apartment.aptPrice);
	printf("Entry date: %d.%d.%d \n", apartment.entDay, apartment.entMonth, apartment.entYear);
	printf("Database entry date: %d.%d.%d \n", apartment.savedDate.day, apartment.savedDate.month, apartment.savedDate.year);
}

//Function to gent an apartment (get-an-apt)
void getApt(int maxPrice, short int minNumRooms, short int maxNumRooms, char* date, char* sortFlag)
{
	aptNode* curr;
	apt currAptDetails;
	if (sortFlag != NULL)
	{
		if (strcmp(sortFlag, "s") == 0)
			sortAptLstAsc(&aptLst);
		else if (strcmp(sortFlag, "sr") == 0)
			sortAptLstDesc(&aptLst);
	}

	for (curr = aptLst.head; curr != NULL; curr = curr->next)
	{
		currAptDetails = curr->data;
		//Checks parameters and prints matching apartments
		if (currAptDetails.aptPrice <= maxPrice && currAptDetails.numRooms >= minNumRooms &&currAptDetails.numRooms <= maxNumRooms){
                if ((!date)||isFirstDateLater(date,currAptDetails.entDay, currAptDetails.entMonth, currAptDetails.entYear)){
                        printApartment(currAptDetails);
                }
        }

	}
}

//Function to add an apartment to linked list
void addApt(aptList* aptLst, apt aptData)
{
	insertAptDataToEndList(aptLst, aptData);
}

//Function to bubble sort apartment list in ascending order (-s flag)
void sortAptLstAsc(aptList* aptLst)
{
	int swapped;
	aptNode *head;
	aptNode *curr = NULL;

	if (isEmptyAptList(*aptLst))
		return;

	do
	{
		swapped = 0;
		head = aptLst->head;

		while (head->next != curr)
		{
			if (head->data.aptPrice > head->next->data.aptPrice)
			{
				swap(head, head->next);
				swapped = 1;
			}
			head = head->next;
		}
		curr = head;
	}
	while (swapped);
}

//Function to bubble sort apartment list in ascending order (-sr flag)
void sortAptLstDesc(aptList* aptLst)
{
	int swapped;
	aptNode *head;
	aptNode *curr = NULL;

	if (isEmptyAptList(*aptLst))
		return;

	do
	{
		swapped = 0;
		head = aptLst->head;

		while (head->next != curr)
		{
			if (head->data.aptPrice < head->next->data.aptPrice)
			{
				swap(head, head->next);
				swapped = 1;
			}
			head = head->next;
		}
		curr = head;
	} while (swapped);
}

//Function to buy-an-apt - deletes it from apt list
void buyAnApt(aptList* apts,int code)
{
    deleteASingleApt(apts,code);
}

//Function that deletes an apartment from apartment list
void deleteASingleApt(aptList* apts,int code)
{
    aptNode* apart=apts->head;
    while (apart->data.aptCode!=code){
        apart=apart->next;
    }
    if (apart==apts->head){
        if (apart->next){
            apts->head=apart->next;
            apart->next->prev=NULL;
        }
        else{
            apts->head=NULL;
            apts->tail=NULL;
        }
    }
    else if(apart==apts->tail){
        apts->tail=apart->prev;
        apart->prev->next=NULL;
    }
    else{
        apart->prev->next=apart->next;
        apart->next->prev=apart->prev;
    }
    freeAppartment(apart);
}

//Function to delete all apartments entered to list in last daysRange
//user for delete-an-apt -Enter command
void deleteAnApt(aptList* apts,int daysRange)
{
    aptNode* currApt=apts->head;
    aptNode* nextApt;
    time_t currTime;        // time_t can also be done in ints, but it will raise warning.
    time(&currTime);
    Time curTime=getTime(currTime);
    Time startTime=substractTime(curTime,daysRange);
    while (currApt){
        nextApt=currApt->next;
        if (currApt->data.savedDate.year>startTime.year){
            deleteASingleApt(apts,currApt->data.aptCode);
        }
        else if (currApt->data.savedDate.year==startTime.year){
            if(currApt->data.savedDate.month>startTime.month){
                deleteASingleApt(apts,currApt->data.aptCode);
            }
            else if(currApt->data.savedDate.month==startTime.month){
                if (currApt->data.savedDate.day>=startTime.day)
                    deleteASingleApt(apts,currApt->data.aptCode);
            }
        }

        currApt=nextApt;
    }
}

//Function to handle commands related to actions on apartments
void callAptFunction (char* command,aptList* apts)
{
    int numOfWords,numOfComm;
    char** splitCommand=splitString(command,' ',&numOfWords);
    char** aptComm=splitString(splitCommand[0],'-',&numOfComm);
    if (strcmp(aptComm[0],"get")==0){       // split to enter and regular getAnApt
        if (strcmp(splitCommand[1],"-Enter")==0){
            getAnAptEnter(apts,convChToInt(splitCommand[2]));
        }
        else{
			//default parameters for get-an-apt, used when they are not entered by user
            int minimumRooms=0,maximumRooms=1000,minimumPrice=0,maximumPrice=999999999; char* date=NULL; char* sort=NULL;
            readGetAptParameters(splitCommand+1,&minimumRooms,&maximumRooms,&minimumPrice,&maximumPrice,&date,&sort);
            getApt(maximumPrice,minimumRooms,maximumRooms,date,sort);
        }
    }
    else if (strcmp(aptComm[0],"add")==0){	//deals with add commands
        char* addString=getSubString(command,seekChar(command,0,' ')+1,strlen(command));
        char* address=NULL; int price,numRooms; short int day,month,year;
        readAddAptParameters(addString,&address,&price,&numRooms,&day,&month,&year);
        apt newApt=createAptFromData(address,price,numRooms,day,month,year,aptLst);
        time_t curr;
        time(&curr);
        copyTime(&(newApt.savedDate),getTime(curr));
        addApt(apts,newApt);
		free(addString);
    }
    else if (strcmp(aptComm[0],"buy")==0){	//deals with buy commands
        buyAnApt(apts,convChToInt(splitCommand[1]));
    }
    else if (strcmp(aptComm[0],"delete")==0){	//deals with delete commands
        deleteAnApt(apts,convChToInt(splitCommand[2]));
    }

    else{
        printf("Illegal input!\n");
    }
    
    freeArOfStrings(splitCommand,numOfWords);
    freeArOfStrings(aptComm,numOfComm);
}

//Function that fings apartment entered in the last X days
//for get-an-anpt -Enter command
void getAnAptEnter(aptList* apts,int days)
{
    time_t cur;
    time(&cur);
    Time currTime=getTime(cur);
    Time startTime=substractTime(currTime,days);
    aptNode* currApt=apts->head;
    while (currApt){
        if (compareTime(currApt->data.savedDate,startTime)<=0){
            printApartCode(currApt->data);
        }
        currApt=currApt->next;
    }
}

void printApartCode(apt apart)
{
    printf("Appartment code:%d\n",apart.aptCode);
}
