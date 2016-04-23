/********
caltool.c -- iCalendar function implementations for caltool.h
Last updated:  1:00 PM March-18-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********/
#include "caltool.h"

/*******************HELPER FUNCTION PROTOTYPES************************/
/*********************************************************************
 * FUNCTION NAME: printDate
 * PURPOSE: Prints date stored inside tm struct in proper format.
 * ARGUMENTS: . Struct that stores the data(struct tm tm).
 *			  . Where the date will be written to(FILE *out).
 * RETURNS: IOERR(CalError) if error occurs printing.
 *********************************************************************/
static CalError printDate(struct tm tm,FILE *out);
/*********************************************************************
 * FUNCTION NAME: dateToTime
 * PURPOSE: Converts a date stored in a string in ics format
 *			to data stored inside tm struct
 * ARGUMENTS: . String containing the date(char *date).
 * RETURNS: Struct with the data stored within it (struct tm).
 *********************************************************************/
static struct tm dateToTm(char *date);
/*********************************************************************
 * FUNCTION NAME: getCompNum
 * PURPOSE: Count the number of components in a calendar file
 *		    and how often each type appears.
 * ARGUMENTS: . Where the components are stored(CalComp *comp).
 *			  . Where the number of components is
 *				stored(int *compNum).
 *			  . Where the number of events is stored(int *events).
 *			  . Where the number of todo's is stored(int *todo).
 *			  . Where the number of other types of components
 *				is stored(int *other).
 *********************************************************************/
static void getCompNum(const CalComp *comp, int *compNum,int *events
	, int *todo,int *other);
/*********************************************************************
 * FUNCTION NAME: getSubCompNum
 * PURPOSE: Gets the number of subcomponents in a 
 * 			CalComp structure.
 * ARGUMENTS: . CalComp object that stores the 
 *				subcomponents(const CalComp *comp).
 * RETURNS: Number of subcomponents (const CalComp *comp).
 *********************************************************************/
static int getSubCompNum(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: getPropNum
 * PURPOSE: Gets the number of properties in a 
 * 			CalComp structure.
 * ARGUMENTS: . CalComp structure that stores the 
 *				properties(const CalComp *comp).
 * RETURNS: Number of properties (const CalComp *comp).
 *********************************************************************/
static int getPropNum(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: initializeTmMin
 * PURPOSE: Initializes struct tm members to 0.
 * ARGUMENTS: . Struct tm to be initialized(struct tm).
 *********************************************************************/
static void initializeTmMax(struct tm *tm);
/*********************************************************************
 * FUNCTION NAME: initializeTmMax
 * PURPOSE: Initializes struct tm members to maximum value.
 * ARGUMENTS: . Struct tm to be initialized(struct tm).
 *********************************************************************/
static void initializeTmMin(struct tm *tm);
/*********************************************************************
 * FUNCTION NAME: compareStr
 * PURPOSE: Compare two void pointers that are strings.
 * ARGUMENTS: . First element to be compared.
 *			  . Second element to be compared.
 *********************************************************************/
static int compareStr(void const *comp1, void const *comp2);
/*********************************************************************
 * FUNCTION NAME: initializeTm
 * PURPOSE: Initializes struct tm members to 0.
 * ARGUMENTS: . struct tm to be initialized(struct tm *tm).
 *********************************************************************/
static void initializeTm(struct tm *tm);
/*********************************************************************
 * FUNCTION NAME: getCopy
 * PURPOSE: Get copy of a string.
 * ARGUMENTS: . String to be copied(char *string).
 * RETURNS: . Copy of the string(char *).
 *********************************************************************/
static char *getCopy(char *string);
/*********************************************************************
 * FUNCTION NAME: removeDups
 * PURPOSE: Remove any duplicates in string array and adjusts count.
 * ARGUMENTS: . String array(char **names).
 *			  . Amount of string in array(int *count).
 *********************************************************************/
static void removeDups(char **names,int *count);
/*********************************************************************
 * FUNCTION NAME: printInfoDate
 * PURPOSE: Print date in -info command format.
 * ARGUMENTS: . Date to be printed(struct tm tm).
 *			  . Where the string should be printed to(FILE *out).
 * RETURNS: . Any errors that occur during printing(CalError).
 *********************************************************************/
static CalError printInfoDate(struct tm tm,FILE *out);
/*********************************************************************
 * FUNCTION NAME: printCompNum
 * PURPOSE: Print amount of each type of component in -info format.
 * ARGUMENTS: . Where the number should be printed(FILE *txtfile).
 *			  . Number of components(int comp).
 *			  . Number of events(int eve).
 *			  . Number of todos(int todo).
 *			  . Number of other types of components(int other).
 * RETURNS: . CalStatus with amount of lines printed 
 *  		  and any errors(CalStatus).
 *********************************************************************/
static CalStatus printCompNum(FILE *txtfile, int comp, int eve
	, int todo, int other);
/*********************************************************************
 * FUNCTION NAME: getCompDate
 * PURPOSE: Get the starting date of a component.
 * ARGUMENTS: . CalComponents date to be gotten(const CalComp *comp).
 * RETURNS: . Date found (struct tm).
 *********************************************************************/
static struct tm getCompDate(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: getDateRange
 * PURPOSE: Get the earliest and latest date found in a component.
 * ARGUMENTS: . Components date to be found(const CalComp *comp).
 *			  . Earliest date found will be stored (struct tm *from).
 *			  . Latest date found will be stored (struct tm *to).
 * RETURNS: . Whether a date was found(bool).
 *********************************************************************/
static bool getDateRange(const CalComp *comp
	,struct tm *from, struct tm *to);
/*********************************************************************
 * FUNCTION NAME: getCN
 * PURPOSE: Get all common names found in a component.
 * ARGUMENTS: . Component that stores the common 
 *				names(const CalComp comp).
 *			  . Where the number of common names found 
 * 				will be stored(int *cnCount).
 * RETURNS: . All common names found in comp(char **).
 *********************************************************************/
static char **getCN(const CalComp *comp, int *cnCount);
/*********************************************************************
 * FUNCTION NAME: compareStr
 * PURPOSE: Compares two strings, their arguments being void pointers.
 * ARGUMENTS: . String being compared to(void const *comp1).
 *			  . String being compared to the 1st 
 *			    string(void const *comp2).
 * RETURNS: . Difference in strings(int).
 *********************************************************************/
static int compareStr(void const *comp1, void const *comp2);
/*********************************************************************
 * FUNCTION NAME: compareDate
 * PURPOSE: Compare two dates and get the difference.
 * ARGUMENTS: . Date being compared to(void const *comp1).
 *			  . Date being compared to comp1(void const *comp2).
 * RETURNS: . Difference between date comp1 and date comp2(int).
 *********************************************************************/
static int compareDate(void const *comp1, void const *comp2);
/*********************************************************************
 * FUNCTION NAME: initializeEvent
 * PURPOSE: Initialize an event node.
 * ARGUMENTS: . Event node to be initialized(CalEvent **event).
 *********************************************************************/
static void initEvent(CalEvent **event);
/*********************************************************************
 * FUNCTION NAME: initializeEventHead
 * PURPOSE: Initialize an event linked list head.
 * ARGUMENTS: . Event head to be initialized(CalEventHead **head).
 *********************************************************************/
static void initEventHead(CalEventHead **head);
/*********************************************************************
 * FUNCTION NAME: initXprop
 * PURPOSE: Initialize a X-PROP linked list node.
 * ARGUMENTS: . X-PROP node to be initialized (Xprop **xprop).
 *********************************************************************/
static void initXprop(XProp **xprop);
/*********************************************************************
 * FUNCTION NAME: initXpropHead
 * PURPOSE: Initialize a X-PROP linked list head.
 * ARGUMENTS: . X-PROP head to be initialized (XPropHead **head).
 *********************************************************************/
static void initXpropHead(XPropHead **head);
/*********************************************************************
 * FUNCTION NAME: addEvent
 * PURPOSE: Add event to an events linked list.
 * ARGUMENTS: . Head of an events linked list(CalEventHead *head).
 *			  . Event node to be added (CalEvent *add).
 *********************************************************************/
static void addEvent(CalEventHead *head, CalEvent *add);
/*********************************************************************
 * FUNCTION NAME: addXprop
 * PURPOSE: Add X-PROP to an X-PROP linked list.
 * ARGUMENTS: . Head of an X-PROP linked list(XPropHead *head).
 *			  . X-PROP node to be added (XProp *add).
 *********************************************************************/
static void addXprop(XPropHead *head, XProp *add);
/*********************************************************************
 * FUNCTION NAME: addCalProp
 * PURPOSE: Add calendar property to a property linked list.
 * ARGUMENTS: . Head of an property linked list(CalProp *head).
 *			  . Property node to be added (CalProp *add).
 *********************************************************************/
static void addCalProp(CalProp *head, CalProp *add);
/*********************************************************************
 * FUNCTION NAME: isXProp
 * PURPOSE: Checks whether a string is an X-PROPERTY.
 * ARGUMENTS: . String to be checked(char *str).
 * RETURNS: Whether the string is an X-PROP(char *str).
 *********************************************************************/
static bool isXProp(char *str);
/*********************************************************************
 * FUNCTION NAME: getXProps
 * PURPOSE: Gets all the X-PROPS found in a calendar component.
 * ARGUMENTS: . Component to look for the 
 *			    X-PROPS in(const CalComp *comp).
 * RETURNS: All X-PROPS found stored in linked list (XPropHead *).
 *********************************************************************/
static XPropHead *getXProps(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: freeXProps
 * PURPOSE: Frees XProp linked list.
 * ARGUMENTS: . Head of linked list to be freed(XProp *prop).
 *********************************************************************/
static void freeXProps(XProp *prop);
/*********************************************************************
 * FUNCTION NAME: freeCalEvents
 * PURPOSE: Frees CalEvents linked list.
 * ARGUMENTS: . Head of linked list to be freed(CalEvents *event).
 *********************************************************************/
static void freeCalEvents(CalEvent *event);
/*********************************************************************
 * FUNCTION NAME: getEvents
 * PURPOSE: Gets all the events found in a calendar component.
 * ARGUMENTS: . Component to look for the 
 *			    events in(const CalComp *comp).
 * RETURNS: All events found stored in linked list (CalEventHead *).
 *********************************************************************/
static CalEventHead *getEvents(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: initializeProp
 * PURPOSE: Intializes CalProps members to NULL or 0.
 * ARGUMENTS: . Calendar property to be initialized(CalProp *prop).
 *********************************************************************/
static void initializeProp(CalProp *prop);
/*********************************************************************
 * FUNCTION NAME: getParamsCopy
 * PURPOSE: Gets copy of calendar parameter.
 * ARGUMENTS: . Parameter to be copied(CalParam *param).
 * RETURNS: Copy of the parameter(CalParam *).
 *********************************************************************/
static CalParam *getParamsCopy(CalParam *param);
/*********************************************************************
 * FUNCTION NAME: freeCalParam
 * PURPOSE: Frees CalParam linked list.
 * ARGUMENTS: . Head of linked list to be freed(CalParam *param).
 *********************************************************************/
static void freeCalParam(CalParam **param);
/*********************************************************************
 * FUNCTION NAME: removeProp
 * PURPOSE: Removes property with matching name from linked list.
 * ARGUMENTS: . Linked list to be removed from (CalProp **prop).
 *			  . Name to be removed(char *name).
 * RETURNS: . Whether a name was removed.
 *********************************************************************/
static bool removeProp(CalProp **prop,char *name);
/*********************************************************************
 * FUNCTION NAME: getCalCompCopy
 * PURPOSE: Gets a copy of a calendar component.
 * ARGUMENTS: . Calendary component to be copied(const CalComp *comp).
 * RETURNS: . Copy of the calendar component(CalComp *).
 *********************************************************************/
CalComp *getCalCompCopy(const CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: getStrFromTo
 * PURPOSE: Gets string version of the from and to date.
 * ARGUMENTS: . All arguments from command line(char **arguments).
 *			  . Where the from date will be stored (char **from).
 *			  . Where the to date will be stored (char **to).
 * RETURNS: Whether the arguments were formatted correctly(bool).
 *********************************************************************/
static bool getStrFromTo(char **arguments,char **from,char **to);
/*********************************************************************
 * FUNCTION NAME: getFromTo
 * PURPOSE: Convert string version of from, and to dates to time_t.
 * ARGUMENTS: . String with from date(char *strfrom).
 *            . String with to date(char *strto).
 *			  . From date in time_t(time_t *timefrom).
 *			  . To date in time_t(time_t *timeto).
 *			  . String that returned the error.
 * RETURNS: . Error number, 0 means no error.
 *********************************************************************/
static int getFromTo(char *strfrom,char *strto
	,time_t *timefrom,time_t *timeto,char **errstr);
/*********************************************************************
 * FUNCTION NAME: printCmdSyntax
 * PURPOSE: Print caltool syntax instructions to stderr.
 *********************************************************************/
static void printCmdSyntax();
/*******************HELPER FUNCTION PROTOTYPES************************/

static void removeDups(char **names,int *count){
	for (int i=0;i<*count;i++){
		for (int j=0;j<*count;j++){
			/*Check for duplicate*/
			if (i != j && !strcmp(names[i],names[j])){
				/*Free one duplicate and shift array left*/
				free(names[j]);
				names[j] = NULL;
				for (int k=j;k<(*count)-1;k++){
					names[k] = names[k+1];
				}
				/*Reset search*/
				(*count)--;
				i=0;
				j=0;
			}
		}
	}
}

static CalError printDate(struct tm tm,FILE *out){
	char buffer[_MAX_DATE_LEN];

	strftime(buffer, _MAX_DATE_LEN, "%Y-%b-\%d %l:%M \%p", &tm );

	/*Check for error*/
	if (fprintf(out,"%s",buffer) < 0){
		return IOERR;
	}
	return OK;
}
static CalError printInfoDate(struct tm tm,FILE *out){
	char buffer[_MAX_DATE_LEN];

	strftime(buffer, _MAX_DATE_LEN, "%Y-%b-\%d", &tm );

	/*Check for error*/
	if (fprintf(out,"%s",buffer) < 0){
		return IOERR;
	}
	return OK;
}
static struct tm dateToTm(char *date){
	int numYear=0,numMonth=0,numDay=0,numHour=0,numMinute=0;
	int numSecond=0;
	char *strYear,*strMonth,*strDay,*strHour,*strMinute,*strSecond;
	char *token=NULL,*datecpy=NULL,*rdate=NULL;
	struct tm tm;


	/*Check for TZID time*/
	datecpy = getCopy(date);
	rdate = getCopy(date);

	token = strtok(datecpy,"=");
	if (token && !strcmp(token,"TZID")){
		/*Store date as whatever is after the colon*/
		token = strtok(NULL,":");
		if (token){
			token = strtok(NULL,":");
		}
		if (token){
			free(rdate);
			rdate = malloc(sizeof(char)*(strlen(token)+1));
			assert(rdate);
			strcpy(rdate,token);
		}
	}
	free(datecpy);

	/*Store year*/
	strYear = malloc(sizeof(char)*5);
	assert(strYear);
	for (int i=0;i<4;i++){
		strYear[i] = rdate[i];
	}
	strYear[4] = '\0';
	numYear = strtol(strYear,NULL,10);
	free(strYear);

	/*Store month*/
	strMonth = malloc(sizeof(char)*3);
	assert(strMonth);
	for (int i=4;i<6;i++){
		strMonth[i-4] = rdate[i];
	}
	strMonth[2] = '\0';
	numMonth = strtol(strMonth,NULL,10);
	free(strMonth);

	/*Store day*/
	strDay = malloc(sizeof(char)*3);
	assert(strDay);
	for (int i=6;i<8;i++){
		strDay[i-6] = rdate[i];
	}
	strDay[2] = '\0';
	numDay = strtol(strDay,NULL,10);
	free(strDay);

	/*Store hour*/
	strHour = malloc(sizeof(char)*3);
	assert(strHour);
	for (int i=9;i<11;i++){
		strHour[i-9] = rdate[i];
	}
	strHour[2] = '\0';
	numHour = strtol(strHour,NULL,10);
	free(strHour);

	/*Store minute*/
	strMinute = malloc(sizeof(char)*3);
	assert(strMinute);
	for (int i=11;i<13;i++){
		strMinute[i-11] = rdate[i];
	}
	strMinute[2] = '\0';
	numMinute = strtol(strMinute,NULL,10);
	free(strMinute);

	/*Store second*/
	strSecond = malloc(sizeof(char)*3);
	assert(strSecond);
	for (int i=13;i<15;i++){
		strSecond[i-13] = rdate[i];
	}
	strSecond[2] = '\0';
	numSecond = strtol(strSecond,NULL,10);
	free(strSecond);

	/*Store data in tm struct*/

	tm.tm_year = (numYear - 1900);
	tm.tm_mon = (numMonth - 1);
	tm.tm_mday = numDay;
	tm.tm_hour = numHour;
	tm.tm_min = numMinute;
	tm.tm_sec = numSecond;
	tm.tm_isdst = -1;

	free(rdate);

	return tm;
}

static void getCompNum(const CalComp *comp, int *compNum,int *events
	, int *todo,int *other){

	/*Count number of components and each type of component*/
	for (int i=0;i<comp->ncomps;i++){
		if (!strcmp(((comp->comp)[i])->name,"VEVENT")){
				(*compNum)++;
				(*events)++;
		}
		else if (!strcmp(((comp->comp)[i])->name,"VTODO")){
				(*compNum)++;
				(*todo)++;
		}
		else{
			(*compNum)++;
			(*other)++;
		}
	}
}

static CalStatus printCompNum(FILE *txtfile, int comp, int eve
	, int todo, int other){
	int ret;
	CalStatus calStatus;

	/*Initialize CalStatus*/
	calStatus.code = OK;
	calStatus.linefrom = calStatus.lineto = 0;

	/*Print number of components*/
	if (comp == 1){
		ret = fprintf(txtfile,"1 component: ");
	}
	else{
		ret = fprintf(txtfile,"%d components: ",comp);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}
	calStatus.lineto++;
	calStatus.linefrom++;

	/*Print number of events*/
	if (eve == 1){
		ret = fprintf(txtfile,"1 event, ");
	}
	else{
		ret = fprintf(txtfile,"%d events, ",eve);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;	
	}
	calStatus.lineto++;
	calStatus.linefrom++;

	/*Print number of todo's*/
	if (todo == 1){
		ret = fprintf(txtfile,"1 todo, ");
	}
	else{
		ret = fprintf(txtfile,"%d todos, ",todo);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}
	calStatus.lineto++;
	calStatus.linefrom++;

	/*Print number of other types of components*/
	if (other == 1){
		ret = fprintf(txtfile,"1 other\n");
	}
	else{
		ret = fprintf(txtfile,"%d others\n",other);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}
 	calStatus.lineto++;
	calStatus.linefrom++;

	return calStatus;
}

static int getSubCompNum(const CalComp *comp){
	static bool orig=true;
	int subCompCount=0;

	if (!orig){
		/*Add subcomponents in this subtree*/
		subCompCount += comp->ncomps;
	}
	/*Don't add to subCompCount if were in the main tree*/
	else{
		orig = false;
	}

	/*Get all subcomponents from all subtrees*/
	for (int i=0;i<comp->ncomps;i++){
		subCompCount += getSubCompNum((comp->comp)[i]);
	}

	return subCompCount;
	
}

static int getPropNum(const CalComp *comp){
	int propCount=0;

	/*Add properties in this subtree*/
	propCount += comp->nprops;

	/*Get all subcomponents from all subtrees*/
	for (int i=0;i<comp->ncomps;i++){
		propCount += getPropNum((comp->comp)[i]);
	}

	return propCount;
}

static struct tm getCompDate(const CalComp *comp){

	struct tm date;
	CalProp *next;

	/*Initialize struct tm*/
	initializeTm(&date);

	/*Traverse all the dates and get the min and max
	  in this tree*/
	next = comp->prop;
	for (int i=0;i<comp->nprops;i++){

		/*Check prop name to see if it is a date
		  and store date inside struct*/
		if(!strcmp(next->name,"DTSTART")
			|| !strcmp(next->name,"DUE")){
			date = dateToTm(next->value);
		}
		next = next->next;
	}

	return date;

}

static bool getDateRange(const CalComp *comp
	,struct tm *from, struct tm *to){

	static bool foundDate = false,returnDate=false;	
	double timediff = 0;
	struct tm subfrom, subto, temptm;
	CalProp *next;

	/*Initialize struct tm*/
	initializeTmMax(&subfrom);
	initializeTmMin(&subto);
	initializeTmMax(&temptm);

	for (int i=0;i<comp->ncomps;i++){
		getDateRange((comp->comp)[i],&subfrom,&subto);

		/*Check if earlier date was found than the min*/
		timediff = difftime(mktime(&subfrom),mktime(from));
		if (timediff < 0){
			*from = subfrom;
		}

		timediff = difftime(mktime(&subto),mktime(to));
		/*Check if later date was found than the max*/
		if (timediff > 0){
			*to = subto;
		}
	}

	/*Traverse all the dates and get the min and max
	  in this tree*/
	next = comp->prop;
	for (int i=0;i<comp->nprops;i++){

		/*Check prop name to see if it is a date
		  and store date inside struct*/
		if (!strcmp(next->name,"COMPLETED")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"DTEND")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"DUE")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"DTSTART")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"CREATED")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"DTSTAMP")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else if(!strcmp(next->name,"LAST-MODIFIED")){
			temptm = dateToTm(next->value);
			foundDate = true; 
		}
		else{
			foundDate = false;
		}

		if (foundDate){
			returnDate = true;
		}

		if (foundDate){
			/*Check if date is less than min*/
			timediff = difftime(mktime(&temptm),mktime(from));
			if (timediff < 0){
				*from = temptm;
			}

			/*Check if date is greater than max*/
			timediff = difftime(mktime(&temptm),mktime(to));
			if (timediff > 0){
				*to = temptm;
			}
		}

		next = next->next;
	}

	return returnDate;

}

static void initializeTmMin(struct tm *tm){
    tm->tm_sec = 0;
    tm->tm_min = 0;
    tm->tm_hour = 0;
    tm->tm_mday = 0;
    tm->tm_mon = 0;
    tm->tm_year = INT_MIN;
    tm->tm_isdst = -1;
}

static void initializeTmMax(struct tm *tm){
    tm->tm_sec = 59;
    tm->tm_min = 59;
    tm->tm_hour = 23;
    tm->tm_mday = 31;
    tm->tm_mon = 11;
    tm->tm_year = INT_MAX;
    tm->tm_isdst = -1;
}

static void initializeTm(struct tm *tm){
	tm->tm_sec = 0;
    tm->tm_min = 0;
    tm->tm_hour = 0;
    tm->tm_mday = 0;
    tm->tm_mon = 0;
    tm->tm_year = 0;
    tm->tm_isdst = -1;	
}

static char **getCN(const CalComp *comp, int *cnCount){

	char **comName=NULL,**subcomName=NULL;
	int curCnCount=0,subCnCount=0;
	CalProp *nextProp;
	CalParam *nextParam;

	for (int i=0;i<comp->ncomps;i++){
		subcomName = getCN((comp->comp)[i],&subCnCount);

		/*Add names in subtree to array of common names*/
		curCnCount += subCnCount;
		if (subCnCount > 0){			
			comName = realloc(comName,sizeof(char*)
				*(curCnCount));
			assert(comName);
			for (int j=0;j<subCnCount;j++){
				comName[curCnCount-subCnCount+j] 
					= subcomName[j];
			}
		}

		/*Free only the **subcomName since were 
		stil using the memory allocared for its elements*/
		free(subcomName);
	}


	/*Go through organizers and store common names*/
	nextProp = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		/*If name is not ORGANIZER, go to next prop*/
		if (strcmp(nextProp->name,"ORGANIZER")){
			nextProp = nextProp->next;
			continue;
		}

		/*Traverse params looking for the name "CN"*/
		nextParam = nextProp->param;
		for (int j=0;j<nextProp->nparams;j++){

			if (!strcmp(nextParam->name,"CN")){

				/*Allocate memory for first CN*/
				if (curCnCount == 0){
					comName = malloc(sizeof(char*));
					assert(comName);
				}
				/*Allocate memory for current CN*/
				else{
					comName = realloc(comName,sizeof(char*)
						*(curCnCount+1));
					assert(comName);
				}

				/*Add the string to the array*/
				comName[curCnCount] = malloc(sizeof(char)
					*(strlen((nextParam->value)[0])+1));
				assert(comName[curCnCount]);
				strcpy(comName[curCnCount],(nextParam->value)[0]);
				curCnCount++;
			}

			nextParam = nextParam->next;
		}

		nextProp = nextProp->next;
	}

	*cnCount = curCnCount;
	return comName;
}

static int compareStr(void const *comp1, void const *comp2){
	char const *str1;
	char const *str2;

	str1 = *(char const**)comp1;
	str2 = *(char const**)comp2;

	return strcmp(str1,str2);
}

static int compareDate(void const *comp1, void const *comp2){
	struct tm date1;
	struct tm date2;

	date1 = *((struct tm *)comp1);
	date2 = *((struct tm *)comp2);

	return ((int)(mktime(&date1) - mktime(&date2)));
}

static void initEvent(CalEvent **event){
	*event = malloc(sizeof(CalEvent));
	assert(*event);
	(*event)->des = NULL;
	(*event)->date = NULL;
	(*event)->next = NULL;
}

static void initEventHead(CalEventHead **head){
	*head = malloc(sizeof(CalEventHead));
	assert(*head);
	(*head)->nevents = 0;
	(*head)->first = NULL;
}

static void initXprop(XProp **xprop){
	*xprop = malloc(sizeof(XProp));
	assert(*xprop);
	(*xprop)->name = NULL;
	(*xprop)->next = NULL;
}

static void initXpropHead(XPropHead **head){
	*head = malloc(sizeof(XPropHead));
	assert(*head);
	(*head)->count = 0;
	(*head)->first = NULL;
}

static void addEvent(CalEventHead *head, CalEvent *add){
	CalEvent *next;

	/*Add to first position*/
	if (!head->first){
		head->first = add;
		head->nevents++;
		return;
	}

	/*Add to last position*/
	next = head->first;
	while (next->next){
		next = next->next;
	}
	next->next = add;
	head->nevents++;
}

static void addXprop(XPropHead *head, XProp *add){
	XProp *next;

	/*Add to first position*/
	if (!head->first){
		head->first = add;
		head->count++;
		return;
	}

	/*Add to last position*/
	next = head->first;
	while (next->next){
		next = next->next;
	}
	next->next = add;
	head->count++;
}

static void addCalProp(CalProp *head, CalProp *add){
	CalProp *next;

	/*Add to first position*/
	if (!head){
		head = add;
		return;
	}

	/*Add to last position*/
	next = head;
	while (next->next){
		next = next->next;
	}
	next->next = add;
}

static bool isXProp(char *str){
	if (strlen(str) >= 2){
		if (str[0] == 'X' && str[1] == '-'){
			return true;
		}
	}

	return false;
}

static XPropHead *getXProps(const CalComp *comp){
	CalProp *nextProp;
	XPropHead *subXProps=NULL,*xprops=NULL;
	XProp *add=NULL;

	initXpropHead(&xprops);

	for (int i=0;i<comp->ncomps;i++){
		subXProps = getXProps((comp->comp)[i]);

		/*Add X-Props found in subtree to events*/
		if (subXProps->count > 0){	
			xprops->count += subXProps->count;	
			addXprop(xprops,subXProps->first);
			/*Account for calEvents adding one to count*/
			xprops->count--; 
		}
		free(subXProps);
	}

	/*Add X-Props in this tree*/
	nextProp = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		/*If an X-PROP copy the name and add to list*/
		if (isXProp(nextProp->name)){
			initXprop(&add);
			add->name = malloc(sizeof(char)
				*(strlen(nextProp->name)+1));
			assert(add->name);
			strcpy(add->name,nextProp->name);
			addXprop(xprops,add);
		}
		nextProp = nextProp->next;
	}

	return xprops;
}

static void freeXProps(XProp *prop){
	
	if (prop->next){
		freeXProps(prop->next);
	}

	if (prop->name){ free(prop->name); }
	free(prop);
}

static void freeCalEvents(CalEvent *event){

	if (event->next){
		freeCalEvents(event->next);
	}

	if (event->des){ free(event->des); }
	if (event->date){ free(event->date); }
	free(event);
}

static CalEventHead *getEvents(const CalComp *comp){
	CalEventHead *subEvents=NULL,*calEvents=NULL;
	CalEvent *add=NULL;
	CalProp *nextProp;
	bool event = false,summary=false;

	initEventHead(&calEvents);

	/*Get all events in the subtrees*/
	for (int i=0;i<comp->ncomps;i++){
		subEvents = getEvents((comp->comp)[i]);
	
		/*Add events found in subtree to events*/
		if (subEvents->nevents > 0){	
			calEvents->nevents += subEvents->nevents;	
			addEvent(calEvents,subEvents->first);
			/*Account for addEvent adding one to nevents*/
			calEvents->nevents--; 
		}
		/*If no events were found in subtree, free struct*/
		free(subEvents);
	}

	/*Check if event*/
	if (!strcmp(comp->name,"VEVENT")){
		event = true;
		initEvent(&add);	
	}

	/*Add events found in this tree*/
	nextProp = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		/*If an event look for date and summary*/
		if (event){
			if (!strcmp(nextProp->name,"SUMMARY")){

				add->des = malloc(sizeof(char)
					*(strlen(nextProp->value)+1));
				assert(add->des);
				strcpy(add->des,nextProp->value);
				summary = true;
			}

			else if (!strcmp(nextProp->name,"DTSTART")){

				add->date = malloc(sizeof(char)
					*(strlen(nextProp->value)+1));
				assert(add->date);
				strcpy(add->date,nextProp->value);
			}
		}
		nextProp = nextProp->next;
	}
	if (event){
		/*If no summary add not available*/
		if (!summary){
			add->des = malloc(sizeof(char)*5);
			strcpy(add->des,"(na)");
			assert(add->des);
		}
		addEvent(calEvents,add); 
	}

	return calEvents;
}

static char *getCopy(char *string){
    char *copy = malloc(sizeof(char)
        *(strlen(string)+1));
    assert(copy);
    strcpy(copy,string);
    return copy;
}

static void initializeProp(CalProp *prop){
    prop->name = NULL;
    prop->value = NULL;
    prop->nparams = 0;
    prop->param = NULL;
    prop->next = NULL;
}

static CalParam *getParamsCopy(CalParam *param){
	CalParam *copy;

	if (!param){ return NULL; }

	copy = malloc(sizeof(CalParam)
		+(sizeof(CalParam*)*param->nvalues));
	assert(copy);
	copy->name = getCopy(param->name);
	copy->nvalues = param->nvalues;
	copy->next = NULL;

	/*Copy values*/
	for (int i=0;i<param->nvalues;i++){
		(copy->value)[i] = getCopy((param->value)[i]);
	}
	/*Get next recursively*/
	if (param->next){
		copy->next = getParamsCopy(param->next);
	}
	return copy;
}

static void freeCalParam(CalParam **param){
    int i;

    if (*param == NULL){
        return;
    }
    /*Free the name*/
    if ((*param)->name != NULL){
        free((*param)->name);       
    }
    /*Free flexible array*/
    for (i=0;i<(*param)->nvalues;i++){
        free(((*param)->value)[i]);
        ((*param)->value)[i] = NULL;

    }

    free(*param);
    *param = NULL;
}

static CalProp *getCalPropsCopy(CalProp *prop){
	CalProp *copy=NULL;

	/*Initialize*/
	copy = malloc(sizeof(CalProp));
	assert(copy);
	initializeProp(copy);

	/*Copy all members*/
	copy->name = getCopy(prop->name);
	copy->value = getCopy(prop->value);
	copy->nparams = prop->nparams;
	copy->param = getParamsCopy(prop->param);

	/*Get next recursively*/
	if (prop->next){
		copy->next = getCalPropsCopy(prop->next);
	}

	return copy;

}

static bool removeProp(CalProp **prop,char *name){
	CalProp **next=NULL,*prev,*temp=NULL;

	if (!(*prop)){
		return false;
	}

	/*Remove head*/
	if (!strcmp((*prop)->name,name)){
		freeCalParam(&(*prop)->param);
		free((*prop)->name);
		free((*prop)->value);

		/*If an element exists ahead in the list than make
		  it the new head since the head got removed*/
		if ((*prop)->next){
			temp = *prop;
			(*prop) = (*prop)->next;
			free(temp);
		}
		/*Free head*/
		else{
			free(*prop);
			*prop = NULL;
		}
		return true;
	}

	if (!(*prop)->next){
		printf("NULL!\n");
		exit(1);
	}
	next = prop;

	/*Traverse list looking for property with matching name to remove*/
	while ((*next)->next){
		prev = *next;
		next = &((*next)->next);
		if (!strcmp((*next)->name,name)){
			freeCalParam(&(*next)->param);
			free((*next)->name);
			free((*next)->value);

			/*If a value exists after, fix the next pointer for
			previous element in the list to the next item*/
			if ((*next)->next){
				prev->next = (*next)->next;
			}
			free(*next);
			*next = NULL;

			return true;
		}
	}

	return false;

}

CalComp *getCalCompCopy(const CalComp *comp){
	CalComp *copy=NULL,*subcopy=NULL;

	copy = malloc(sizeof(CalComp)
		+(sizeof(CalComp*)*comp->ncomps));
	assert(copy);

	/*Copy this tree's elements*/
	copy->name = getCopy(comp->name);
	copy->nprops = comp->nprops;
	copy->prop = getCalPropsCopy(comp->prop);
	copy->ncomps = comp->ncomps;

	/*Copy flexible array*/
	for (int i=0;i<comp->ncomps;i++){
		subcopy = getCalCompCopy((comp->comp)[i]);

		(copy->comp)[i] = subcopy;
	}

	return copy;
}

static bool getStrFromTo(char **arguments,char **from,char **to){
	int i=0;
	int fromPos=0,toPos=0;

	while (arguments[i]){

		/*Look for from and check if its specified*/
		if (!strcmp(arguments[i],"from")){
			fromPos = i;
			/*Next argument is the from date*/
			if (arguments[i+1]){

				/*Allocate memory and copy date*/
				*from = malloc(sizeof(char)
					*(strlen(arguments[i+1])+1));
				assert(*from);
				strcpy(*from,arguments[i+1]);
			}

			/*No from specificed, when found keyword from*/
			else{
				return false;
			}
		}

		/*Look for to and check if its specified*/
		if (!strcmp(arguments[i],"to")){
			toPos = i;

			/*Next argument is the to date*/
			if (arguments[i+1]){

				/*Allocate memory and copy date*/
				*to = malloc(sizeof(char)
					*(strlen(arguments[i+1])+1));
				assert(*to);
				strcpy(*to,arguments[i+1]);
			}

			/*No to specificed, when found keyword 'to'*/
			else{
				return false;
			}
		}
		i++;
	}

	/*From comes after to*/
	if (fromPos > toPos){ return false; }
	
	return true;
}

static int getFromTo(char *strfrom,char *strto
	,time_t *timefrom,time_t *timeto,char **errstr){

	struct tm *tmfrom,*tmto;
	int ret;

	/*Make sure both from and to are present*/
	if (!strfrom || !strto){
		*errstr = NULL;
		return 8;
	}

	/*Use todays date for from*/
	if (strfrom){
		if (!strcmp(strfrom,"today")){
			time(timefrom);
			tmfrom = localtime(timefrom);

			/*Change time to 00:00:00*/
			tmfrom->tm_sec = 0;
			tmfrom->tm_min = 0;
			tmfrom->tm_hour = 0;

			*timefrom = mktime(tmfrom);
		}

		/*Use inputed date*/
		else{

			/*Initialize time struct*/
			tmfrom = malloc(sizeof(struct tm));
			assert(tmfrom);
			tmto = malloc(sizeof(struct tm));
			assert(tmto);
			initializeTm(tmfrom);
			initializeTm(tmto);

			ret = getdate_r(strfrom,tmfrom);
			if (ret){
				*errstr = strfrom;
				return ret;
			}

			/*Change time to 00:00:00*/
			tmfrom->tm_sec = 0;
			tmfrom->tm_min = 0;
			tmfrom->tm_hour = 0;

			*timefrom = mktime(tmfrom);
			
			free(tmto);
			free(tmfrom);
		}
	}
	if (strto){

		/*Use todays date for to*/
		if (!strcmp(strto,"today")){
			time(timeto);
			tmto = localtime(timeto);

			/*Change time to 23:59:00*/
			tmto->tm_sec = 0;
			tmto->tm_min = 59;
			tmto->tm_hour = 23;

			*timeto = mktime(tmto);

			/*Check if 'to' comes before 'from'*/
			if (difftime(*timeto,*timefrom) < 0){
				fprintf(stderr,"Error, 'from' before 'to'\n");
				return 8;
			}

		}

		/*Use inputed date*/
		else{

			/*Initialize time struct*/
			tmfrom = malloc(sizeof(struct tm));
			assert(tmfrom);
			tmto = malloc(sizeof(struct tm));
			assert(tmto);
			initializeTm(tmfrom);
			initializeTm(tmto);
			
			ret = getdate_r(strto,tmto);
			if (ret){
				*errstr = strto;
				return ret;
			}

			/*Change time to 23:59:00*/
			tmto->tm_sec = 0;
			tmto->tm_min = 59;
			tmto->tm_hour = 23;

			*timeto = mktime(tmto);

			/*Check if 'to' comes before 'from'*/
			if (difftime(*timeto,*timefrom) < 0){
				printf("Error, 'from' before 'to'\n");
				return 8;
			}

			free(tmto);
			free(tmfrom);
		}
	}

	return 0;
}

static void printCmdSyntax(){
	fprintf(stderr,"---Invalid command or argument syntax.");
	fprintf(stderr," Command options are listed below---\n");
	fprintf(stderr,"(1) caltool -info\n");
	fprintf(stderr,"(2) caltool -extract kind('e' or 'x')\n");
	fprintf(stderr,"(3) caltool -filter content [from date ] [to date ]\n");
	fprintf(stderr,"(4) caltool -combine file2 \n");
}

int main(int argc,char **argv){
	char *strfrom=NULL,*strto=NULL,*errstr = NULL;
	int ret;
	CalComp *comp = NULL,*merge=NULL;
	CalStatus status;
	CalOpt kind;
	time_t from=0,to=0;
	FILE *file;

	/*Read cal comp from file*/
	status = readCalFile(stdin,&comp);
	if (status.code != OK){
		fprintf(stderr,"Error: ics contents formatted incorrectly\n");
		return EXIT_FAILURE;
	}

	/*Get info*/
	if ((argc == 2) && !strcmp(argv[1],"-info")){

		status = calInfo(comp,status.lineto,stdout);
		if (status.code == IOERR){
			fprintf(stderr,"Error found: IOERR\tline: %d\n"
				,status.lineto);
		}
	}
	/*Extract*/
	else if ((argc == 3) && !strcmp(argv[1],"-extract")
		&& (argv[2][0] == 'e' || argv[2][0] == 'x')){

		/*Get the kind of extraction requested*/
		if (argv[2][0] == 'e'){
			kind = OEVENT;
		}
		else if (argv[2][0] == 'x'){
			kind = OPROP;
		}

		/*Extract the information*/
		status = calExtract(comp,kind,stdout);
		if (status.code == IOERR){
			fprintf(stderr,"Error found: IOERR\tline: %d\n"
				,status.lineto);
			return EXIT_FAILURE;
		}
	}
	/*Filter*/
	else if (argc > 2 && !strcmp(argv[1],"-filter") 
		&& !strcmp(argv[1],"-filter")){
		
		/*Events*/
		if(argv[2][0] == 'e'){
			kind = OEVENT;

			/*Check for date range*/
			if (argc > 3){

				/*Get from and to in string form, check for
				input error*/
				if (!getStrFromTo(argv,&strfrom,&strto)){
					printCmdSyntax();
					return EXIT_FAILURE;
				}

				ret = getFromTo(strfrom,strto,&from,&to,&errstr);
				if (ret >= 1 && ret <= 5){
					printf("Problem with DATEMSK environment variable or template file\n");
					return EXIT_FAILURE;
				}
				else if (ret == 7 || ret == 8){
					printf("Date '%s' could not be interpreted\n"
						,errstr);
					return EXIT_FAILURE;
				}
				assert(ret != 6);

				/*Free string version of dates*/
				if (strfrom){ free(strfrom); }
				if (strto){ free(strto); }

				/*Make sure from and to are present
				and check that they have the right format*/
				if ((long int)to && (long int)from){
					if (difftime(to,from) < 0){
						printf("Error: to date is less than from date\n");
						exit(EXIT_FAILURE);
					}
				}
			}

			/*Filter the ics*/
			status = calFilter(comp,kind,from,to,stdout);
			if (status.code != OK){
				if (status.code == IOERR){
					fprintf(stderr,"Error found: IOERR\tline: %d\n"
						,status.lineto);
					exit(EXIT_FAILURE);
				}
				else if (status.code == NOCAL){
					fprintf(stderr,"Error found: NOCAL\tline: %d\n"
						,status.lineto);
					exit(EXIT_FAILURE);
				}
			}
		}
		/*To-do items*/
		else if(argv[2][0] == 't'){
			kind = OTODO;

			/*Check for date range*/
			if (argc > 3){

				/*Get from and to in string form, check for
				input error*/
				if(!getStrFromTo(argv,&strfrom,&strto)){
					printCmdSyntax();
					return EXIT_FAILURE;	
				}
				
				ret = getFromTo(strfrom,strto,&from,&to,&errstr);
				if (ret >= 1 && ret <= 5){
					fprintf(stderr,"Problem with DATEMSK environment variable or template file\n");
					exit(EXIT_FAILURE);
				}
				else if (ret == 7 || ret == 8){
					fprintf(stderr,"Date '%s' could not be interpreted\n"
						,errstr);
					exit(EXIT_FAILURE);
				}
				assert(ret != 6);

				/*Free string version of dates*/
				if (strfrom){ free(strfrom); }
				if (strto){ free(strto); }
				
				/*Make sure from and to are present
				and check that they have the right format*/
				if ((long int)to && (long int)from){
					if (difftime(to,from) < 0){
						printf("Error: to date is less than from date\n");
						exit(EXIT_FAILURE);
					}
				}
			}

			/*Filter the ics*/
			status = calFilter(comp,kind,from,to,stdout);
			if (status.code != OK){
				if (status.code == IOERR){
					fprintf(stderr,"Error found: IOERR\tline: %d\n"
						,status.lineto);
					exit(EXIT_FAILURE);
				}
				else if (status.code == NOCAL){
					fprintf(stderr,"Error found: NOCAL\tline: %d\n"
						,status.lineto);
					exit(EXIT_FAILURE);
				}
			}
		}
		else{
			printCmdSyntax();
			return EXIT_FAILURE;
		}
	}
	/*Combine*/
	else if ((argc == 3) && !strcmp(argv[1],"-combine")){
		file = fopen(argv[2],"r");
		/*Couldn't open file so print error message*/
		if (!file){
			fprintf(stderr,"Error: could not open file for comibing.\n");
			return EXIT_FAILURE;
		}
		
		/*Read cal comp from file*/
		status = readCalFile(file,&merge);
		if (status.code != OK){
			fprintf(stderr,"Error: ics contents formatted incorrectly\n");
			return EXIT_FAILURE;
		}

		/*Combine files*/
		status = calCombine(comp,merge,stdout);
		if (status.code == IOERR){
			fprintf(stderr,"Error found: IOERR\tline: %d\n"
				,status.lineto);
		}

		freeCalComp(merge);
		fclose(file);
	}
	else{
		printCmdSyntax();
		return EXIT_FAILURE;
	}

	freeCalComp(comp);

	return EXIT_SUCCESS;
}

CalStatus calInfo( const CalComp *comp
	, int lines, FILE *const txtfile ){

	int compNum=0,eveNum=0,todoNum=0,otherNum=0;
	int subCompNum=0,propNum=0,ret,cnCount=0;
	bool dateFound = false;
	struct tm from,to;
	char **cn=NULL;
	CalStatus calStatus,calledStatus;

	/*Initialize CalStatus*/
	calStatus.code = OK;
	calStatus.linefrom = calStatus.lineto = 0;

	/*Output number of lines*/
	if (lines == 1){
		ret = fprintf(txtfile,"1 line\n");
	}
	else{
		ret = fprintf(txtfile,"%d lines\n",lines);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}
	calStatus.lineto++;
	calStatus.linefrom++;

	/*Get the number of components,events,todo,other*/
	getCompNum(comp,&compNum,&eveNum,&todoNum,&otherNum);

	/*Print number of components*/
	calledStatus = printCompNum(txtfile,compNum,eveNum,todoNum,otherNum);
	
	/*Check for error during printCompNum()*/
	if (calledStatus.code == IOERR){
		calStatus.code = calledStatus.code;
		return calStatus; 	
	}	

	calStatus.lineto ++;
	calStatus.linefrom ++;
	
	/*Print number of subcomponents*/
	subCompNum = getSubCompNum(comp);

	if (subCompNum == 1){
		ret = fprintf(txtfile,"1 subcomponent\n");
	}
	else{
		ret = fprintf(txtfile,"%d subcomponents\n"
			,subCompNum);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}

	calStatus.linefrom++;
	calStatus.lineto++;

	/*Print number of properties*/
	propNum = getPropNum(comp);
	if (propNum == 1){
		ret = fprintf(txtfile,"1 property\n");
	}
	else{
		ret = fprintf(txtfile,"%d properties\n"
			,propNum);
	}
	if (ret < 0){
		calStatus.code = IOERR;
		return calStatus;
	}

	calStatus.linefrom++;
	calStatus.lineto++;


	/*Initialize time structs*/
	initializeTmMax(&from);
	initializeTmMin(&to);

	/*Get range of dates*/
	dateFound = getDateRange(comp,&from,&to);

	if (!dateFound){

		if (fprintf(txtfile,"No dates\n") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}
		calStatus.linefrom++;
		calStatus.lineto++;
	}
	else{

		/*Print from*/
		if (fprintf(txtfile,"From ") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}

		calStatus.code = printInfoDate(from,txtfile);
		if (calStatus.code == IOERR){
			return calStatus;
		}

		if (fprintf(txtfile," to ") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}

		/*Print to*/
		calStatus.code = printInfoDate(to,txtfile);
		if (calStatus.code == IOERR){
			return calStatus;
		}

		if (fprintf(txtfile,"\n") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}

		/*Line printed successfully so increment linecount*/
		calStatus.linefrom++;
		calStatus.lineto++;
	}

	/*Get all common names*/
	cn = getCN(comp,&cnCount);

	/*Remove duplicates*/
	removeDups(cn,&cnCount);

	/*Sort common names*/
	qsort(cn, cnCount, sizeof(char*)
		,compareStr);

	/*Print organizers*/
	if (cnCount == 0){
		if (fprintf(txtfile,"No organizers\n") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}
		calStatus.linefrom++;
		calStatus.lineto++;
	}
	else{
		if (fprintf(txtfile,"Organizers:\n") < 0){
			calStatus.code = IOERR;
			return calStatus;
		}
		calStatus.linefrom++;
		calStatus.lineto++;

		for (int i=0;i<cnCount;i++){
			if (fprintf(txtfile,"%s\n",cn[i]) < 0){
				calStatus.code = IOERR;
				return calStatus;
			}
			calStatus.linefrom++;
			calStatus.lineto++;
		}
	}

	/*Free memory allocated for common names*/
	for (int i=0;i<cnCount;i++){
		free(cn[i]);
	}
	if (cn){
		free(cn);
	}

	return calStatus;
}

CalStatus calExtract( const CalComp *comp, CalOpt kind
	, FILE *const txtfile ){

	int *printed;
	char **strXProps=NULL,**des;
	CalStatus calStatus;
	CalEventHead *events=NULL;
	CalEvent *nextEvent=NULL;
	XProp *nextXProp=NULL;
	XPropHead *xprops=NULL;
	struct tm *dates=NULL,*oldDates=NULL;
	CalError error;

	/*Initialize CalStatus*/
	calStatus.code = OK;
	calStatus.linefrom = 0;
	calStatus.lineto = 0;

	/*Get events*/
	if (kind == OEVENT){
		events = getEvents(comp);

		/*Allocate memory*/
		oldDates = malloc(sizeof(struct tm)*events->nevents);
		assert(oldDates);
		dates = malloc(sizeof(struct tm)*events->nevents);
		assert(dates);
		des = malloc(sizeof(char*)*events->nevents);
		assert(des);

		/*Store dates in 2D array*/
		nextEvent = events->first;
		for (int i=0;i<events->nevents;i++){

			dates[i] = dateToTm(nextEvent->date);
			/*Save an array that will not be rearraged*/
			oldDates[i] = dates[i];

			/*Save descriptions for printing purposes*/
			des[i] = malloc(sizeof(char)*
				(strlen(nextEvent->des)+1));
			assert(des[i]);
			strcpy(des[i],nextEvent->des);

			nextEvent = nextEvent->next;
		}		

		/*Sort events by date*/
		qsort(dates,events->nevents,sizeof(struct tm)
			,compareDate);

		if (events->nevents > 0){
			printed = malloc(sizeof(int) 
				*events->nevents);
			assert(printed);
		}
		for (int i=0;i<events->nevents;i++){
			printed[i] = 0;
		}

		/*Print events in order*/
		for (int i=0;i<events->nevents;i++){
			for (int j=0;j<events->nevents;j++){

				/*Print event and date*/
				if ( mktime(&(dates[i])) == mktime(&(oldDates[j])) ){

					/*Check if was already printed*/
					if (printed[j]){
						continue;
					}

					error = printDate(dates[i],txtfile);
					if (error == IOERR){
						calStatus.code = IOERR;
						return calStatus;
					}

					if (fprintf(txtfile,": %s\n",des[j]) < 0){
						calStatus.code = IOERR;
						return calStatus;
					}

					/*Keep track of what was printed*/
					printed[j] = 1;

					calStatus.lineto++;
					calStatus.linefrom++;
				}
			}
		}

		if (events->nevents > 0){
			free(printed);
		}

		/*Free dates and events linked list*/
		for (int i=0;i<events->nevents;i++){
			free(des[i]);
		}
		free(des);
		free(oldDates);
		free(dates);
		if (events && events->first){
			freeCalEvents(events->first);
		}
		free(events);


	}
	/*Get X-PROPS*/
	else if (kind == OPROP){
		xprops = getXProps(comp);

		/*Put xprops in 2D array*/
		strXProps = malloc(sizeof(char*)*xprops->count);
		assert(strXProps);
		nextXProp = xprops->first;
		for (int i=0;i<xprops->count;i++){

			/*Store name*/
			strXProps[i] = malloc(sizeof(char)
				*(strlen(nextXProp->name)+1));
			assert(strXProps[i]);
			strcpy(strXProps[i],nextXProp->name);
			nextXProp = nextXProp->next;
		}

		/*Remove duplicates*/
		removeDups(strXProps,&(xprops->count));

		/*Sort xprops alphabetically and print*/
		qsort(strXProps,xprops->count,sizeof(char*)
			,compareStr);

		for (int i=0;i<xprops->count;i++){
			if (fprintf(txtfile,"%s\n",strXProps[i]) < 0){
				calStatus.code = IOERR;
				return calStatus;
			}
			calStatus.lineto++;
			calStatus.linefrom++;
		}

		/*Free memory in linked list and array*/
		for (int i=0;i<xprops->count;i++){
			free(strXProps[i]);
		}
		free(strXProps);
		if (xprops && xprops->first){
			freeXProps(xprops->first);
		}
		free(xprops);
	}


	return calStatus;
}

CalStatus calFilter( const CalComp *comp, CalOpt content
	, time_t datefrom, time_t dateto, FILE *const icsfile ){

	char filter[20];
	bool usefrom=false,useto=false;
	int del=0;
	struct tm subdate;
	CalComp *copy=NULL;
	CalStatus calStatus;

	/*Initialize subfrom subto*/
	initializeTm(&subdate);

	/*Initialize CalStatus*/
	calStatus.code = OK;
	calStatus.linefrom = calStatus.lineto = 0;

	/*Check if using date range*/
	useto = !(dateto == 0);
	usefrom = !(datefrom == 0);

	/*Check filter type*/
	if (content == OTODO){
		strcpy(filter,"VTODO");
	}
	else if (content == OEVENT){
		strcpy(filter,"VEVENT");
	}

	copy = getCalCompCopy(comp);

	/*Remove every componet not in the date range*/
	for (int i=0;i<copy->ncomps;i++){
		subdate = getCompDate((copy->comp)[i]);

		if (useto && usefrom){

			/*Remove anything not in from to range*/
			if (difftime(datefrom,mktime(&subdate)) > 0
				|| difftime(dateto,mktime(&subdate)) < 0){

				/*Remove flexible array member*/
				freeCalComp((copy->comp)[i]);
				(copy->comp)[i] = NULL;
				del++;
			}
		}
	}

	/*Remove anything that does not match the filter
	in the components */
	for (int i=0;i<copy->ncomps;i++){

		/*Skip comp if already removed*/
		if (!((copy->comp)[i])){
			continue;
		}
		if (strcmp(((copy->comp)[i])->name,filter)){
			/*Remove flexible array member*/
			freeCalComp((copy->comp)[i]);
			(copy->comp)[i] = NULL;
			del++;
		}
	}

	/*get rid off NULL members in flexible array*/
	for (int i=0;i<copy->ncomps;i++){
		if ((copy->comp)[i]){
			int j=1;

			/*make sure index not less than 0*/
			if ((i-j) < 0){
				continue;
			}

			/*Shift member left until member to left is not null
			or until its the first member*/
			while (!((copy->comp)[i-j])){
				(copy->comp)[i-j] = (copy->comp)[i-(j-1)];
				(copy->comp)[i-(j-1)] = NULL;
				j++;
				if ((i-j) < 0){
					break;
				} 
			}
		}
	}
	
	/*Correct number of components after deletion*/
	copy->ncomps -= del;

	if (del > 0){

		/*Free memory in flexible array*/
		copy = realloc(copy,sizeof(CalComp)
			+(sizeof(CalComp*)*(copy->ncomps)));	
		assert(copy);
	}

	/*Check for NOCAL*/
	if (copy->ncomps == 0){
		calStatus.code = NOCAL;
		return calStatus;
	}

	calStatus = writeCalComp(icsfile,copy);

	freeCalComp(copy);

	return calStatus;
}

CalStatus calCombine( const CalComp *comp1
	, const CalComp *comp2, FILE *const icsfile ){
	 
	CalStatus calStatus;
	CalComp *copy,*flexCopy;
	CalProp *propcopy;
	int propSub=0;

	/*Initialize CalStatus*/
	calStatus.code = OK;
	calStatus.lineto = calStatus.linefrom = 0;

	#ifdef _DEBUG_COMB
	printf("-------COMP1--------\n");
	writeCalComp(stdout,comp1);
	printf("---------------------\n");
	printf("-------COMP2--------\n");
	writeCalComp(stdout,comp2);
	printf("---------------------\n");
	#endif

	/*Get copy of comp1*/
	copy = getCalCompCopy(comp1);

	/*Allocate size for comp2's flexible array*/
	copy = realloc(copy,sizeof(CalComp)
		+(sizeof(CalComp*)*(copy->ncomps+comp2->ncomps)));
	assert(copy);

	/*Copy comp2's flexible array into comp1*/
	for (int i=0;i<comp2->ncomps;i++){
		flexCopy = getCalCompCopy((comp2->comp)[i]);

		#ifdef _DEBUG_COMB
		printf("-----FLEX COPY------\n");
		writeCalComp(stdout,flexCopy);
		printf("---------------------\n");
		#endif

		(copy->comp)[copy->ncomps+i] = flexCopy;
	}
	copy->ncomps += comp2->ncomps;

	/*Get copy of properties*/
	propcopy = getCalPropsCopy(comp2->prop);

	/*Remove prodid and version since they already exist*/
	if (removeProp(&propcopy,"PRODID")){
		propSub++;
	}

	if (removeProp(&propcopy,"VERSION")){
		propSub++;
	}

	/*Increment property num, but subtract prodid and version
	if they were present*/
	copy->nprops += (comp2->nprops-propSub);

	/*Link properties together*/
	addCalProp(copy->prop,propcopy);

	#ifdef _DEBUG_COMB
	printf("--------FULL COPY----------\n");
	#endif
	calStatus = writeCalComp(icsfile,copy);

	freeCalComp(copy);

	return calStatus;
}