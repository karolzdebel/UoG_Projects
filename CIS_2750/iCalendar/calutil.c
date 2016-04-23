/********
calutil.c -- iCalendar utility functions and implementations of calutil.h
Last updated:  1:00 PM March-18-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********/
#include "calutil.h"

/*******************HELPER FUNCTION PROTOTYPES************************/
/*********************************************************************
 * FUNCTION NAME: calCheck
 * PURPOSE: Checks the name of all the members of the comp
 *          array for atleast one name that starts with 'V'.
 * ARGUMENTS: . CalComp to be checked.
 * RETURNS: CalError based on if a 'V' was found.
 *********************************************************************/
static CalError calCheck(CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: getCopy
 * PURPOSE: Gets copy of a given string.
 * ARGUMENTS: . String to be copied (char * string).
 * RETURNS: Copy of a string.
 *********************************************************************/
static char *getCopy(char *string);
/*********************************************************************
 * FUNCTION NAME: isEOF
 * PURPOSE: Check is a string only contains an EOF.
 * ARGUMENTS: . The string to be checked for EOF (char * line).
 * RETURNS: Boolean value based on whether only EOF was found.
 *********************************************************************/
static bool isEOF(char *line);
/*********************************************************************
 * FUNCTION NAME: endsEOF
 * PURPOSE: Checks for EOF at the end of a string.
 * ARGUMENTS: . The string to be checked for EOF present (char * line).
 * RETURNS: Boolean value based on whether EOF was found.
 *********************************************************************/
static bool endsEOF(char *line);
/*********************************************************************
 * FUNCTION NAME: setCompName
 * PURPOSE: Copies a string into the comp name.
 * ARGUMENTS: . CalComp which will have its name changed (CalComp* comp).
 *            . Name that will be copied into the CalComp. (char *name).
 *********************************************************************/
static void setCompName(CalComp *comp, char *name);
/*********************************************************************
 * FUNCTION NAME: addCalProp
 * PURPOSE: Adds a CalProp to the linked list located
 *          inside comp.
 * ARGUMENTS: . CalProp to be inserted (CalProp *insert).
 *            . CalComp that is being inserted into (CalComp *comp).
 *********************************************************************/
static void addCalProp(CalProp *insert,CalComp *comp);
/*********************************************************************
 * FUNCTION NAME: stringToUpper
 * PURPOSE: Changes every alphebetic character of the string to uppercase.
 * ARGUMENTS: . String to be uppercased (char *string).
 *********************************************************************/
static void stringToUpper(char *string);
/*********************************************************************
 * FUNCTION NAME: freeCalParam
 * PURPOSE: Free's all memory allocated to a CalParam and it's contents.
 * ARGUMENTS: . CalParam to be free'd (CalParam **param).
 *********************************************************************/
static void freeCalParam(CalParam **param);
/*********************************************************************
 * FUNCTION NAME: freeCalProp
 * PURPOSE: Frees all memory assigned to a CalProp and it's contents.
 * ARGUMENTS: . CalProp to be free'd (CalProp **prop).
 *********************************************************************/
static void freeCalProp(CalProp **prop);
/*********************************************************************
 * FUNCTION NAME: storeValues
 * PURPOSE: Expands an arrays memory and stores a value at a position.
 * ARGUMENTS: . Current size of the array that dictates which
 *              position the value should be input to (int curSize).
 *            . Value to be input to the array (int value).
 *            . Array in which the value will be stored. (int **array).
 *********************************************************************/
static void storeValue(int curSize,int value,int **array);
  /*********************************************************************
 * FUNCTION NAME: syntaxCheck
 * PURPOSE: Checks for correct syntax within a given buffer.
 * ARGUMENTS: . Buffer which stores the syntax to be checked (char *buffer).
 * RETURNS: CalError SYNTAX or CalError OK
 *********************************************************************/
static CalError syntaxCheck(char *buffer);
  /*********************************************************************
 * FUNCTION NAME: addParam
 * PURPOSE: Adds a parameter to the end of the linked list.
 * ARGUMENTS: . CalProp which contains the parameter linked list
 *              (CalProp *prop).
 *            . CalParam to be added into the linked list (CalParam *add).
 * RETURNS: CalError SYNTAX or CalError OK
 *********************************************************************/
static void addParam(CalProp *prop,CalParam *add);
 /*********************************************************************
 * FUNCTION NAME: tokenizeParen
 * PURPOSE: Tokenizes a buffer ignoring any delimiters inside parenthesis.
 * ARGUMENTS: . Buffer to be tokenized (char *buffer).
 *            . Delimiter to be used when tokenizing (char *delim).
 *            . Array in which the tokens will be stored(char ***tokenArray).
 *            . Size of the token array(int *arraySize).
 *********************************************************************/
static void tokenizeParen(char* buffer,char *delim,char ***tokenArray
    ,int *arraySize);
 /*********************************************************************
 * FUNCTION NAME: fillParams
 * PURPOSE: Fills the CalParams struct with data.
 * ARGUMENTS: . CalParam struct to be filled in (CalParam **param).
 *            . String in which the data is stored (char *data).
 *********************************************************************/
static void fillParams(CalParam **param,char *data);
 /*********************************************************************
 * FUNCTION NAME: initializeProp
 * PURPOSE: Initializes all the members of a CalProp struct to NULL/0.
 * ARGUMENTS: . CalProp to be initialized(CalProp *prop).
 *********************************************************************/
static void initializeProp(CalProp *prop);
/*********************************************************************
 * FUNCTION NAME: checkProp
 * PURPOSE: Finds what kind of property CalProp is.
 * ARGUMENTS: CalProp to be checked.
 * RETURNS: Kind of property CalProp is(int): _BEGIN, _END, _ORDINARY
 *********************************************************************/
static int checkProp(CalProp *prop);
/*********************************************************************
 * FUNCTION NAME: vpCheck
 * PURPOSE: Checks CalProp for proper version and prodid presence.
 * ARGUMENTS: CalProp to be checked.
 * RETURNS: CalError: OK, BADVER, NOPROD
 *********************************************************************/
static CalError vpCheck(CalProp *prop);
/*********************************************************************
 * FUNCTION NAME: initializeComp
 * PURPOSE: Allocates memory for CalComp and sets all members to NULL/0.
 * ARGUMENTS: CalComp to be initialized (CalComp **pcomp).
 *********************************************************************/
static void initializeComp(CalComp **pcomp);
/*********************************************************************
 * FUNCTION NAME: getLine
 * PURPOSE: Returns a line once \n is read from the stream.
 * ARGUMENTS: . File stream to be read from (FILE *file).
 * RETURNS: String that has been read from the file(char *).
 *********************************************************************/
static char *getLine(FILE *file);
/*********************************************************************
 * FUNCTION NAME: checkCRNL
 * PURPOSE: Checks whether a string ends with CRNL(\r\n).
 * ARGUMENTS: String to be checked(char *line).
 * RETURNS: Boolean based on if the string has proper format.
 *********************************************************************/
static bool checkCRNL(char *line);
/*********************************************************************
 * FUNCTION NAME: checkEmpty
 * PURPOSE: Checks whether a string only contains whitespace.
 * ARGUMENTS: String to be checked(char *line).
 * RETURNS: . Whether or not the string is empty (bool).
 *********************************************************************/
static bool checkEmpty(char *line);
/*********************************************************************
 * FUNCTION NAME: removeEOL
 * PURPOSE: Removes the CRNL(\r\n) characters from a string.
 * ARGUMENTS: String to be have CRNL removed from(char *line).
 *********************************************************************/
static void removeEOL(char *line);
/*********************************************************************
 * FUNCTION NAME: removeSpace
 * PURPOSE: Removes a space from the beginning of a string.
 * ARGUMENTS: String to be have the same removed from(char *line).
 *********************************************************************/
static void removeSpace(char *line);
/*********************************************************************
 * FUNCTION NAME: addToFlex
 * PURPOSE: Adds a CalComp to the flexible array inside another CalComp
 *          at the last position in the flexible array.
 * ARGUMENTS: . CalComp which contains the flexible array to be 
 *              added to(CalComp **comp).
 *            . CalComp to be added (CalComp *addComp).
 *********************************************************************/
static void addToFlex(CalComp **comp,CalComp *addComp);
/*********************************************************************
 * FUNCTION NAME: printProp
 * PURPOSE: Prints contents of properties recursively 
 *          in proper format to file pointer.
 * ARGUMENTS: . Property to be printed(CalProp prop).
 *            . File pointer to be written to(FILE *ics).
 *********************************************************************/
static CalError printProps(FILE *ics,CalProp *prop
    ,int *linesRead);
/*********************************************************************
 * FUNCTION NAME: printParam
 * PURPOSE: Prints contents of all props recursively in ics 
 *          style format to file pointer.
 * ARGUMENTS: . Property to be printed(CalProp prop).
 *            . File pointer to be written to(FILE *ics)
 *            . Amount of lines that were read
 * RETURNS: IOERR if printing malfunctions(CalError)
 *********************************************************************/
static char *getParams(FILE *ics, CalParam *param);
/*********************************************************************
 * FUNCTION NAME: isFold
 * PURPOSE: Checks if a line should fold.
 * ARGUMENTS: . Line to be checked(char *line).
 * RETURNS: Whether the line folds(bool).
 *********************************************************************/
static bool shouldFold(char *line);
/*********************************************************************
 * FUNCTION NAME: foldLine
 * PURPOSE: Creates copy of a line, folded at FOLD_LEN.
 * ARGUMENTS: . Line to made into folded line(char *line).
 * RETURN: Folded line(char *line).
 *********************************************************************/
static char *foldLine(char **line,int *foldCount);

static bool shouldFold(char *line){
    return (strlen(line) > FOLD_LEN);
}

static char *foldLine(char **line,int *foldCount){
    char *foldedLine = getCopy(*line);
    char *nline = NULL,*tempLine=NULL;

    /*Initialize variables*/
    free(*line);
    *line = NULL;

    /*Get rid of CRNL*/
    removeEOL(foldedLine);

    /*Create a folding line*/
    for (int i=0;i<=(strlen(foldedLine)-1)/FOLD_LEN;i++){

        nline = malloc(sizeof(char)*(FOLD_LEN+3));
        if (i == 0){
            /*Leave 75 chars in first line*/
            for (int j=0;j<FOLD_LEN;j++){
                nline[j] = foldedLine[(i*FOLD_LEN)+j];
            }
            nline[FOLD_LEN] = '\0';
        }

        /*Rest of the lines only have 74 chars 
          due to space*/
        else{
            for (int j=0;j<FOLD_LEN-1;j++){
                if ((i*FOLD_LEN)+j >= strlen(foldedLine)){
                    nline[j] = '\0';
                    break;
                }
                nline[j] = foldedLine[(i*FOLD_LEN)+j];
            }   
            nline[FOLD_LEN-1] = '\0';
        }

        /*Add CRNL*/
        strcat(nline,"\r\n");

        /*Add space to every folding line*/
        if (i){
            (*foldCount)++;

            tempLine = malloc(sizeof(char)
                *(strlen(nline)+2));
            assert(tempLine);
            tempLine[0] = ' ';
            tempLine[1] = '\0';
            strcat(tempLine,nline);
            free(nline);
            nline = tempLine;
        }

        /*Merge the lines*/
        if (!(*line)){
            *line = malloc(sizeof(char)
                *(strlen(nline)+1));
            assert(*line);
            strcpy(*line,nline);
        }
        else{
            *line = realloc(*line,sizeof(char)
                *(strlen(*line)+strlen(nline)+1));
            assert(*line);
            strcat(*line,nline);
        }

        free(nline);
    }

    free(foldedLine);

    return *line;
}

static char *getParams(FILE *ics, CalParam *param){
    int i;
    char *line=NULL,*value=NULL;
    char *nextParam=NULL;

    /*If parameter doesn't exist, return*/
    if (!param){
        return line;
    }

    /*Store paramter name*/
    line = malloc(sizeof(char)*(strlen(param->name)+3));
    assert(line);
    strcpy(line,";");
    strcat(line,param->name);
    strcat(line,"=");

    for (i=0;i<param->nvalues;i++){
        /*Print first and last value with no comma*/
        if (i==0){
            line = realloc(line,sizeof(char)
                *((strlen(line))+strlen((param->value)[i])+1));
            assert(line);
            strcat(line,(param->value)[i]);
        }
        /*Print values in between with commas*/
        else{
            line = realloc(line,sizeof(char)
                *((strlen(line))+strlen((param->value)[i])+2));
            assert(line);
            value = malloc(sizeof(char)
                *(strlen((param->value)[i])+2));
            assert(value);
            /*Add comma before value and concatonate*/
            strcpy(value,",");
            strcat(value,(param->value)[i]);
            strcat(line,value);
            free(value);
        }
    }    
    /*Add following parameters to current line*/
    if (param->next){
        nextParam = getParams(ics,param->next);
    }

    if (nextParam){
        line = realloc(line,sizeof(char)
            *(strlen(line)+strlen(nextParam)+1));
        assert(line);
        line = strcat(line,nextParam);
        free(nextParam);
    }

    return line;
}

static CalError printProps(FILE *ics,CalProp *prop
    ,int *linesRead){

    char *line=NULL,*add=NULL;
    CalError calError;

    /*Initialize calStatus*/
    calError = OK;

    if (!prop){
        return OK;
    }

    /*Add name to line*/
    line = getCopy(prop->name);

    /*Get the parameters*/
    add = getParams(ics,prop->param);

    if (add){
        /*Add parameters to line*/
        line = realloc(line,sizeof(char)
            *((strlen(line))+(strlen(add))+1));
        assert(line);
        strcat(line,add);
        free(add);
    }

    /*Add colon,value,CRNL*/
    add = malloc(sizeof(char)*(strlen(prop->value)+4));
    assert(add);
    strcpy(add,":");
    strcat(add,prop->value);
    strcat(add,"\r\n");
    line = realloc(line,sizeof(char)
        *((strlen(line))+(strlen(add))+1));
    assert(line);
    strcat(line,add);
    free(add);

    (*linesRead)++;
    /*Check for folding*/
    if (shouldFold(line)){
        foldLine(&line,linesRead);
    }

    /*Print the property*/
    if (fprintf(ics,"%s",line) < 0){
        calError = IOERR;
    }

    free(line);
    /*Print next property*/
    calError = printProps(ics,prop->next,linesRead);

    return calError;
}

static CalError calCheck(CalComp *comp){
    int i;

    for (i=0;i<comp->ncomps;i++){
        if (((((comp->comp)[i])->name)[0]) == 'V'){
            return OK;
        }
    }
    return NOCAL;
}

static char *getCopy(char *string){
    char *copy = malloc(sizeof(char)
        *(strlen(string)+1));
    assert(copy);
    strcpy(copy,string);
    return copy;
}

static bool isEOF(char *line){
    if (strlen(line) == 1 && line[0] == EOF){
        return true;
    }
    return false;
}    

static bool endsEOF(char *line){
    
    return (line[strlen(line)-1] == EOF);
}

static void setCompName(CalComp *comp, char *name){
    int size;
    size = sizeof(char)*(strlen(name)+1);
    comp->name = malloc(size);
    assert(comp->name);
    strcpy(comp->name,name);
}

static void addCalProp(CalProp *insert,CalComp *comp){
    CalProp *next;

    #ifdef DEBUG_COMP
    assert(comp != NULL);
    #endif

    /*Insert into the start of the list*/
    if (comp->prop == NULL){
        comp->prop = insert;
        comp->nprops++;
        return;
    }

    /*Traverse the list and add to the last position*/
    next = comp->prop;
    while (next->next != NULL){
        next = next->next;
    }
    next->next = insert;

    comp->nprops++;
}

static void stringToUpper(char *string){
    int i;

    for (i=0;i<strlen(string);i++){
        string[i] = toupper(string[i]);
    }
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

static void freeCalProp(CalProp **prop){

    CalParam **next;
    int i;

    /*Free the name and value*/
    if ((*prop)->name != NULL){
        free((*prop)->name);        
    }
    if ((*prop)->value != NULL){
        free((*prop)->value);
    }

    if ((*prop)->param == NULL){
        free(*prop);
        return;
    }

    /*Free all the parameters and their contents*/
    if ((*prop)->param != NULL){
        for (i=0;i<(*prop)->nparams;i++){
            next = &((*prop)->param);
            while ((*next)->next != NULL){
                next = &((*next)->next);
            }
            freeCalParam(next);
            *next = NULL;
        }
    }

    free(*prop);

}

static void storeValue(int curSize,int value,int **array){
    if (*array == NULL){
        *array = malloc(sizeof(int));
        assert(*array);
    }
    else{
        *array = realloc(*array,(sizeof(int) 
            *curSize)+sizeof(int));
        assert(*array);
    }
    (*array)[curSize] = value;
}

static CalError syntaxCheck(char *buffer){
    int i,j;
    int equalsCount=0,colonCount=0,semiCount=0;
    int parenCount = 0,equalsParam = 0,whiteCount=0;
    int *equals = NULL,*colon = NULL,*semi = NULL,*white=NULL;
    bool ignoreParen = false, colonFound = false;
    CalError error = OK;

    /*Count the number of colons, semicolons
      and equals signs*/
    for (i=0;i<strlen(buffer);i++){
        /*Check if inside parenthesis or outside*/
        if (buffer[i] == '\"'){
            ignoreParen = !ignoreParen;
            parenCount++;
        }
        /*Count the characters only if were before the colon
          and not inside parenthesis*/
        if (!colonFound && !ignoreParen){
            switch(buffer[i]){
                case '=':
                    /*Only count one equals sign
                      per parameter*/
                    if (equalsParam != semiCount){
                        storeValue(equalsCount,i,&equals);
                        equalsCount++;
                        equalsParam = semiCount;
                    }
                    break;
                case ';':
                    storeValue(semiCount,i,&semi);
                    semiCount++;
                    break;
                case ':':
                    storeValue(colonCount,i,&colon);
                    colonFound = true;
                    colonCount++;
                    break;
                case ' ':
                    storeValue(whiteCount,i,&white);
                    whiteCount++;
                    break;
                case '\t':
                    storeValue(whiteCount,i,&white);
                    whiteCount++;
                    break;
                default:
                    break;
            }
        }
    }

    /*If a parameter value contains a parenthesis
      character than return SYNTAX*/
    if (error != SYNTAX && ((parenCount%2) != 0)){
        error =  SYNTAX;
    }
    /*More than one colon is not allowed*/
    if (error != SYNTAX && colonCount != 1){
        error = SYNTAX;
    }
    /*Check to make sure colon is in the right
      position, meaning after all the parameters*/
    if (error != SYNTAX && semiCount > 0){
        for (i=0;i<semiCount;i++){
            if (semi[i] > colon[0]){
                error = SYNTAX;
            }
        }
    }
    /*Make sure every parameter contains an equals 
      sign*/
    if (error != SYNTAX && semiCount != equalsCount){
        error = SYNTAX;
    }
    /*If a semicolon or colon exists at position 0
      the prop name is empty*/
    if (error != SYNTAX && semiCount > 0){
        if (semi[0] == 0){
           error = SYNTAX;
        }
    }
    if (error != SYNTAX && colonCount > 0){
        if (colon[0] == 0){
            error = SYNTAX;
        }
    }   
    /*Check to make sure that no empty parameters
      exist, check all but the last*/
    if (error != SYNTAX && semiCount > 0){
        for (i=0;i<semiCount-1;i++){
            if ((semi[i+1]-semi[i]) < 2){
                error = SYNTAX;
            }
        }
        /*Check the last parameters length by comparing
          it with the position of the colon*/
        if ((colon[colonCount-1]-semi[semiCount-1]) < 2){
            error = SYNTAX;
        }
    }

    /*Check for empty parameter name*/
    if (error != SYNTAX){
        for (i=0;i<semiCount;i++){
            for (j=0;j<equalsCount;j++){
                if (semi[i] == (equals[j]-1)){
                    error = SYNTAX;
                    break;
                }
            }
        }
    }

    /*Check for unexpected whitespace*/
    if (error != SYNTAX){
        for (i=0;i<whiteCount;i++){

            if ((buffer[(white[i]+1)] == ':')
                || (buffer[(white[i]-1)] == ':')){
                error = SYNTAX;
                break;
            }
            else if ((buffer[(white[i]+1)] == ';') 
                || (buffer[(white[i]-1)] == ';')){
                error = SYNTAX;
                break;
            }
            else if ((buffer[(white[i]+1)] == '=')
                || (buffer[(white[i]-1)] == '=')){
                error = SYNTAX;
                break;
            }
        }
    }

    /*Free memory*/
    free(white);
    free(semi);
    free(colon);
    free(equals);

    return error;
}

static void addParam(CalProp *prop,CalParam *add){
    CalParam *next;

    #ifdef DEBUG_PARSE
    assert(add != NULL);
    #endif

    /*Add to the beginning of the list if empty*/
    if (prop->param == NULL){
        prop->param = add;
    }
    /*Traverse the list and add to end*/
    else{
        next = prop->param;
        while (next->next != NULL){
            next = next->next;
        }
        next->next = add;
    }
    prop->nparams++;
}

static void tokenizeParen(char* buffer,char *delim,char ***tokenArray
    , int *arraySize){

    char *token;
    int parenNum = 0,tokenNum = 0,i;
    int oldSize,newSize;
    bool catLast = false,newString = false;

    /*If buffer NULL return one empty line array element*/
    if (buffer == NULL){
        *arraySize = 1;
        *tokenArray = malloc(sizeof(char*));
        assert(*tokenArray);

        (*tokenArray)[0] = malloc(sizeof(char));
        assert((*tokenArray)[0]);
        (*tokenArray)[0][0] = '\0';

        return;
    }

    /*Get all the tokens ignoring parenthesis*/
    token = strtok(buffer,delim);
    while (token != NULL){

        /*Find the number of parenthesis in token*/
        for (i=0;i<strlen(token);i++){
            if (token[i] == '\"'){
                parenNum++;
            }
        }

        /*Allocate memory for char array*/
        if (*tokenArray == NULL){
            *tokenArray = malloc(sizeof(char*));
            assert(*tokenArray);
            (*tokenArray)[0] = NULL;
        }
        else{
            if (newString){
                oldSize = sizeof(char*)*(tokenNum+1);
                newSize = sizeof(char*);

                *tokenArray = realloc(*tokenArray
                    ,oldSize + newSize);
                assert(*tokenArray);
                (*tokenArray)[tokenNum] = NULL;
            }
        }

        /*If there is an odd number of parenthesis
        add the current token to the previous ones*/
        if (parenNum%2 != 0){

            if ((*tokenArray)[tokenNum] == NULL){

                (*tokenArray)[tokenNum] = malloc(sizeof(char)
                    *(strlen(token)+2));
                assert((*tokenArray)[tokenNum]);
                strcpy((*tokenArray)[tokenNum],token);

                /*Add delimiter back*/
                (*tokenArray)[tokenNum][strlen((*tokenArray)[tokenNum])+1] 
                    = '\0';
                (*tokenArray)[tokenNum][strlen((*tokenArray)[tokenNum])]
                    = delim[0];

            }
            /*If were only adding to the current token since were toking
              between parenthesis than concatonate the strings*/
            else{

                /*Add the delimiter back since its between parenthesis*/
                oldSize = sizeof(char)*
                    (strlen((*tokenArray)[tokenNum])+1);
                (*tokenArray)[tokenNum] 
                	= realloc((*tokenArray)[tokenNum],oldSize + sizeof(char));
                assert((*tokenArray)[tokenNum]);

                (*tokenArray)[tokenNum][strlen((*tokenArray)[tokenNum])+1] 
                    = '\0';
                (*tokenArray)[tokenNum][strlen((*tokenArray)[tokenNum])]
                    = delim[0];

                oldSize = sizeof(char)
                	*(strlen((*tokenArray)[tokenNum])+1);
                newSize = sizeof(char)*(strlen(token)+1);

                (*tokenArray)[tokenNum] 
                	= realloc((*tokenArray)[tokenNum],oldSize + newSize);
                assert((*tokenArray)[tokenNum]);
                strcat((*tokenArray)[tokenNum],token);
            }
            catLast = true;
            newString = false;

        }
        /*If there is an even number of parenthesis than add
          the token to the token array at a new position*/
        else{
            if (catLast){
                oldSize =sizeof(char) *(strlen((*tokenArray)[tokenNum])+1);
                newSize =sizeof(char) *(strlen(token)+1);
                (*tokenArray)[tokenNum] = realloc((*tokenArray)[tokenNum],
                    oldSize + newSize);
                assert((*tokenArray)[tokenNum]);
                
                strcat((*tokenArray)[tokenNum],token);
                catLast = false;
            }
            else{
                (*tokenArray)[tokenNum] = malloc(sizeof(char)
                *(strlen(token)+1));
                assert((*tokenArray)[tokenNum]);
                strcpy((*tokenArray)[tokenNum],token);
            }

            tokenNum++;
            newString = true;
        }

        #ifdef DEBUG_PARSE
        printf("-----------------------------------------------------\n");
        #endif

        token = strtok(NULL,delim);
    }

    *arraySize = tokenNum;
}

static void fillParams(CalParam **param,char *data){
    char **tokenArray = NULL,*values,*token;
    int i, arraySize;

    /*Get the name of the prop first which is at the 
      beginning of the data*/
    token = strtok(data,"=");
    (*param)->name = malloc(sizeof(char)*(strlen(token)+1));
    assert((*param)->name);
    strcpy((*param)->name,token);
    stringToUpper((*param)->name);

    /*Get all the values seperated by commas*/
    token = strtok(NULL,"=");
    if (token != NULL){
        values = malloc(sizeof(char)*(strlen(token)+1));
        assert(values);
        strcpy(values,token);
    }
    else{
        values = NULL;
    }
    tokenizeParen(values,",",&tokenArray,&arraySize);

    /*Create memory for flexible array in the struct*/
    *param = realloc(*param,sizeof(CalParam)
        +(sizeof(char*)*arraySize));
    assert(*param);

    /*Store all the values in the parameter*/
    (*param)->nvalues = 0;
    for (i=0;i<arraySize;i++){

        ((*param)->value)[i] = malloc(sizeof(char)
            *(strlen(tokenArray[i])+1));
        assert(((*param)->value)[i]);
        strcpy((*param)->value[i],tokenArray[i]);

        /*Uppercase values not in between quotes*/
        if (((*param)->value[i])[0] != '\"'){
        	//stringToUpper((*param)->value[i]);
        }

        (*param)->nvalues++;
        free(tokenArray[i]);
    }

    free(values);
    free(tokenArray);
}

static void initializeParam(CalParam *param){
    param->name = NULL;
    param->next = NULL;
}

static void initializeProp(CalProp *prop){
    prop->name = NULL;
    prop->value = NULL;
    prop->nparams = 0;
    prop->param = NULL;
    prop->next = NULL;
}

static int checkProp(CalProp *prop){
    if (strcmp(prop->name,"BEGIN") == 0){
        return _BEGIN;
    }
    else if (strcmp(prop->name,"END") == 0){
        return _END;
    }
    return _ORDINARY;
}

static CalError vpCheck(CalProp *prop){
    CalProp *next=NULL;
    char *propName= NULL,*propValue=NULL;
    bool versionError=false,prodidError=false;
    bool validValue = true;
    int prodidCount=0,versionCount=0;

    /*Check if prod is NULL*/
    if (!prop){ return BADVER; }
    next = prop;

    /*Traverse all the props checking each for a 
      version or prodid*/
    do{
        propName = next->name;
        propValue = next->value;

        if (strcmp(propName,"VERSION") == 0){
            versionCount++;
            if (strcmp(propValue,VCAL_VER) != 0){
                validValue = false;
            }
        }
        if (strcmp(propName,"PRODID") == 0){
            prodidCount++;
        }
        if (next->next != NULL){
            next = next->next;
        }
        else{
            break;
        }
    }while (next != NULL);

    /*Set version and prodid errors based on the
      prodid and version data*/
    if (versionCount > 1 || versionCount == 0
        || !validValue){
        versionError = true;
    }
    if (prodidCount > 1 || prodidCount == 0){
        prodidError = true;
    }

    /*Returns the errors found or OK if none were*/
    if (versionError && prodidError){
        return BADVER;
    }
    else if (versionError){
        return BADVER;
    }
    else if (prodidError){
        return NOPROD;
    }
    return OK;
}

static void initializeComp(CalComp **pcomp){

    *pcomp = malloc(sizeof(CalComp));
    assert(*pcomp);
    (*pcomp)->name = NULL;
    (*pcomp)->nprops = 0;
    (*pcomp)->prop = NULL;
    (*pcomp)->ncomps = 0;
}

static char *getLine(FILE *file){

    int charCount = 0;
    char currentChar ='\0', *line=NULL;

    #ifdef DEBUG_PARSE
    assert(file != NULL);
    #endif

    /*Return EOF if at end of file*/
    if (feof(file)){
        line = malloc(sizeof(char)*2);
        assert(line);
        line[0] = EOF;
        line[1] = '\0';

        return line;
    }
    /*Parse until new line or EOF is found*/
    while (currentChar != '\n' && !feof(file)){
        currentChar = fgetc(file);

        /*Allocate memory*/
        if (!line){
            line = malloc(sizeof(char)*2);
            assert(line);
        }
        else{
            line = realloc(line,sizeof(char)
                *(charCount+2));
            assert(line);
        }
        /*Store character inside string*/
        line[charCount] = currentChar;
        charCount++;
    }
    
    line[charCount] = '\0';
    return line;
}

static bool checkCRNL(char *line){

    regex_t regex;
    int regResult;

    /*Check for \r and \n in the string*/
    regResult = regcomp(&regex, ".*\r\n$",0);
    if (regResult == 1){ 
        exit(1);
    }
    
    /*If the regex matched return OK
      otherwise return NOCRNL*/
    if ((regexec(&regex,line,0,NULL,0) == 0)){
        if (regResult == 0){
            regfree(&regex);
        }
        return true;
    }
    else{
        if (regResult == 0){
            regfree(&regex);
        }
        return false;
    }
    return true;
}

static bool checkEmpty(char *line){
    regex_t regex;
    int regResult;

    #ifdef DEBUG_EMPTY
    assert (line != NULL);
    #endif
    /*Check for only whitespace*/
    regResult = regcomp(&regex, "^[[:space:]\r\n]*$",0);
    if (regResult == 1){ 
        exit(1);
    }

    /*If regex matched than the line is empty
      otherwise a whitespace exists*/
    if (regexec(&regex,line,0,NULL,0) == 0){
        if (regResult == 0){
            regfree(&regex);
        }
        return true;
    }
    if (regResult == 0){
        regfree(&regex);
    }

    return false;
}

static void removeEOL(char *line){
    int i,j;
    bool removed = false;

    /*Remove EOF character, if one exists
      than also remove any \r\n that exist
      in the line*/
    for (i=0;i<strlen(line);i++){
        if (line[i] == EOF){
            line[i] = '\0';
            for (j=0;j<strlen(line);j++){
                if (line[j] == '\n' || line[j] == '\r'){
                    line[j] = '\0';
                }
            }
            removed = true;
            break;
        }
    }
    if (removed){ return; }
    /*If two consecutive characters are \r than \n
      remove them*/
    for (i=0;i<strlen(line);i++){
        if (line[i] == '\r'){
            if (i+1 <= strlen(line)){
                if (line[i+1] == '\n'){
                    line[i] = '\0';
                    line[i+1] = '\0';
                }
            }
        }
    }

}

static void removeSpace(char *line){
    int i = 0;

    /*Shift the string leftwards, writing 
      over the space, removing it*/
    while (line[i] != '\0'){
        line[i] = line[i+1];
        i++;
    }
}   

static void addToFlex(CalComp **comp,CalComp *addComp){
    int ncomps = (*comp)->ncomps;
    size_t compSize = sizeof(CalComp[ncomps])+sizeof(CalComp);

    /*Create space in the flexible array for another element*/
    *comp = realloc(*comp,compSize);
    assert(*comp);
    ((*comp)->comp)[ncomps-1] = addComp;
}

CalStatus readCalFile( FILE *const ics, CalComp **const pcomp ){

    CalStatus calStatus;
    char *line;

    #ifdef DEBUG_FILE
    assert(ics != NULL);
    #endif

    /*Call readCalLine() with NULL arguments so it can 
    initialize itself*/
    calStatus = readCalLine(NULL,&line);
    initializeComp(pcomp);
    calStatus = readCalComp(ics,pcomp);

    if (calStatus.code != OK){
        freeCalComp(*pcomp);
        return calStatus;
    }

        /*Check for no componenets that start with V*/
    calStatus.code = calCheck(*pcomp);
    if (calStatus.code != OK){
        freeCalComp(*pcomp);
        return calStatus;
    }

    /*Check properties for proper version and prodid*/
    calStatus.code = vpCheck((*pcomp)->prop);
    if (calStatus.code != OK){
        freeCalComp(*pcomp);
        return calStatus;
    }

    /*Check to make sure that readCalComp ended at EOF
      and not before*/
     calStatus = readCalLine(ics,&line);
     if (line != NULL){
        calStatus.code = AFTEND;
        free(line);
        return calStatus;
     }
     free(line);

    return calStatus;
}

CalStatus readCalComp( FILE *const ics, CalComp **const pcomp ){

    static int nestCount;
    static CalComp **prevComp = NULL;
    CalStatus calStatus;
    CalComp *thisPrevComp = NULL,*addComp = NULL;
    CalProp *calProp = NULL;
    bool compSwitched = false;
    char * buffer=NULL,*upperString=NULL;
    int propType;

    #ifdef DEBUG_COMP
    assert(ics != NULL);
    assert(pcomp != NULL);
    #endif

    calStatus.code = OK;

    /*Check to make sure first line is BEGIN:VCALENDAR*/
    if ((*pcomp)->name == NULL){

        #ifdef DEBUG_COMP
        printf("(*comp)->name is NULL.\n");
        #endif

        /*Get first line*/
        calStatus = readCalLine(ics,&buffer);

        /*Empty file*/
        if (buffer == NULL){
            return calStatus;
        }

        if (calStatus.code != OK){
            free(buffer);
            return calStatus;
        }

        /*Parse first line to get name*/
        calProp = malloc(sizeof(CalProp));
        assert(calProp);
        calStatus.code = parseCalProp(buffer,calProp);
        if (calStatus.code != OK){
            freeCalProp(&calProp);
            return calStatus;
        }        

        upperString = getCopy(buffer);
        stringToUpper(upperString); 

        /*Free and return if first line did not match*/
        if (strcmp(upperString,"BEGIN:VCALENDAR") != 0){
            #ifdef DEBUG_COMP
            printf("First line did not match: \"BEGIN:VCALENDAR\"\n");
            #endif

            calStatus.code = NOCAL;
            freeCalProp(&calProp);
            return calStatus;
        }
        /*If first line matched than set comp member values accordingly*/
        else if (strcmp(upperString,"BEGIN:VCALENDAR") == 0){
            #ifdef DEBUG_COMP
            printf("First line matched: \"BEGIN:VCALENDAR\"\n");
            #endif

            (*pcomp)->ncomps = 0;
            (*pcomp)->prop = NULL;
            nestCount = 1;
            setCompName(*pcomp,calProp->value);
        }
        free(upperString);
        freeCalProp(&calProp);
        free(buffer);
    }

    /*Read the contents of the file line by line, parsing them
      and fill in the CalComp(s) and their flexible arrays
      accordingly*/
    while (calStatus.code == OK){
    
        /*Get the next line*/
        calStatus = readCalLine(ics,&buffer);
        if (calStatus.code != OK){
            free(buffer);
            return calStatus;
        }

        /*If reached EOF before END, return*/
        if (!buffer){
            free(buffer);
            calStatus.code = BEGEND;
            return calStatus;
        }

        #ifdef DEBUG_COMP
        printf("buffer: %s\n",buffer);
        printf("line: %d\n",calStatus.lineto);
        #endif

        /*Get the CalProp of the line read*/
        calProp = malloc(sizeof(CalProp));
        assert(calProp);
        calStatus.code = parseCalProp(buffer,calProp);
        free(buffer);
        if (calStatus.code != OK){
            freeCalProp(&calProp);
            return calStatus;
        }

        /*Find out what kind of CalProp was read*/
        propType = checkProp(calProp);

        /*If CalProp is BEGIN than create a new CalComp and 
          add it to the flexible array, than call
          readCalComp() recursively*/
        if (propType == _BEGIN){

            nestCount++;

            #ifdef DEBUG_COMP
            printf("-------------NEW BEGIN-----------------\n");
            printf("proptype: BEGIN\n");
            printf("nest count: %d\n",nestCount);
            #endif

            if (nestCount > 3){
                calStatus.code = SUBCOM;
                freeCalProp(&calProp);
                return calStatus;
            }

            /*Only allocate memory if its not the very first BEGIN*/
            (*pcomp)->ncomps++;

            /*Initialize and allocate memory for new cal comp*/
            initializeComp(&addComp);
            setCompName(addComp,calProp->value);
            addToFlex(pcomp,addComp);
            if (nestCount > 2){
                /*Update the address of the flexible array member in the previous
                  blocks flexible array because the realloc changed its address*/
                if (compSwitched){
                    (thisPrevComp->comp)[thisPrevComp->ncomps-1] = *pcomp;
                }
                else{
                    ((*prevComp)->comp)[(*prevComp)->ncomps-1] = *pcomp;
                }
            }

            if (addComp->name == NULL){
                setCompName(addComp,calProp->value);
            }
            /*Get the previous comp for this specific BEGIN
              block*/
            if (!compSwitched && prevComp != NULL){
                thisPrevComp = *prevComp;
                compSwitched = true;
            }
            prevComp = pcomp;

            /*Free CalProp and make a recursive call to 
              read the new BEGIN block*/
            freeCalProp(&calProp);
            calStatus = readCalComp(ics,&addComp);

            if (calStatus.code != OK){
                return calStatus;
            }
        }
        /*If CalProp is an END than reduce nesting by one and 
          make sure its name matches the name of the BEGIN*/
        else if (propType == _END){
            #ifdef DEBUG_COMP
            printf("proptype: END\n");
            #endif

            upperString = getCopy(calProp->value);
            stringToUpper(upperString);

            /*Check for matching names with the BEGIN*/
            if (strcmp(upperString,(*pcomp)->name) != 0){
                calStatus.code = BEGEND;
                freeCalProp(&calProp);
                return calStatus;
            }

            free(upperString);

            /*Check if data exists within the block*/
            if (((*pcomp)->nprops == 0) && ((*pcomp)->ncomps == 0)){
                calStatus.code = NODATA;
                freeCalProp(&calProp);
                return calStatus;
            }
            nestCount--;

            #ifdef DEBUG_COMP
            printf("nestcount: %d\n",nestCount);
            printf("------------------END OF BLOCK-----------------\n\n\n");
            #endif

            freeCalProp(&calProp);
            return calStatus;
        }
        /*An ordinary CalProp is added to the CalProp linked list*/
        else if (propType == _ORDINARY){

            #ifdef DEBUG_COMP
            printf("proptype: ORDINARY\n");
            #endif

            addCalProp(calProp,*pcomp);

            #ifdef DEBUG_COMP
            printf("Added cal prop.\n");
            #endif
        }
    }

    freeCalProp(&calProp);
    return calStatus;
}

CalStatus readCalLine( FILE *const ics, char **const pbuff ){
    static int currentLine;
    bool folding=false,firstRead=true;
    char *line=NULL,foldChar;
    size_t size;
    CalStatus calStatus;    

    /*Reset state in preparation for initializing a new series of reads*/    
    if (ics == NULL){

        #ifdef DEBUG_LINE
        printf("ics is NULL, reseting state.r\n");
        #endif

        currentLine = 0;
        calStatus.code = OK;
        calStatus.linefrom = calStatus.lineto = 0;

        return calStatus;
    }

    /*Initialize CalStatus error to OK*/
    calStatus.code = OK;

    /*Set pbuff to EOF and status to OK if at end of file*/
    if (feof(ics)){
        calStatus.linefrom = currentLine;
        calStatus.lineto = currentLine;
        *pbuff = NULL;
        return calStatus;        
    }

    /*Increment current line after checking for EOF*/
    currentLine++;
    calStatus.linefrom = currentLine;
    calStatus.lineto = currentLine;

    /*Get all lines including folded ones*/
    do{
        line = getLine(ics);

        /*Folding was set as a result of an empty line so return*/
        if (checkEmpty(line) && folding){
            return calStatus;
        }

        /*If the line is empty and not EOF, move on to next*/
        while (checkEmpty(line)){
            free(line);
            line = getLine(ics);
            currentLine++;

            /*First line parsed is empty so update line from to next line*/
            if (firstRead){
                calStatus.linefrom = currentLine;
                firstRead = false;
            }
            calStatus.lineto = currentLine;
        }

        /*If eof was returned by getLine, concatonate or return EOF*/
        if (isEOF(line)){
            if (folding){
                size = sizeof(char)*((strlen(*pbuff)+1)+2);
                *pbuff = realloc(*pbuff,size);
                assert(*pbuff);
                (*pbuff)[0] = EOF;
                (*pbuff)[1] = '\0';
            }
            else{
                *pbuff = NULL;
                /*Fix the current line since EOF doesn't count
                 towards line count*/
                currentLine--;
                calStatus.linefrom = currentLine;
                calStatus.lineto = currentLine;
            }

            return calStatus;
        }

        /*Check for current EOL characters*/
        if (!endsEOF(line) && !checkCRNL(line)){
            calStatus.code = NOCRNL;
            return calStatus;
        }

        /*Get rid of CRNL or EOF*/
        removeEOL(line);

        /*Get rid of the first space of a folding line*/
        if (folding){
            removeSpace(line);
        }

        /*Store the current line read to pbuff*/
        if (!folding){
            size = sizeof(char)*(strlen(line)+1);
            (*pbuff) = malloc(size);
            assert(*pbuff);
            strcpy(*pbuff,line);
            free(line);
        }

        /*Concatenate if adding folded line*/
        else{
            size = sizeof(char)
                *((strlen(*pbuff)+1)+(strlen(line)+1));
            *pbuff = realloc(*pbuff,size);
            assert(*pbuff);
            strcat(*pbuff,line);
            free(line);
        }
        
        /*Check for folding*/
        foldChar = fgetc(ics);
        if (foldChar == ' ' || foldChar == '\t'){
            folding = true;
            firstRead = false;
            /*Increment line num since another line will be read*/
            currentLine++;
            calStatus.lineto = currentLine;
        }
        else{
            folding = false;
        }
        /*Move file pointer back to old position if not EOF*/
        if (foldChar != EOF){
            ungetc(foldChar,ics);
        }

    }while(folding);

    return calStatus;
}

CalError parseCalProp( char *const buff, CalProp *const prop ){
    
    CalParam *param;
    CalError error = OK;
    char **semiTokens = NULL,**lastTokens = NULL;
    char **colonTokens = NULL,*buffer;
    int colonCount = 0,lastCount = 0,semiCount = 0;
    int i,oldSize,newSize,nameLength;

    #ifdef DEBUG_PARSE
    assert(buff != NULL);
    #endif

    /*Copy buffer to not disrupt it*/
    buffer = malloc(sizeof(char)*(strlen(buff)+1));
    assert(buffer);
    strcpy(buffer,buff);

    initializeProp(prop);

    /*Make sure no syntax errors exist*/
    if (syntaxCheck(buffer) == SYNTAX){
        error = SYNTAX;
        free(buffer);
        return SYNTAX;
    }

    if (error != SYNTAX){
        /*Seperate the string by the colon to get prop
         name and prop value*/
        tokenizeParen(buffer,":",&colonTokens,&colonCount);

        tokenizeParen(colonTokens[0],";",
            &semiTokens,&semiCount);

        /*If theres only one token than no parameters exist
          so simply store the prop name*/
        if (semiCount == 1){
            
            /*Get the prop name*/
            nameLength = strlen(semiTokens[0])+1;
            prop->name = malloc(sizeof(char)*nameLength);
            assert(prop->name);
            strcpy(prop->name,semiTokens[0]);
            stringToUpper(prop->name);
        }

        /*If more than one token exists than there is at least one
          parameter, so they all need to be stored*/
        if (semiCount > 1){
             /*Store the prop name which is in the first token*/
             prop->name = malloc(strlen(semiTokens[0])+1);
             assert(prop->name);
             strcpy(prop->name,semiTokens[0]);
             stringToUpper(prop->name);

            /*The remaining tokens are all parameters so store them*/
            for (i = 1; i<semiCount; i++){
                param = malloc(sizeof(CalParam));
                assert(param);
                initializeParam(param);
                fillParams(&param,semiTokens[i]);
                addParam(prop,param);
            }
        }


        /*If no property value is present store an empty one*/
        if (colonCount == 1){
            prop->value = malloc(sizeof(char));
            assert(prop->value);

            (prop->value)[0] = '\0';
        }
        /*Store the property value which is one or all the remaining
          tokens after the first one*/
        for (i=1;i<colonCount;i++){
            if (prop->value == NULL){
                prop->value = malloc(sizeof(char)
                    *(strlen(colonTokens[i])+1));
                assert(prop->value);  
                strcpy(prop->value,colonTokens[i]); 
            }
            else{
                oldSize = sizeof(char)*(strlen(prop->value)+1);
                newSize = sizeof(char)*(strlen(colonTokens[i])+2);
                prop->value = realloc(prop->value,oldSize+newSize);
                assert(prop->value);

                /*Add colon back since it was removed during tokenization*/
                (prop->value)[strlen(prop->value)+1] = '\0';
                (prop->value)[strlen(prop->value)] = ':';

                strcat(prop->value,colonTokens[i]);     
            }   
        }
    }

    /*Free the tokens*/
    free(buffer);
    if (colonTokens != NULL){
        for (i=0;i<colonCount;i++){
            free(colonTokens[i]);
        }
        free(colonTokens);
    }
    if (semiTokens != NULL){
        for (i=0;i<semiCount;i++){
            free(semiTokens[i]);
        }
        free(semiTokens);
    }
    if (lastTokens != NULL){
        for (i=0;i<lastCount;i++){
            free(lastTokens[i]);
        }
        free(lastTokens);
    }
    return error;
}

CalStatus writeCalComp( FILE *const ics, const CalComp *comp ){
    static int compNum = 1;
    int i = 0,linesRead=0;
    char *name=NULL,*line=NULL;
    CalStatus calStatus,itirStatus;

    /*Initialize Calenar Status*/
    calStatus.code = OK;
    calStatus.linefrom = calStatus.lineto = 0;

    /*Create a string with the lines full contents*/
    name = getCopy(comp->name);
    line = malloc(sizeof(char)*(strlen("BEGIN:")+1));
    assert(line);
    strcpy(line,"BEGIN:");

    /*Merge 'BEGIN:' and 'name'*/
    line = realloc(line,sizeof(char)
        *((strlen(name))+(strlen(line))+1));
    assert(line);
    strcat(line,name); 
    free(name);

    /*Check for folding*/
    if (shouldFold(line)){
        foldLine(&line,&linesRead);
    }

    /*Print BEGIN:value */
    if (fprintf(ics,"%s\r\n",line) < 0){
        calStatus.code = IOERR;
        return calStatus;
    }
    free(line);
    linesRead++;

    /*Print properties*/
    calStatus.code = printProps(ics,comp->prop,&linesRead);
    
    /*Check for error in printProps*/
    if (calStatus.code == IOERR){
        /*Update lines read*/
        calStatus.linefrom += linesRead;
        calStatus.lineto += linesRead;
        return calStatus;
    }

    for (i=0;i<comp->ncomps;i++){
        compNum++;
        itirStatus = writeCalComp(ics,(comp->comp)[i]);

        /*Update lines read*/
        calStatus.linefrom += itirStatus.linefrom;
        calStatus.lineto += itirStatus.lineto;

        /*Check for error*/
        calStatus.code = itirStatus.code;
        if (calStatus.code == IOERR){
            calStatus.linefrom += linesRead;
            calStatus.lineto += linesRead;
            return calStatus;
        }
    }

    /*Create a string with the lines full contents*/
    name = getCopy(comp->name);
    line = malloc(sizeof(char)*(strlen("END:")+1));
    assert(line);
    strcpy(line,"END:");

    /*Merge 'BEGIN:' and 'name'*/
    line = realloc(line,sizeof(char)
        *((strlen(name))+(strlen(line))+1));
    assert(line);
    strcat(line,name);
    free(name);

    /*Check for folding*/
    if (shouldFold(line)){
        foldLine(&line,&linesRead);
    }

    /*Print END:value*/
    if (fprintf(ics,"%s\r\n",line) < 0){
        calStatus.code = IOERR;
        calStatus.linefrom += linesRead;
        calStatus.lineto += linesRead;
        return calStatus;
    }
    free(line);
    linesRead++;


    /*Update lines read*/
    calStatus.linefrom += linesRead;
    calStatus.lineto += linesRead;

    return calStatus;
}

void freeCalComp( CalComp *const comp ){

    int i;
    CalProp **next;

    if (comp == NULL){
        #ifdef DEBUG_FREE
        printf("Trying to free NULL comp. Returning.\n");
        #endif
        return;
    }
    #ifdef DEBUG_FREE
    printf("Freeing comp. props: %d\n",comp->nprops);
    #endif

    /*Free name*/
    if (comp->name){
        free(comp->name);
    }
    
    /*Free members of flexible array with
     recursive call*/
    for (i=0;i<comp->ncomps;i++){
        freeCalComp((comp->comp)[i]);
    }

    /*Free all the properties*/
    for (i=0;i<comp->nprops;i++){
        if (comp->prop){
            next = &(comp->prop);
            while ((*next)->next != NULL){
                next = &((*next)->next);
            }
            freeCalProp(next);
            *next = NULL;
        }
    }
    free(comp);
}