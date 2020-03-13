#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_Node {
	char *sk;
	char *sv;
	struct s_Node *s_next;
}s_Node_t;

typedef struct f_Node {
	char *fv;
	struct s_Node *shead;
	struct f_Node *f_next;
}f_Node_t;
f_Node_t *AHead = NULL;

void RemoveUnChara( char *value ) {
	char *p = strdup(value);
	int i = 0;
	int len = strlen(value);
	for( i = 0; i < len; i++ ) {
		if( p[i] == ' ' || p[i] == '\n' ) {
			continue;
		} else {
			*value = p[i];
			value++;
		}
	}
	*value = 0;
	if( p ) {
		free( p );
	}
}
void InsertF_list( f_Node_t *fnode ) {
	static f_Node_t *end = NULL;
	if( AHead == NULL ) {
		AHead = fnode;
		end = fnode;
	} else {
		end->f_next = fnode;
		end = fnode;
	}
}
f_Node_t *Getsheadbykey( const char *fv ) {
	f_Node_t *tmp = AHead;
	if( AHead == NULL ) {
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
		//stmp->sv = strdup(value);
		#if 0
		char *p = strtok(value,"=");
		if( p ) {
			stmp->sk = strdup(p);
			p = strtok(NULL,"=");
			if( p ) {
				stmp->sv = strdup(p);
			}
		}
		#endif
		InsertS_list(last.fv,stmp);
	}
}
int Init_iniFile( const char *filepath ) {
	char lbuf[1024] = {0};
	int ret = 0;
	FILE *fp = NULL;
	fp = fopen(filepath,"r");
	if( fp ) {
		while( fgets(lbuf,1024,fp) != NULL ) {
			if( lbuf[0] == '#' || lbuf[0] == ';' ) {
				continue;
			} else {
				RemoveUnChara(lbuf);
				InsertList( lbuf );
			}
		}
		fclose( fp );
	}
	return ret;
}
char *GetValue(const char *setion, const char *option ) {
	int len = strlen(setion);
	char *secbuf = malloc(len+3);
	memset(secbuf,0,len+3);
	sprintf(secbuf,"[%s]",setion);
	f_Node_t *tmp = AHead;
	char *p = NULL;
	if( AHead == NULL ) {
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
	f_Node_t *tmp = AHead;
	if( AHead == NULL ) {
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
							//strcpy(stmp->sv,value);
							break;
						}
					}
				}
			}
		}
	}
	#if 1
	if( secflag == 0 ) {
		int len = strlen(setion);
		char *secbuf = malloc(len+3);
		memset(secbuf,0,len+3);
		sprintf(secbuf,"[%s]",setion);
		f_Node_t *ftmp = (f_Node_t *)malloc(sizeof(f_Node_t));
		ftmp->fv = strdup(secbuf);
		ftmp->shead = NULL;
		ftmp->f_next = NULL;
		InsertF_list(ftmp);
		free(secbuf);
		s_Node_t *stmp = (s_Node_t *)malloc(sizeof(s_Node_t));
		stmp->sk = strdup(option);
		stmp->sv = strdup(value);
		stmp->s_next = NULL;
		InsertS_list(ftmp->fv,stmp);
		
	}
	#endif
}
void print_Ahead( void ) {
	f_Node_t *tmp = AHead;
	if( AHead == NULL ) {
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
int main(void){
	Init_iniFile("./haha.ini");
	
	char *p = GetValue("tmp1","t2");
	//printf("get = %s\n",p);
	SetValue("tmp1","t2","990808");
	
	p = GetValue("tmp1","t2");
	//printf("get = %s\n",p);
	SetValue("tmp3","t1","fsfsgas");
	print_Ahead();
	return 0;
}
