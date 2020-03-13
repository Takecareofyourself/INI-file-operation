#ifndef __LIST_H_
#define __LIST_H_

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



f_Node_t * GetHead( void );
void InsertS_list( const char *fv, s_Node_t *snode );
void InsertList( char *value );
void InsertF_list( f_Node_t *fnode );
void Init_lini(const char *);
void ListFree( void );


#endif
