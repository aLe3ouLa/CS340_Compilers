#include <assert.h>

#include "fun.h"
#include "alpha_vm.h"

extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern unsigned top;
extern unsigned topsp;
extern avm_memcell stack[AVM_STACKSIZE];
extern unsigned char executionFinished;
extern unsigned pc;



void execute_NEWTABLE(instruction* instr){
	avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*) 0);
	//assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && &stack[top] <= lv || lv == &retval));
	
	avm_memcellclear(lv);
	
	lv->type = table_m;
	lv->data.tableVal = avm_tablenew();
	avm_tableincrefcounter(lv->data.tableVal);
	
}

int hashKey (avm_memcell* index){
	unsigned i;
	
	switch (index->type){
		case string_m :{
			unsigned long int hashval = 0;
			int i = 0;
			char * name = strdup( index->data.strVal);
			//printf("name: %s\n", name);
			if (name[0] != '\"'){
				sprintf(name, "\"%s\"",index->data.strVal);
			}
			//printf("newname: %s\n", name);
				/* Convert our string to an integer */
			while( hashval < 100000 && i < strlen(name)) {
				hashval = hashval << 8;
				hashval += name[i];
				i++;
			} 
			return hashval % AVM_TABLE_HASHSIZE;
		}
		case number_m: {
			double db = index->data.numVal;
			unsigned ub = (unsigned)db + 1;
			return ub%AVM_TABLE_HASHSIZE;
		}	
		case bool_m:{
			return (int)index->data.boolVal%AVM_TABLE_HASHSIZE;
		}
		case libfunc_m :{
			unsigned long int hashval = 0;
			int i = 0;

				/* Convert our string to an integer */
			while( hashval < 100000 && i < strlen( index->data.strVal)) {
				hashval = hashval << 8;
				hashval += index->data.strVal[i];
				i++;
			} 
			return hashval % AVM_TABLE_HASHSIZE;
		}
		case userfunc_m:{
			return (int)index->data.funcVal%AVM_TABLE_HASHSIZE;
		}
		case table_m:{
			return (int)index->data.tableVal->total%AVM_TABLE_HASHSIZE;
		}
		
	}
	
}

avm_memcell* avm_tablegetelem(avm_table * table, avm_memcell* index){
	
	int hash = hashKey(index);
	//printf("Hashing with: %d, index->type: %d\n", hash, index->type);
	
	switch (index->type){
		case string_m: {
			int i;
			
			char * name = strdup( index->data.strVal);
			//printf("name: %s\n", name);
			if (name[0] != '\"'){
				sprintf(name, "\"%s\"",index->data.strVal);
			}
			//printf("newname: %s\n", name);
			//printf("table total: %d", table->total);
			avm_table_bucket* temp = table->strIndexed[hash];
			if (temp == NULL){
				return NULL;
			}else{
				while (temp != NULL){
						//printf("temp->key.data.strVal: %s | name: %s\n", temp->key.data.strVal, name);
						if (!strcmp(temp->key.data.strVal, name)){
							//printf("Bingo!\n");
							return &table->strIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		}
		case number_m: {
			avm_table_bucket* temp = table->numIndexed[hash];
			if ( temp == NULL){
				return NULL;
				avm_error("Error in table get element");
			}else{
				while (temp != NULL){
						if (temp->key.data.numVal == index->data.numVal){
							//printf("Content: %p\n", table->numIndexed[hash]->value.data.tableVal);
								return &table->numIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		}
		case bool_m: {
			avm_table_bucket* temp = table->boolIndexed[hash];
			if ( temp == NULL){
				return NULL;
				avm_error("Error in table get element");
			}else{
				while (temp != NULL){
						if (temp->key.data.boolVal == index->data.boolVal ){
								return &table->boolIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		}	
		case libfunc_m:{
			avm_table_bucket* temp = table->libFuncIndexed[hash];
			if ( temp == NULL){
				return NULL;
				avm_error("Error in table get element");
			}else{
				while (temp != NULL){
						if (!strcmp(temp->key.data.libfuncVal, index->data.libfuncVal)){
								return &table->libFuncIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		
		}		
		case userfunc_m: {
			avm_table_bucket* temp = table->userFuncIndexed[hash];
			if ( temp == NULL){
				return NULL;
				avm_error("Error in table get element");
			}else{
				while (temp != NULL){
					if (temp->key.data.funcVal == index->data.funcVal ){
								return &table->userFuncIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		}	
		case table_m: {
			avm_table_bucket* temp = table->tableIndexed[hash];
			if ( temp == NULL){
				return NULL;
				avm_error("Error in table get element");
			}else{
				while (temp != NULL){
						if (temp->key.data.tableVal == index->data.tableVal){
								//printf("into hash:key: %lf val: %lf \n",  index->data.numVal, table->numIndexed[hash]->value.data.numVal );
								return &table->tableIndexed[hash]->value;
						}
						temp = temp->next;
				}
				return NULL;
			}
			break;
		}
	}
	
}

void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content){
	int hash = hashKey(index);
	//printf("Hashing with: %d, index->type: %d", hash, index->type);
	//printf("Content: %p \n", content->data.tableVal);
	switch (index->type){
		case number_m: {
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			if (table->numIndexed[hash] == NULL){
				table->numIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table->numIndexed[hash];
				table->numIndexed[hash] = newBucket;
			}
			table->total++;
			break;			
		}
		case string_m:{
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			//printf("\n%s\n", newBucket->key.data.strVal);
			if (table->strIndexed[hash] == NULL){
				table->strIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table->strIndexed[hash];
				table->strIndexed[hash] = newBucket;
			}
			table->total++;
			break;			
		}
		case bool_m:{
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			if (table->boolIndexed[hash] == NULL){
				table->boolIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table->boolIndexed[hash];
				table->boolIndexed[hash] = newBucket;
			}
			table->total++;
			break;
		}
		case libfunc_m:{
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			if (table->libFuncIndexed[hash] == NULL){
				table->libFuncIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table->libFuncIndexed[hash];
				table->libFuncIndexed[hash] = newBucket;
			}
			table->total++;
			break;
		}
		case userfunc_m:{
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			if (table->userFuncIndexed[hash] == NULL){
				table->userFuncIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table-> userFuncIndexed[hash];
				table-> userFuncIndexed[hash] = newBucket;
			}
			table->total++;
			break;
		}
		case table_m:{
			avm_table_bucket* newBucket = malloc(sizeof (avm_table_bucket));
			newBucket->key = *index;
			newBucket->value = *content;
			if (table->tableIndexed[hash] == NULL){
				table->tableIndexed[hash] = newBucket;
				newBucket->next = NULL;
			}else{
				/* If bucket is not empty, insert it in the beggining */
				newBucket->next = table->tableIndexed[hash];
				table->tableIndexed[hash] = newBucket;
			}
			table->total++;
			break;
		}
	}
}

void execute_TABLEGETELEM(instruction* instr){
	avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*) 0);
	avm_memcell* t = avm_translate_operand(instr->arg1, (avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(instr->arg2, &ax);

	//assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv &&  &stack[top] < lv || lv == &retval));
	assert(t && &stack[AVM_STACKSIZE - 1] >= t && &stack[top] < t );
	assert(i);
	
	if (t->type != table_m) {
		avm_error("TABLEGETELEM:illegal use of type as table!");
	}else{
		avm_memcell* content = malloc(sizeof(avm_memcell));
		content = avm_tablegetelem(t->data.tableVal, i);
		if (content) {
			avm_memcellclear(lv);
			lv->type = nil_m; /*Default value.*/
			avm_assign(lv, content);
		}else{
			//avm_memcellclear(lv);
			//lv->type = nil_m; /*Default value.*/
			avm_warning("Element not found!");
		}
	}
	
}

void execute_TABLESETELEM(instruction* instr){
	
	//printf("execute_SETELEM\n");
	avm_memcell* t = avm_translate_operand(instr->result, (avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* c = avm_translate_operand(instr->arg2, &bx);

	//assert(t && &stack[AVM_STACKSIZE - 1] >= t && &stack[top] < t);
	assert(i && c);
	
	if (t->type != table_m)
		avm_error("TABLESETELEM:illegal use of type as table!");
	else
		avm_tablesetelem(t->data.tableVal, i, c);
	
}
