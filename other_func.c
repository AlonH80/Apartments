#include "apartments.h"

//This function frees apartment list nodes and their addresses
void freeAppartment(aptNode* apart)
{
    free(apart->data.aptAddress);
    free(apart);
}

//This function returns number of apartments in list
int getNumOfAparts(aptList* apts)
{
    aptNode* apart=apts->head;
    int numOfAparts=0;
    while (apart){
        numOfAparts++;
        apart=apart->next;
    }
    return numOfAparts;
}


//This function finds the current maximum apartment code
int findMaxCode(aptList aptlst)
{
    int max=0;
    aptNode* curr=aptlst.head;
    if (!isEmptyAptList(aptlst)){
        while (curr){
            if (max<curr->data.aptCode){
                max=curr->data.aptCode;
            }
            curr=curr->next;
        }
    }
    return max;
}

//This function swaps between nodes
void swap(aptNode *a, aptNode *b)
{
	apt temp = a->data;
	a->data = b->data;
	b->data = temp;
}
