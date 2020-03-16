#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

typedef struct ini {
	char filepath[64];
	f_Node_t *Ahead;
}ini_t;

static ini_t lini;

static f_Node_t *AHead = NULL;

void RemoveUnChara( char *src ,char *dst) {
	int i = 0;
	int len = strlen(src);
	for( i = 0; i < len; i++ ) {
		if( src[i] == ' ' || src[i] == '\n' ) {
			continue;
		} else {
			*dst = src[i];
			dst++;
		}
	}
	*dst = 0;
}


void Init_lini( const char *file ) {
	strcpy(lini.filepath,file);
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
		ftmp->in = 0;
		ftmp->shead = NULL;
		ftmp->f_next = NULL;
		InsertF_list(ftmp);
		last = *ftmp;
	} else {
		stmp = (s_Node_t *)malloc(sizeof(s_Node_t));
		stmp->sk = NULL;
		stmp->sv = NULL;
		stmp->s_next = NULL;
		stmp->in = 0;
		PareseKeyAndValue(stmp,value);
		InsertS_list(last.fv,stmp);
	}
}
void ListFree( void ) {
	if( lini.Ahead != NULL ) {
		f_Node_t *tmp = lini.Ahead;
		if( tmp == NULL ) {
			printf("Ahead is nil\n");
		} else {
			for( ;tmp != NULL;tmp = tmp->f_next ) {
				printf("free(%s)\n",tmp->fv);
				if( tmp->shead != NULL ) {
					s_Node_t *stmp = tmp->shead;
					for(; stmp!=NULL; stmp = stmp->s_next ) {
						printf("free( %s = %s )\n",stmp->sk,stmp->sv);
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

void CommitFile( void ) {
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	char filepathtmp[64] = {0};
	char buf[256] = {0};
	char dst[256] = {0};
	int len = 0;
	
	s_Node_t *sstmp = NULL;
	f_Node_t * tmp = NULL;
	s_Node_t *stmp = NULL;
	
	sprintf(filepathtmp,"%s.tmp",lini.filepath);
	fp1 = fopen(lini.filepath,"r");
	fp2 = fopen(filepathtmp,"w");
	if( fp1 && fp2 ) {
		while( fgets(buf,256,fp1) != NULL ) {
			RemoveUnChara(buf,dst);
			len = strlen(dst);
			if( dst[0] == ';' || dst[0] == '#' ) {
				fprintf(fp2,"%s",buf);
			} else {
				if( dst[0] == '[' && dst[len-1] == ']' ) {	
					for(tmp = lini.Ahead; tmp != NULL; tmp = tmp->f_next ) {
						if( strcmp(tmp->fv,dst) == 0 ) {
							fprintf(fp2,"%s",buf);
							sstmp = tmp->shead;
							break;
						}
					}
				} else {
					int change = 0;
					for( stmp = sstmp; stmp != NULL; stmp = stmp->s_next ){
						if( stmp->in == 1 && strncmp(stmp->sk,dst,strlen(stmp->sk)) == 0 ) {
							fprintf(fp2,"%s = %s\n",stmp->sk,stmp->sv);	
							change = 1;
							break;
						} else if( stmp->in == 2 ) {
							fprintf(fp2,"%s = %s\n",stmp->sk,stmp->sv);	
							stmp->in = 0;
							break;
						}
					}
					if( change == 0 )
						fprintf(fp2,"%s",buf);
				}
			}
		}
		
		for (tmp = lini.Ahead; tmp != NULL; tmp = tmp->f_next ) {
			if( tmp->in == 1 ) {
				fprintf(fp2,"%s\n",tmp->fv);
				for(stmp = tmp->shead; stmp != NULL; stmp = stmp->s_next ){
					fprintf(fp2,"%s = %s\n",stmp->sk,stmp->sv);
				}
			}
		}
		ListFree();
		fclose(fp1);
		fclose(fp2);
		rename(filepathtmp,lini.filepath);
	}
}


