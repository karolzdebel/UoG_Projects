/********
calmodule.c -- Python extensions for calutil and caltool functions
Last updated:  1:00 PM Match-18-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********/

#include "caltool.h"
#include <Python.h>
#include "calutil.h"

CalComp *getCalCompCopy(const CalComp *comp);
PyMODINIT_FUNC PyInit_Cal(void);
static PyObject *Cal_writeFile( PyObject *self, PyObject *args );
static PyObject *Cal_freeFile( PyObject *self, PyObject *args );
static PyObject *Cal_readFile( PyObject *self, PyObject *args );
static PyObject *Cal_getData( PyObject *self, PyObject *args );
static PyObject *getCompTuple(CalComp *comp);
static char *getSummary(CalProp *prop);

static PyMethodDef CalMethods[] = {
    {"readFile", Cal_readFile, METH_VARARGS},
    {"writeFile", Cal_writeFile, METH_VARARGS},
    {"freeFile", Cal_freeFile, METH_VARARGS},
    {"getData", Cal_getData, METH_VARARGS},
    {NULL, NULL} 
};

static struct PyModuleDef calModuleDef = {
    PyModuleDef_HEAD_INIT,
    "Cal",     //enable "import Cal"
    NULL,      //omit module documentation
    -1,        //don't reinitialize the module
    CalMethods //link module name "Cal" to methods table 
};

static PyObject *getCompTuple(CalComp *comp){
    Py_ssize_t len = 4;
    PyObject *tuple = PyTuple_New(len);
    PyObject *data;

    data = Py_BuildValue("s",comp->name);
    PyTuple_SET_ITEM(tuple, 0, data);

    data = Py_BuildValue("i",comp->nprops);
    PyTuple_SET_ITEM(tuple, 1, data);

    data = Py_BuildValue("i",comp->ncomps);
    PyTuple_SET_ITEM(tuple, 2, data);

    data = Py_BuildValue("s",getSummary(comp->prop));
    PyTuple_SET_ITEM(tuple, 3, data);

    return tuple;
}

static char *getSummary(CalProp *prop){
    if (!prop){
        return NULL;
    }
    if (!strcmp(prop->name,"SUMMARY")){
        return prop->value;
    }
    if (prop->next){
        return getSummary(prop->next);
    }
    return "";
}

PyMODINIT_FUNC PyInit_Cal(void) {
    return PyModule_Create( &calModuleDef );
} 

static PyObject *getOrganizerName(CalComp *comp){
	CalParam *param;
	CalProp *prop;
	PyObject *name;

	prop = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		if (!strcmp(prop->name,"ORGANIZER")){

			/*Get name*/		
			param = prop->param;
			for (int i=0;i<prop->nparams;i++){
				if (!strcmp(param->name,"CN")){
					name = Py_BuildValue("s",param->value[0]);
					return name;
				}
				param = param->next;
			}
		}
		prop = prop->next;
	}
	return NULL;
}

static PyObject *getOrganizerData(CalComp *comp){
	CalParam *param;
	CalProp *prop;
	PyObject *name, *contact;
	PyObject *data = NULL;

	prop = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		if (!strcmp(prop->name,"ORGANIZER")){

			/*Get name*/		
			param = prop->param;
			for (int i=0;i<prop->nparams;i++){
				if (!strcmp(param->name,"CN")){
					name = Py_BuildValue("s",param->value[0]);
				}
				param = param->next;
			}

			/*Get contact*/
			contact = Py_BuildValue("s",prop->value);

			data = PyList_New(0);
			PyList_Append(data,name);
			PyList_Append(data,contact);
		}
		prop = prop->next;
	}
	return data;
}

static PyObject *getTodoData(CalComp *comp){
	CalProp *prop;
	PyObject *summary,*priority,*organizer;
	PyObject *data;
	
	/*Summary,priority, organizer*/
	organizer = getOrganizerName(comp);

	prop = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		
		if (!strcmp(prop->name,"SUMMARY")){
			summary = Py_BuildValue("s",prop->value);
		}
		else if (!strcmp(prop->name,"PRIORITY")){
			priority = Py_BuildValue("s",prop->value);
		}

		prop = prop->next;
	}

	data = PyList_New(0);
	PyList_Append(data,summary);
	PyList_Append(data,priority);
	PyList_Append(data,organizer);

	return data;
}

static PyObject *getEventData(CalComp *comp){
	CalProp *prop;
	PyObject *summary=NULL,*start=NULL;
	PyObject *location=NULL,*organizer=NULL;
	PyObject *data=NULL;
	
	/*Summary,start,location,organizer*/
	organizer = getOrganizerName(comp);

	prop = comp->prop;
	for (int i=0;i<comp->nprops;i++){
		
		if (!strcmp(prop->name,"SUMMARY")){
			summary = Py_BuildValue("s",prop->value);
		}
		else if (!strcmp(prop->name,"START")){
			start = Py_BuildValue("s",prop->value);
		}
		else if (!strcmp(prop->name,"LOCATION")){
			location = Py_BuildValue("s",prop->value);
		}
		else if (!strcmp(prop->name,"DTSTART")){
			start = Py_BuildValue("s",prop->value);
		}

		prop = prop->next;
	}

	data = PyList_New(0);
	PyList_Append(data,summary);
	PyList_Append(data,start);
	PyList_Append(data,location);
	PyList_Append(data,organizer);

	return data;
}

static CalComp *getNewComp(PyObject *complist,CalComp *pcal){
   int index;
   CalComp *copy;
   PyObject *comp;

    /*Get size of complist*/
    int size = PyList_Size(complist);

    /*Create and initialize array*/   
    int listed[pcal->ncomps];
    for (int i=0;i<pcal->ncomps;i++){
        listed[i] = 0;
    }

    for (int j=0;j<size;j++){

        /*Store tuple information in variables*/
        comp = PyList_GetItem(complist,j);
        PyArg_ParseTuple(comp,"i"
            ,&index);
        listed[index-1] = 1;
    }

    /*Create copy of original comp*/
    copy = getCalCompCopy(pcal);

    /*Remove unlisted components*/
    int del=0;
    for (int i=0;i<copy->ncomps;i++){
        if (listed[i] == 0){
            freeCalComp(copy->comp[i]);
            copy->comp[i] = NULL;
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

    /*Correct umber of components after deletion*/
    copy->ncomps -= del;

    return copy;
}

static PyObject *Cal_getData( PyObject *self, PyObject *args ){
	CalComp *pcal,*comp;
	PyObject *pyErrorMsg=NULL;
	PyObject *results;
	PyObject *eventData=NULL;
	PyObject *todoData=NULL;
	PyObject *events=NULL;
	PyObject *todos=NULL;
	PyObject *organizers=NULL;
	PyObject *complist=NULL;
	PyObject *organizerData=NULL;

	PyArg_ParseTuple(args, "kOO"
		,(unsigned long*)&pcal, &complist, &results);

	comp = getNewComp(complist,pcal);

	events = PyList_New(0);
	todos = PyList_New(0);
	organizers = PyList_New(0);


	for (int i=0;i<comp->ncomps;i++){
		
		/*Get all event data*/
		if (!strcmp(comp->comp[i]->name,"VEVENT")){
			
			/*Get event data and add to results*/
			eventData = getEventData(comp->comp[i]);
			
			if (PyList_Append(events, eventData) == -1){
        		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
    			return pyErrorMsg;
			}
		}

		/*Get all todo data*/
		else if (!strcmp(comp->comp[i]->name,"VTODO")){

			/*Get todo data and add to results*/
			todoData = getTodoData(comp->comp[i]);
			
			if (PyList_Append(todos, todoData) == -1){
        		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
    			return pyErrorMsg;
    		}
		}

		/*Get all organizer data*/
		organizerData = getOrganizerData(comp->comp[i]);
		if (organizerData){
			if (PyList_Append(organizers, organizerData) == -1){
        		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
    			return pyErrorMsg;
    		}
		}

	}

	/*Add todos, events and organizers to result*/
	if (PyList_Append(results, organizers) == -1){
		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
		return pyErrorMsg;
	}
	if (PyList_Append(results, events) == -1){
		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
		return pyErrorMsg;
	}
	if (PyList_Append(results, todos) == -1){
		pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
		return pyErrorMsg;
	}

	pyErrorMsg = Py_BuildValue("s","OK\n");
	return pyErrorMsg;

}

static PyObject *Cal_writeFile( PyObject *self, PyObject *args ){
    char *name;
    char *errorMsg = NULL;
    int index;
    CalComp *pcal,*copy;
    PyObject *complist;
    PyObject *pyErrorMsg;
    PyObject *comp;
    Py_ssize_t size;
    FILE *ics;

    /*Store argument data inside variables*/
    PyArg_ParseTuple( args, "skO", &name
        , (unsigned long*)&pcal, &complist );

    ics = fopen(name,"w+");

    /*Make sure file opened successfully*/  
    if (!ics){
        errorMsg = strerror(errno);
        pyErrorMsg = Py_BuildValue("s",errorMsg);
        fclose(ics);
        return pyErrorMsg;
    }

    errorMsg = malloc(sizeof(MSG_LEN));
    strcpy(errorMsg,"OK");
    pyErrorMsg = Py_BuildValue("s",errorMsg);

    /*Get size of complist*/
    size = PyList_Size(complist);

    /*Look for particular comp and write it*/
    if (size == 1){

        /*Store tuple information in variables*/
        comp = PyList_GetItem(complist,0);
        PyArg_ParseTuple(comp,"i",&index);

        writeCalComp(ics,pcal->comp[index-1]);

        fclose(ics);
        return pyErrorMsg;
    }

    /*Create and initialize array*/
    int listed[pcal->ncomps];
    for (int i=0;i<pcal->ncomps;i++){
        listed[i] = 0;
    }

    for (int j=0;j<size;j++){

        /*Store tuple information in variables*/
        comp = PyList_GetItem(complist,j);
        PyArg_ParseTuple(comp,"i"
            ,&index);
        listed[index-1] = 1;
    }

    /*Create copy of original comp*/
    copy = getCalCompCopy(pcal);

    /*Remove unlisted components*/
    int del=0;
    for (int i=0;i<copy->ncomps;i++){
        if (listed[i] == 0){
            freeCalComp(copy->comp[i]);
            copy->comp[i] = NULL;
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

    /*Correct umber of components after deletion*/
    copy->ncomps -= del;

    writeCalComp(ics,copy);

    freeCalComp(copy);

    fclose(ics);
    return pyErrorMsg;
}

static PyObject *Cal_freeFile( PyObject *self, PyObject *args ){
    CalComp *pcal;
    PyObject *err;
    
    if (!PyArg_ParseTuple( args, "k", (unsigned long*)&pcal )){
        err = Py_BuildValue("s","Error: incorrect calenar pointer\n");
    }
    else{
        err = Py_BuildValue("s","OK");
    }

    freeCalComp(pcal);

    return err;    
}

static PyObject *Cal_readFile( PyObject *self, PyObject *args ){
    char *name,*errString;
    char *errorMsg;
    CalComp *comp;
    CalStatus status;
    PyObject *tuple;
    PyObject *result;
    PyObject *pyErrorMsg;
    PyObject *compData;
    PyObject *compList = PyList_New(0);
    FILE *ics;

    PyArg_ParseTuple( args, "sO", &name, &result );

    ics = fopen(name,"r");
    /*Make sure file opened successfully*/  
    if (!ics){
        errorMsg = strerror(errno);
        pyErrorMsg = Py_BuildValue("s",errorMsg);
        fclose(ics);
        return pyErrorMsg;
    }

    /*Get CalComp*/
    status = readCalFile(ics,&comp);

    /*Return error*/
    errorMsg = malloc(sizeof(char)*MSG_LEN);
    errorMsg[0] = '\0';
    switch(status.code){
        case OK:
            strcpy(errorMsg,"OK");
            break; 
        case AFTEND:
            strcpy(errorMsg,"Formatting Error: text found after end of calendar.\n");
            break;
        case BEGEND:
            strcpy(errorMsg,"Formatting Error: calendar version missing or incorrect.\n");
            break;
        case IOERR:
            strcpy(errorMsg,"Formatting Error: BEGIN and END not found as expected.\n"); 
            break;
        case NOCAL:
            strcpy(errorMsg,"Formatting Error: outer block not VCALENDAR or no V components found.\n"); 
            break;
        case NOCRNL:
            errString = malloc(sizeof(char)*MSG_LEN);
            sprintf(errString,"Formatting Error: CRNL missing at end of line. lines:%d to: %d\n"
                ,status.linefrom,status.lineto);
            strcpy(errorMsg,errString); 
            break; 
        case NODATA:
            strcpy(errorMsg,"Formatting Error: no data found between BEGIN and END.\n"); 
            break;
        case NOPROD:
            strcpy(errorMsg,"Formatting Error: PRODID missing.\n"); 
            break;
        case SUBCOM:
            strcpy(errorMsg,"Formatting Error: unallowed subcomponent found.\n");
            break; 
        case SYNTAX:
            errString = malloc(sizeof(char)*MSG_LEN);
            sprintf(errString,"Formatting Error: CRNL missing at end of line. lines:%d to: %d\n"
                ,status.linefrom,status.lineto);
            strcpy(errorMsg,errString); 
            break;  
        default:
            strcpy(errorMsg,"Unrecognized Error\n");
            break;
    }
    /*Store error message*/
    pyErrorMsg = Py_BuildValue("s",errorMsg);

    /*Don't proceed if error was returned*/
    if (status.code != OK){
        fclose(ics);
        return pyErrorMsg;
    }

    /*Store Calendar pointer in result list*/
    compData = Py_BuildValue("k",comp);
    if (PyList_Append(result, compData) == -1){
        pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
    }

    /*Create a tuple for every member of flexible array*/
    for (int i=0;i<comp->ncomps;i++){
        tuple = getCompTuple(comp->comp[i]);
        /*Add tuple to list*/
        if (PyList_Append(compList, tuple) == -1){
            pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
        }
    }

    /*Add list of tuples to result*/
    if (PyList_Append(result, compList) == -1){
        pyErrorMsg = Py_BuildValue("s","Error appending to list\n");
    }

    fclose(ics);
    return pyErrorMsg;
}
