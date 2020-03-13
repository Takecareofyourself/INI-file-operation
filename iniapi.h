#ifndef _INIAPI_H_
#define _INIAPI_H_

int Init_iniFile( const char *filepath );
int Commit_File( void );
char *GetValue(const char *setion, const char *option );
void SetValue(const char *setion, const char *option, const char *value );


#endif
