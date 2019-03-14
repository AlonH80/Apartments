#include "apartments.h"

Time substractTime(Time currTime,int days)
{
	// This function return the date of (current date - days)
    if (currTime.day<=days){
        days-=currTime.day;
        if (currTime.month==1){
            currTime.month=12;
            currTime.year--;
        }
        else{
            currTime.month-=1;
        }
        currTime.day=daysInMonth(currTime.month,currTime.year);
        return substractTime(currTime,days);		// Call the function recursively, since the max days substract in every iter is the number of days in the month.
    }
    else{
        currTime.day-=days;
        return currTime;
    }
}

BOOL isLeapYear(int year)
{
	// This function checks if a given year is a leap year (February has 29 days).
    if (year % 4) {return FALSE;}
    else if (year % 100) {return FALSE;}
    else if (year % 400) {return FALSE;}
    return TRUE;
}

int daysInMonth(int month,int year)
{
	// This function returns the number of days in month.
	int actualDays = 0;
    if (month>=1 && month<=7){
        if (month%2){
			actualDays = 31;
        }
        else if (month==2){
            if (isLeapYear(year)){
				actualDays = 29;
            }
			else {
				actualDays = 28;
			}
        }
        else{
			actualDays = 30;
        }
    }

    else{
        if (month%2){
			actualDays = 30;
        }
        else{
			actualDays = 31;
        }
    }
	return actualDays;
}

int compareTime(Time t1,Time t2)
{
	// This function returns -1 if t1 is later then t2, 0 if the time is equal, and 1 if t2 is later then t1.
    if (t1.year==t2.year){
        if (t1.month==t2.month){
            if (t1.day==t2.day){
                return 0;
            }
            else if (t1.day>t2.day){
                return -1;
            }
            else{
                return 1;
            }
        }

        else if (t1.month>t2.month){
            return -1;
        }
        else{
            return 1;
        }
    }
    else if (t1.year>t2.year){
        return -1;
    }
    else{
        return 1;
    }
}

void convToDate(char* str,Time* t)
{
	// This function converts ctime string into dates.
    char* dayString=getSubString(str,0,2);
    char* monthString=getSubString(str,3,5);
    char* yearString=getSubString(str,5,9);
    t->day=convChToInt(dayString);
    t->month=convChToInt(monthString);
    t->year=convChToInt(yearString);
    free(dayString);
    free(monthString);
    free(yearString);
}

int isFirstDateLater(char* firstDate, short int secDateDay, short int secDateMon, short int secDateYear)
{
	// This function checks if firstDate is later then given second date.
	char firstDateDay[3]={ '\0' };
	char firstDateMon[3]={ '\0' };
	char firstDateYear[5]={'\0'};
	short int firstDateDayInt=convChToInt(getSubString(firstDate,0,2));
	short int firstDateMonInt=convChToInt(getSubString(firstDate,2,4));
	short int firstDateYearInt=convChToInt(getSubString(firstDate,4,8));

	if (secDateYear < firstDateYearInt)
		return 1;
	else if (secDateYear == firstDateYearInt)
	{
		if (secDateMon < firstDateMonInt)
			return 1;
		else if (secDateMon == firstDateMonInt)
		{
			if (secDateDay <= firstDateDayInt)
				return 1;
		}
	}


	return 0;
}

int getMonth(char* monStr)
{
	// This function return the month's number according to his name.
    char* months[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    int i=0;
    while (strcmp(months[i],monStr)!=0){
        i++;
    }
    return i+1;
}

Time getTime(time_t timeSecs)
{
	// This function converts time in secs into struct Time.
    Time currTime;
    int numOfStrings;
    char** timeStrings=splitString(ctime(&timeSecs),' ',&numOfStrings);
    currTime.year=convChToInt(timeStrings[4]);
    currTime.day=convChToInt(timeStrings[2]);
    currTime.month=getMonth(timeStrings[1]);
    freeArOfStrings(timeStrings,numOfStrings);
    return currTime;
}

void copyTime(Time* to,Time from)
{
	// This function copy the time from "from" to "to.
    to->day=from.day;
    to->month=from.month;
    to->year=from.year;
}
