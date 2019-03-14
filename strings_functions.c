#include "apartments.h"

int readCommand(char* command,aptList* apts,cmdList hstList)
{
	// This function read the command from user and call that function.
    int numOfWords;
    char** words=splitString(command,' ',&numOfWords);
	int toExit = 0;
    if (command[0]=='!'){		// Redo the command, take command from history.
        if (strcmp(command,"!!")==0){
           char* lastCommand=getLastCmdShortTerm(short_term_history);
           readCommand(lastCommand,apts,hstList);
        }
        else{
            char** cmdNums=splitString(command+1,'^',&numOfWords);
            char* comm=getCmdWithNum(convChToInt(cmdNums[0]),hstList,short_term_history);
            if (numOfWords>1){
                comm=replaceSubStr(comm,cmdNums[1],cmdNums[2]);
            }
            return readCommand(comm,apts,hstList);
        }
    }

    else if (strcmp(words[0],"history")==0){	// Present the entire history.
        printAllHistory(hstList);
    }

    else if (strcmp(words[0],"short_history")==0){	// Present short_history
        printShortTerm(hstList);
    }

    else if (strstr(words[0],"apt")){		// Call apt function
        callAptFunction(command,apts);
        insertShortTerm(command,&cmdLst);
    }
    else if (strcmp(words[0],"exit")==0){	// Exit the program.
        printf("Goodbye!\n");
        toExit=1;
    }
    else{
        printf("Illegal command entered!!\n");
    }
	freeArOfStrings(words, numOfWords);
    return toExit;
}

char* getSubString(char* str,int start,int end)
{
	// This function return the substring between start and end index of str, assuming str[end] is a char or '\0'.
    char* subString=(char*)malloc(end-start+1);
    int i;
    for (i=start;i<end;i++){
        subString[i-start]=str[i];
    }
    subString[end-start]='\0';
    return subString;
}

int seekChar (char* str,int startSeek,char ch)        // Return ch index in str, or -1 if ch not found.
{
    int i=startSeek;
    while (str[i] && str[i]!='\n' && str[i]!=ch){
        i++;
    }
    if (!str[i] || str[i]=='\n')
        i=-1;
    return i;
}

int readGetAptParameters(char** parameters,int* minimumRooms,int* maximumRooms,int* minimumPrice,int* maximumPrice,char** date,char** sort)
{
	// This function receive parameters as input and change their value according to string array parameters.
    int i=0;
    while (parameters[i]){
        if (strcmp(parameters[i],"-MinimumNumRooms")==0){
            (*minimumRooms)=convChToInt(parameters[i+1]);
            i+=2;
        }

        else if (strcmp(parameters[i],"-MaximumNumRooms")==0){
            (*maximumRooms)=convChToInt(parameters[i+1]);
            i+=2;
        }

        else if (strcmp(parameters[i],"-MinimumPrice")==0){
            (*minimumPrice)=convChToInt(parameters[i+1]);
            i+=2;
        }
        else if (strcmp(parameters[i],"-MaximumPrice")==0){
            (*maximumPrice)=convChToInt(parameters[i+1]);
            i+=2;
        }

        else if (strcmp(parameters[i],"-Date")==0){
            (*date)=parameters[i+1];
            i+=2;
        }

        else if(strcmp(parameters[i],"-sr")==0 || strcmp(parameters[i],"-s")==0){
            (*sort)=parameters[i]+1;
            i++;
        }
        else{
            return ILLEGAL_COMMAND;
        }
    }
    return LEGAL_COMMAND;
}

char** splitString(char* str,char ch,int* numOfWords)
{
	// This function split str according to ch encounter, and update numOfWords
    char** words=(char**)malloc(50*sizeof(char*));
    (*numOfWords)=0;
    int currChPos,prevChPos=0;
    currChPos=seekChar(str,0,ch);
    while (currChPos!=-1){
        if (currChPos>prevChPos)
            words[(*numOfWords)++]=getSubString(str,prevChPos,currChPos);
        prevChPos=currChPos+1;
        currChPos=seekChar(str,prevChPos,ch);
    }
    words[(*numOfWords)++]=getSubString(str,prevChPos,strlen(str));
    words=(char**)realloc(words,((*numOfWords)+1)*sizeof(char*));
    words[*numOfWords]=NULL;
    return words;
}

void freeArOfStrings(char** arOfStrings, int size) 
{
	// This function free array of strings.
	int i;
	for (i = 0; i<size; i++) {
		free(arOfStrings[i]);
	}
	free(arOfStrings);
}

char * replaceSubStr(char* original,char* pattern,char* replacement)
{
	// This function pattern in original with replacement
	int replen = strlen(replacement);
	int patlen = strlen(pattern);
	int orilen = strlen(original);
	int patcnt = 0;
	char * oriptr;
	char * patloc;

	// find how many times the pattern occurs in the original string
	for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
		patcnt++;

	// allocate memory for the new string
	int retlen = orilen + patcnt * (replen - patlen);
	char * const returned = (char *)malloc(sizeof(char) * (retlen + 1));

	if (returned != NULL)
	{
		// copy the original string,
		// replacing all the instances of the pattern
		char * retptr = returned;
		for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
		{
			int skplen = patloc - oriptr;
			// copy the section until the occurence of the pattern
			strncpy(retptr, oriptr, skplen);
			retptr += skplen;
			// copy the replacement
			strncpy(retptr, replacement, replen);
			retptr += replen;
		}
		// copy the rest of the string.
		strcpy(retptr, oriptr);
	}
	return returned;
}

void prinRev(cmdList lst)
{
	// This function print lst from tail to head.
	cmdNode* curr;;
	for (curr = lst.tail; curr != NULL; curr = curr->prev)
	{
		printf("%d. %s\n", curr->commandId, curr->command);
	}

}

int convChToInt(char* str)
{
	// This function convert string to int. if there is a char that is not digit or space, the return value will be -1.
    int i=0;
    int returnVal=0;
    int length=strlen(str);
    while (i<length && str[i] && str[i]!='\n'){
        if (str[i]>='0'&&str[i]<='9'){
            returnVal*=10;
            returnVal+=(int)(str[i]-'0');
            i++;
        }
        else{
            return -1;
        }
    }
    return returnVal;
}

void readAddAptParameters(char* str,char** address,int* price,int* numRooms,short int* day,short int* month,short int* year)
{
	// This function read paremeters for add-apt function, and save them in the input parameters.
    int numOfWords,numOfParas;
    char** words=splitString(str+1,'"',&numOfWords);
    char** parameters=splitString(words[1]+1,' ',&numOfParas);
    (*address)=words[0];
    (*price)=convChToInt(parameters[0]);
    (*numRooms)=convChToInt(parameters[1]);
    (*day)=convChToInt(parameters[2]);
    (*month)=convChToInt(parameters[3]);
    (*year)=convChToInt(parameters[4])+2000;
}

char* getString()
{
	// This function receive a string in an unknown size from user.
    int currSize=0,allocSize=1;
    char* str=(char*)malloc(allocSize);
    char currChar;
    while ((currChar=getc(stdin)) && currChar!='\n'){
        str[currSize]=currChar;
        currSize++;
        if (currSize==allocSize){
            allocSize*=2;
            str=(char*)realloc(str,allocSize);
        }
    }
    str[currSize++]='\0';
    str=(char*)realloc(str,currSize);
	return str;
}
