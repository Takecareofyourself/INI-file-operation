#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

typedef struct ini {
	char *filepath[64];
	f_Node_t *Ahead;
}ini_t;

static ini_t lini;

static f_Node_t *AHead = NULL;
void Init_lini( const char *file ) {
	memcpy(lini.filepath,file,64);
	lini.Ahead = NULL;
}

f_Node_t * GetHead( void ) {
	return lini.Ahead ;
}

f_Node_t *Getsheadbykey( const char *fv ) {
	f_Node_t *tmp = GetHead();
	if( tmp == NULL ) {
		return NULL;
	} else {
		for(;tmp!= NULL; tmp = tmp->f_next) {
			if( strcmp(tmp->fv,fv) == 0 ) {
				break;
			}
		}
	}
	return tmp;
}

void PareseKeyAndValue(s_Node_t *stmp,char *value) {
	char *p = strtok(value,"=");
	if( p ) {
		stmp->sk = strdup(p);
		p = strtok(NULL,"=");
		if( p ) {
			stmp->sv = strdup(p);
		}
	}
}

void InsertF_list( f_Node_t *fnode ) {
	static f_Node_t *end = NULL;
	//f_Node_t *head = GetHead();
	if( lini.Ahead == NULL ) {
		lini.Ahead = fnode;
		end = fnode;
	} else {
		end->f_next = fnode;
		end = fnode;
	}
}

void InsertS_list( const char *fv, s_Node_t *snode ) {
	static s_Node_t *end = NULL;
	f_Node_t *fnode = Getsheadbykey( fv );
	if( fnode == NULL ) {
		printf("%s can't find\n",fv);
		return;
	} else {
		if( fnode->shead == NULL ) {
			fnode->shead = snode;
			//snode->s_next = NULL;
			end = snode;
		} else {
			end->s_next = snode;
			end = snode;
			//end->s_next = NULL;
		}
	}
}

void InsertList( char *value ) {
	static f_Node_t last;
	f_Node_t *ftmp = NULL;
	s_Node_t *stmp = NULL;
	
	int len = strlen(value);
	if( value[0] == '[' && value[len-1] == ']' ) {
		ftmp = (f_Node_t *)malloc(sizeof(f_Node_t));
		ftmp->fv = strdup(value);
		ftmp->shead = NULL;
		ftmp->f_next = NULL;
		InsertF_list(ftmp);
		last = *ftmp;
	} else {
		stmp = (s_Node_t *)malloc(sizeof(s_Node_t));
		stmp->s_next = NULL;
		PareseKeyAndValue(stmp,value);
		InsertS_list(last.fv,stmp);
	}
}

void CommitFile( void ) {

}

void ListFree( void ) {
	if( lini.Ahead != NULL ) {
		f_Node_t *tmp = lini.Ahead;
		if( tmp == NULL ) {
			printf("Ahead is nil\n");
		} else {
			for( ;tmp != NULL;tmp = tmp->f_next ) {
				printf("%s\n",tmp->fv);
				if( tmp->shead != NULL ) {
					s_Node_t *stmp = tmp->shead;
					for(; stmp!=NULL; stmp = stmp->s_next ) {
						printf("%s = %s\n",stmp->sk,stmp->sv);
						free(stmp->sk);
						free(stmp->sv);
						free(stmp);
					}
				}
				free(tmp->fv);
				free(tmp);
			}
		}
		lini.Ahead = NULL;
	}
}
