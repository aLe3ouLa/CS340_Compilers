#include "symtable.h"

/* For Printing */
char * typeNames[] = {"Global", "Local", "Formal", "User Function" , "Library Function" };

/* Hashing Function */
int hash(SymTable_t* symbolTable, const char *key ) {

	unsigned long int hashval = 0;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < 100000 && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[i];
		i++;
	}

	return hashval % symbolTable->size;
}

/* Create the symbol table */
SymTable_t* createSymbolTable (){
    
    int i = 0;
	SymTable_t* newSymbolTable = NULL;
	SymbolTableEntry_t * newSymbolTableEntry = NULL;
	
	/*Allocate the table structure*/
	newSymbolTable = malloc (sizeof(SymTable_t));
	if( newSymbolTable == NULL){
	    fprintf(stderr, "Cannot allocate the hash table.\n");
	    exit(EXIT_FAILURE);
	}
	
	/* Allocate the table itself */
	newSymbolTable->hash = malloc(sizeof(SymbolTableEntry_t*)*SYMBOL_TABLE_SIZE);
	
    if (newSymbolTable->hash == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate the hash table.\n");
	    exit(EXIT_FAILURE);
    }
    
    /* Initialize the pointers of the hash table with NULL */
    
    for(i = 0; i < SYMBOL_TABLE_SIZE; i++){ 
        newSymbolTable->hash[i] = NULL;
    }

    /* Store the size of the hash table in the structure */ 
     newSymbolTable->size = SYMBOL_TABLE_SIZE;
     /* Return a pointer to hash table */
     return newSymbolTable;
}

/* Create an Entry for the symbol table */
SymbolTableEntry_t* createSymbolTableEntry (char * name, int lineno, int scope, SymbolTableType_t type){

	/* Create new entry for symbol table */
    SymbolTableEntry_t *newnode;
    newnode = malloc(sizeof(SymbolTableEntry_t));
    
    if (newnode == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate a new node.\n");
        exit(EXIT_FAILURE);
    }

	/* Fill the data in node */
	/* Name of the symbol*/
    newnode->name = malloc ((strlen(name)+1)*sizeof(char));
    strcpy(newnode->name, name);
	/* Line where the symbol is found */
    newnode->lineno = lineno;
	/* What kind of symbol it is */
    newnode->type = type;
	/* The symbol is hidden?*/
    newnode->isActive = 1;
	/* The scope of the symbol*/
    newnode->scope = scope;
	
	/* Connect the symbols */
    newnode->next = NULL;
    
    /* Connect the scopes */
    newnode->nextinScope = NULL;
	newnode->prvScope = NULL;
	
    return newnode;

}

/* Insert symbol in symbol table */
void insertSymbol (SymTable_t* symbolTable, SymbolTableEntry_t* newEntry){
    /* hash the name of new entry */
    int hashingKey = hash(symbolTable, newEntry->name);
	
    SymbolTableEntry_t * lastinScope = NULL;
    bool exists = false;
    /* Find the last inserted in that scope*/
	lastinScope = lookup_LastSymbolInScope(symbolTable, newEntry->scope);
    
    /* If bucket is empty, just insert the new symbol in list */
    if (symbolTable->hash[hashingKey] == NULL){
        symbolTable->hash[hashingKey] = newEntry;
        newEntry->next = NULL;
    }else{
         /* If bucket is not empty, insert it in the beggining */
        newEntry->next = symbolTable->hash[hashingKey];
        symbolTable->hash[hashingKey] = newEntry;
    }
	
    /* Connect the same scope symbols -- Collision List */
    
    if (lastinScope!=NULL){
	/* if there is a symbol in same scope */
        newEntry->prvScope = lastinScope;
        lastinScope->nextinScope = newEntry;
        newEntry->nextinScope = NULL;
        
    }else{
	/* if the symbol is the first in that scope */
        newEntry->prvScope = NULL;
        newEntry->nextinScope = NULL;
    }
    
} 

/* Create a new argument for an entry */
Argument* createArgument (char* name,SymbolTableType_t type, int scope, int lineno){
    /* Create new argument*/
    Argument *newArgument = malloc(sizeof(Argument));
    if (newArgument == NULL){
        fprintf(stderr, "ERROR: Cannot allocate a new argument.\n");
        exit(EXIT_FAILURE);
    }
    /* fill argument data */
    newArgument->name =malloc((strlen(name)+1)*sizeof(char));
    strcpy (newArgument->name, name);
    newArgument->type = type;
    newArgument->scope = scope;
    newArgument->isActive = 1;
    newArgument->lineno = lineno;
    newArgument->next = NULL;
	
    return newArgument;
}

/* Insert the argument in the argument list */
void insertArgument (SymTable_t* symbolTable, SymbolTableEntry_t* mySymTabEntry, Argument* newArgument){

	SymbolTableEntry_t * argSymbol;
	/* First hash the argument in symbol table as a new symbol  */
	printf("\n\nname: %s | scope: %d\n\n", newArgument->name, newArgument->scope);
	argSymbol  = createSymbolTableEntry (newArgument->name, newArgument->lineno, newArgument->scope, newArgument->type);
	insertSymbol (symbolTable, argSymbol);
	
	/* Connect argument in the list of arguments */
    if(mySymTabEntry->argList != NULL){
		/* Insert new argument in the beginning of list */
        newArgument->next = mySymTabEntry->argList;
        mySymTabEntry->argList=newArgument;  
    }
    else {
        mySymTabEntry->argList = newArgument;
        newArgument->next=NULL;
    }

}

/* Find the first symbol inserted in scope */
SymbolTableEntry_t* lookup_firstSymbolInScope(SymTable_t *symbolTable, int scope){

    int i = 0;
    SymbolTableEntry_t *temp = NULL, *scoped = NULL;
    
    for (i = 0; i < symbolTable->size; i++){
            temp = symbolTable->hash[i]; 
			
            while (temp != NULL){
				/* Find an entry with scope and active */
                if (temp->scope == scope && temp->isActive ){
                    scoped = temp;       
                    break;
                }
                temp = temp->next;
            }
    }
    
    if (scoped != NULL){
	/* if an entry with this scope exists, has previous entries with that scope?*/
		while(scoped->prvScope != NULL){
			scoped = scoped->prvScope;
        }
        return scoped;
   }else {
		return NULL;
   }
            
    
}

/* Including hidden */
SymbolTableEntry_t* lookup_firstSymbolInScopeHidden(SymTable_t *symbolTable, int scope){

    int i = 0;
    SymbolTableEntry_t *temp = NULL;
	SymbolTableEntry_t *scoped = NULL;
    
    for (i = 0; i < symbolTable->size; i++){
            temp = symbolTable->hash[i]; 
			
            while (temp != NULL){
				/* Find an entry with scope and active */
                if (temp->scope == scope){
                    scoped = temp;       
                    break;
                }
                temp = temp->next;
            }
    }
    
    if (scoped != NULL){
	/* if an entry with this scope exists, has previous entries with that scope?*/
		while(scoped->prvScope != NULL){
			scoped = scoped->prvScope;
        }
        return scoped;
   }else {
		return NULL;
   }
            
    
}

/* Find the last symbol inserted with scope */
SymbolTableEntry_t* lookup_LastSymbolInScope(SymTable_t *symbolTable, int scope){

    int i = 0;
    SymbolTableEntry_t *temp = NULL, *scoped = NULL;
    
    for (i = 0; i < symbolTable->size; i++){
            /* Find an entry with scope and active */
			temp = symbolTable->hash[i]; 
            while (temp != NULL){
                if (temp->scope == scope && temp->isActive ){
                    scoped = temp;
                    break;
                }
                temp = temp->next;
            }
    }
            
			if (scoped != NULL){
			/* Find the last symbol inserted with that scope */
                while(scoped->nextinScope != NULL){
                  scoped = scoped->nextinScope;
                }
                return scoped;
            }else {
                return NULL;
            }     
    
}

/* Find a symbol with a particular name and scope */
bool lookup_specific_scope (SymTable_t* mySymTab, char* name, int scope){
    
    int i = 0;
    bool flag = false;
    SymbolTableEntry_t* temp;

	/* Find the first symbol with that scope */
    temp = lookup_firstSymbolInScope(mySymTab, scope);
    while (temp != NULL){
	/* Search an entry with name */	
        if(strcmp(temp->name, name) == 0 && temp->isActive){
            flag = true;
            return flag;
        }
        temp = temp->nextinScope;
    }
    
    return flag;
    
}

/* Find symbol with name, in all scopes -- slow approach */
bool lookup_all_scopes (SymTable_t* symbolTable, char* name){
    int i = 0;
    bool flag = false;
    SymbolTableEntry_t* temp;
    for (i = 0; i < SYMBOL_TABLE_SIZE; i++){
        temp = symbolTable->hash[i];
            while (temp != NULL){
                if (!strcmp(temp->name, name) && temp->isActive){
                    flag = true;
                    return flag;
                }
                temp = temp->next;
            }
    }
    
    return flag;
    
}

/* Find symbol with name, in all scopes -- fast approach */
bool lookup_in_all_scope(SymTable_t* symbolTable, char* name){
    int i = 0;
    bool flag = false;
    SymbolTableEntry_t* temp;
    int key = hash(symbolTable, name);
                
    /* If name exists then it will be in the row hashed the name */
    temp = symbolTable->hash[key];
    while (temp != NULL){
        if (!strcmp(temp->name, name) && temp->isActive){
            flag = true;
            return flag;
        }
        temp = temp->next;
    }
    
    
    return flag;
}

/* Print symbol table and argument lists  */
void printSymbolTable(SymTable_t* symbolTable){
    int i = 0;
    SymbolTableEntry_t* temp;
    Argument *tempArgs;
    for (i = 0; i < symbolTable->size; i++){
        temp = symbolTable->hash[i];
            while (temp != NULL){
                fprintf(stdout, "Sym Table Entry: Name: %s, Type: %s, Line of definition: %d Scope: %d isActive: %d \n", temp ->name, typeNames[temp->type], temp->lineno, temp->scope, temp->isActive );
                /*tempArgs = temp->argList;
                while (tempArgs != NULL){
                    fprintf(stdout, "Arg Table Entry: Symbol Name: %s, Type: %s, Line of definition: %d Scope: %d \n", tempArgs->name, typeNames[tempArgs->type], tempArgs->lineno, tempArgs->scope);  
                    tempArgs = tempArgs->next;
                }*/
                temp = temp ->next;
            }
     
    }
    
    
}

/* Print symbol table and argument lists  */
void printSymbolTableperscope(SymTable_t* symbolTable, int maxScope){
    int i = 0;
    SymbolTableEntry_t* temp;
	for (i = 0; i <= maxScope; i++){ 
		fprintf(stdout, "Scope: %d\n",i);
		temp = lookup_firstSymbolInScopeHidden(symbolTable, i);
            while (temp != NULL){
                fprintf(stdout, "\tName: %s, Type: %s, Line: %d Scope: %d isActive: %d \n", temp->name, typeNames[temp->type], temp->lineno, temp->scope, temp->isActive);
                temp = temp->nextinScope;
            }
			
			temp = NULL;
    }
    
    
}
/* Hide Symbols in scope */
void Hide(SymTable_t* mySymTab, int scope){
    SymbolTableEntry_t *firstSymbolInScope = lookup_firstSymbolInScope(mySymTab, scope);
    
    while(firstSymbolInScope != NULL){
        firstSymbolInScope->isActive = 0;
        firstSymbolInScope = firstSymbolInScope->nextinScope;
    }
    
}

/* Free symbol table */

void freeSymbolTable(SymTable_t* symbolTable){
    
	int i = 0;
	SymbolTableEntry_t *temp, *symbolEntry;
    Argument * tempArg, *arg;
	/* If the table is empty return */
    if (symbolTable == NULL) return;

    /* If the table has elements, free every list and then free the table itself */
    for(i = 0; i < symbolTable->size; i++) {
        symbolEntry = symbolTable->hash[i];
        while(symbolEntry != NULL) {
            temp = symbolEntry;
            symbolEntry = symbolEntry->next;
			
			arg = temp->argList;
			while (arg!= NULL){
				tempArg = arg;
				arg = arg->next;
				
				free(tempArg->name);
				free(tempArg);
			}
			
            free(temp->name);
            free(temp);
        }
    }

    free(symbolTable->hash);
    free(symbolTable);

}

