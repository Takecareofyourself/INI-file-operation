#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

#define BUF_MAX 512

int Init_iniFile( const char *filepath ) {
	char lbuf[BUF_MAX] = {0};
	char dst[BUF_MAX] = {0};
	int ret = 0;
	FILE *fp = NULL;
	fp = fopen(filepath,"r");
	if( fp ) {
		Init_lini(filepath);
		while( fgets(lbuf,BUF_MAX,fp) != NULL ) {
			RemoveUnChara(lbuf,dst);
			if( dst[0] == '#' || dst[0] == ';' || dst[0] == 0 ) {
				continue;
			} else {
				InsertList( dst );
			}
		}
		fclose( fp );
	}
	return ret;
}
int Commit_File( void ) {
	CommitFile();
}
char *GetValue(const char *setion, const char *option ) {
	int len = strlen(setion);
	char *secbuf = malloc(len+3);
	memset(secbuf,0,len+3);
	sprintf(secbuf,"[%s]",setion);
	f_Node_t *tmp = GetHead();
	char *p = NULL;
	if( tmp == NULL ) {
		printf("Ahead is nil\n");
	} else {
		for( ;tmp != NULL;tmp = tmp->f_next ) {
			if( strcmp(secbuf,tmp->fv) == 0 ) {
				if( tmp->shead != NULL ) {
					s_Node_t *stmp = tmp->shead;
					for(; stmp!=NULL; stmp = stmp->s_next ) {
						if( strcmp(option,stmp->sk) == 0 ) {
							p = stmp->sv;
							break;
						}
					}
				}
			}
		}
	}
	free(secbuf);
	return p;
}
void SetValue(const char *setion, const char *option, const char *value ) {
	int secflag = 0;
	int len = strlen(setion);
	char *secbuf = malloc(len+3);
	memset(secbuf,0,len+3);
	sprintf(secbuf,"[%s]",setion);
	f_Node_t *tmp = GetHead();
	if( tmp == NULL ) {
		printf("Ahead is nil\n");
	} else {
		for( ;tmp != NULL;tmp = tmp->f_next ) {
			if( strcmp(secbuf,tmp->fv) == 0 ) {
				secflag = 1;
				if( tmp->shead != NULL ) {
					s_Node_t *stmp = tmp->shead;
					for(; stmp!=NULL; stmp = stmp->s_next ) {
						if( strcmp(option,stmp->sk) == 0 ) {
							if(stmp->sv) free(stmp->sv);
							stmp->sv = strdup(value);
							stmp->in = 1;
							//strcpy(stmp->sv,value);
							break;
						}
					}
				}
			}
		}
	}

	if( secflag == 0 ) {
		f_Node_t *ftmp = (f_Node_t *)malloc(sizeof(f_Node_t));
		ftmp->fv = strdup(secbuf);
		ftmp->in = 1;
		ftmp->shead = NULL;
		ftmp->f_next = NULL;
		InsertF_list(ftmp);
	
		s_Node_t *stmp = (s_Node_t *)malloc(sizeof(s_Node_t));
		stmp->sk = strdup(option);
		stmp->sv = strdup(value);
		stmp->in = 1;
		stmp->s_next = NULL;
		InsertS_list(ftmp->fv,stmp);
	}
	free(secbuf);
}
void print_Ahead( void ) {
	f_Node_t *tmp = GetHead();
	if( tmp == NULL ) {
		printf("Ahead is nil\n");
	} else {
		for( ;tmp != NULL;tmp = tmp->f_next ) {
			printf("%s\n",tmp->fv);
			if( tmp->shead != NULL ) {
				s_Node_t *stmp = tmp->shead;
				for(; stmp!=NULL; stmp = stmp->s_next ) {
					printf("%s = %s\n",stmp->sk,stmp->sv);
				}
			}
		}
	}
}

