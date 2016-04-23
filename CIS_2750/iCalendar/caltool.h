/********
caltool.h -- Public interface for iCalendar tools in caltool.c
Last updated:  1:00 PM March-18-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********/

#ifndef CALTOOL_H
#define CALTOOL_H A2_RevA

#ifndef _GNU_SOURCE
#define _GNU_SOURCE     // for getdate_r
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE   // for strptime
#endif
#define _MAX_DATE_LEN 100   // date length
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include "calutil.h"

/* Symbols used to send options to command execution modules */

typedef enum {
    OEVENT,     // events
    OPROP,      // properties
    OTODO,      // to-do items
} CalOpt;

typedef struct XProp XProp;
typedef struct XProp {
	char *name;
	XProp *next;
} XProp;

typedef struct XPropHead XPropHead;
typedef struct XPropHead {
	int count;
	XProp *first;
} XPropHead;

typedef struct CalEvent CalEvent;
typedef struct CalEvent {
    char *des;		//Description of event
    char *date;		//Date of event
    CalEvent *next; //Next event in linked list
} CalEvent;

typedef struct CalEventHead CalEventHead;
typedef struct CalEventHead {
    int nevents;	//Number of events
    CalEvent *first; //Next event in linked list
} CalEventHead;

/* iCalendar tool functions */

CalStatus calInfo( const CalComp *comp
    , int lines, FILE *const txtfile );
CalStatus calExtract( const CalComp *comp
    , CalOpt kind, FILE *const txtfile );
CalStatus calFilter( const CalComp *comp, CalOpt content
    , time_t datefrom, time_t dateto, FILE *const icsfile );
CalStatus calCombine( const CalComp *comp1
    , const CalComp *comp2, FILE *const icsfile );

#endif
