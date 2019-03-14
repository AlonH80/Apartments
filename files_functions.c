#include "apartments.h"

unsigned char* convertNumRoomsDate(short int numRooms, short int day, short int month, short int year)
{
	// This function convert 4 short ints into 3 bytes, in order to save them in the binary files
	unsigned char* allData = (unsigned char*)calloc(3,sizeof(char));
	allData[0] = (unsigned char)numRooms << 4;
	allData[0] |= (unsigned char)day >> 1;
	allData[1] = (unsigned char)day << 7;
	allData[1] |= (unsigned char)month << 3;
	allData[1] |= (unsigned char)(year - 2000) >> 4;
	allData[2] = (unsigned char)(year - 2000) << 4;
	return allData;
}

unsigned short int convSavedDate(Time savedDate)
{
	// This function converts savedDate into a short int in order to save them in the binary file.
    unsigned short int compDate=0;
    compDate=compDate|(savedDate.year-2000);
    compDate=compDate|(savedDate.month<<7);
    compDate=compDate|(savedDate.day<<11);
	return compDate;
}

void saveAptInFile(FILE* binFile,apt apart)
{
	// This function save a single apartment in a binary file.
    unsigned short int aptAddLen=(unsigned short int)strlen(apart.aptAddress);
    unsigned char* roomsDateData=convertNumRoomsDate(apart.numRooms,apart.entDay,apart.entMonth,apart.entYear);
    unsigned short int compSaveDate=convSavedDate(apart.savedDate);
    fwrite(&apart.aptCode,sizeof(short int),1,binFile);
    fwrite(&aptAddLen,sizeof(short int),1,binFile);
    fwrite(apart.aptAddress,sizeof(char),aptAddLen,binFile);
    fwrite(&apart.aptPrice,sizeof(int),1,binFile);
    fwrite(roomsDateData,3,1,binFile);
    fwrite(&compSaveDate,sizeof(unsigned short int),1,binFile);
	free(roomsDateData);
}

void saveAppartments(aptList apartments)
{
	// This function save all the apartments into a binary file.
    aptNode* currApt=apartments.head;
    FILE* aptFile=fopen("apartments.bin","wb");
    while (currApt){
        saveAptInFile(aptFile,currApt->data);
        currApt=currApt->next;
    }
    fclose(aptFile);
}

void unpackRoomsEntDateData(unsigned char* compData, apt* apart)
{
	// This function convert 3 bytes into 3 short ints (day,month,year).
	apart->numRooms = (short int)(compData[0] >>  4);
	apart->entDay = (short int)(((compData[0]<<1)+(compData[1]>>7)) & 31);
	apart->entMonth = (short int)((compData[1] >> 3) & 15);
	apart->entYear = (short int)(((compData[1] <<4) | (compData[2] >> 4))& 127);
	apart->entYear += 2000;
}

void unpackSavedDate(unsigned short int compDate,apt* apart)
{
	// This function convert short int into 3 ints, who will be saved in savedDate of given apartment.
    apart->savedDate.day=(compDate)>>11;
    apart->savedDate.month=(compDate>>7)&15;
    apart->savedDate.year=(compDate&127)+2000;
}

int loadApt(FILE* binFile,apt* currApt)
{
	// This function load a single apartment froom binFile to currApt.
	short int addLen;
    unsigned char* compRoomsDate=(unsigned char*)malloc(3);
    unsigned short int compSavedDate;
    int bytesRead;
	char* currStr;
    bytesRead=fread(&currApt->aptCode,sizeof(short int),1,binFile);
	if(bytesRead) {
		fread(&addLen, sizeof(short int), 1, binFile);
		currStr = (char*)malloc(addLen + 1);
		fread(currStr, sizeof(char), addLen, binFile);
		currStr[addLen] = '\0';
		currApt->aptAddress = currStr;
		fread(&currApt->aptPrice, sizeof(int), 1, binFile);
		fread(compRoomsDate, 3, 1, binFile);
		bytesRead = fread(&compSavedDate, sizeof(unsigned short int), 1, binFile);
		unpackSavedDate(compSavedDate, currApt);
		unpackRoomsEntDateData(compRoomsDate, currApt);
	}
	free(compRoomsDate);
    return bytesRead;
}

aptList loadAppartments()
{
	// This function load all the apartments in binary file, and put them in an aptList.
    int bytesRead;
    aptList loadAptList=makeEmptyAptList();
    FILE* aptFile=fopen("apartments.bin","rb");
	if (aptFile == NULL) {
        return loadAptList;
    }
    apt newApt;
    bytesRead=loadApt(aptFile,&newApt);
    if (bytesRead){
        loadAptList.head=createAptNode(newApt,NULL,NULL);
        loadAptList.tail=loadAptList.head;
    }
    else{
        return loadAptList;
    }
    aptNode* currNode=loadAptList.head;
    while (bytesRead){
        bytesRead=loadApt(aptFile,&newApt);
        if (bytesRead){
            currNode->next=createAptNode(newApt,NULL,currNode);
            currNode=currNode->next;
            loadAptList.tail=currNode;
        }
    }
    fclose(aptFile);
    return loadAptList;
}

//Function to write short term arr to text file
void writeShortTermToFile(char* fileName, char** short_term_historyArr)
{
	// This function save short_term_history in txt file.
	int i = 0, j;
	FILE * fp;

	while (short_term_history[i])	//find actual size of short term arr
		i++;

	fp = fopen(fileName, "w");  

	for (j = i - 1; j >= 0; j--)
		fprintf(fp, "%s\n", short_term_history[j]);

	fclose(fp);
}

//Function to write commands list to text file
void writeCmdLstToFile(char* fileName, cmdList lst)
{
	// This function write the commands list (history that's not short term) to txt file.
	cmdNode* curr;
	FILE * fp;
	fp = fopen(fileName, "a");

	for (curr = lst.tail; curr != NULL; curr = curr->prev)
		fprintf(fp, "%s\n", curr->command);

	fclose(fp);
}

//Function to write all commands databases to text file
void writeHistoryToFile(char* fileName, char** short_term_historyArr, cmdList lst)
{
	// This function call the function to write the entire history to txt file
	writeShortTermToFile(fileName, short_term_historyArr);
	writeCmdLstToFile(fileName, lst);
}

//This function reads from a text file containing commands, and loads them to short term arr and command list
// 7 newest commands will be loaded to short term arr
// all the rest will be loaded to commands list
//In the file they are arranged from newest to oldest, in the arr and list it's the opposite
void loadHistoryFromFile(char* fileName, char** short_term_historyArr, cmdList* lst)
{
	FILE * file;
	int numOfLines, i,cmd_i = 0;
	char c;
	char* line;

	file = fopen(fileName, "r");

	if (!file)	//First execution of func - the file doesn't exist
		return;

	numOfLines = getNumOfLines(fileName);
	line = (char*)malloc(256);	//256 is used as a default size for string that will be reallocated

	if (numOfLines <= N)	//we only insert to short term arr
	{
		i = numOfLines - 1;
		while ((c = fgetc(file)) != EOF)
		{
			if (c == '\n')
			{
				line[cmd_i] = '\0';
				line = (char*)realloc(line, cmd_i + 1);
				short_term_historyArr[i] = (char*)malloc(cmd_i + 1);
				strcpy(short_term_historyArr[i], line);
				free(line);
				cmd_i = 0;
				i--;
				line = (char*)malloc(256);
			}
			else
			{
				line[cmd_i] = c;
				cmd_i++;
				if (cmd_i > 256)
					line = (char*)realloc(line, cmd_i * 2);
			}
		}
		free(line);
	}

	else
	{
		numOfLines -= N;
		//first we insert the command to short term arr
		i = N - 1;
		while ((c = fgetc(file)) != EOF)
		{
			if (i >= 0)
			{
				if (c == '\n')
				{
					line[cmd_i] = '\0';
					line = (char*)realloc(line, cmd_i + 1);
					short_term_historyArr[i] = (char*)malloc(cmd_i + 1);
					strcpy(short_term_historyArr[i], line);
					free(line);
					cmd_i = 0;
					i--;
					line = (char*)malloc(256);
				}
				else
				{
					line[cmd_i] = c;
					cmd_i++;
					if (cmd_i > 256)
						line = (char*)realloc(line, cmd_i * 2);
				}
			}

			else //now we insert the rest of the commands to cmd lst
			{
				if (c == '\n')
				{
					line[cmd_i] = '\0';
					line = (char*)realloc(line, cmd_i + 1);

					insertDataToStartList(lst, line);
					lst->head->commandId = numOfLines;
					numOfLines--;

					free(line);
					cmd_i = 0;
					line = (char*)malloc(256);
				}
				else
				{
					line[cmd_i] = c;
					cmd_i++;
					if (cmd_i > 256)
						line = (char*)realloc(line, cmd_i * 2);
				}
			}
		}
		free(line);
	}

	fclose(file);
}

//Function to create and insert command node to start of list
void insertDataToStartList(cmdList* lst, char* command)
{
	cmdNode* newHead = createCmdNode(command, lst->head, NULL, *lst);

	if (isEmptyCmdList(*lst))
	{
		lst->head = lst->tail = newHead;
	}
	else
	{
		lst->head->prev = newHead;
		lst->head = newHead;
	}
}

//Function to return number of lines in a given text file
int getNumOfLines(char* fileName)
{
	FILE * fp;
	int cnt = 0;
	char c;

	fp = fopen(fileName, "r");

	for (c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n')
			cnt++;

	fclose(fp);
	return cnt;
}
