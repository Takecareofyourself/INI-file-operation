#include "iniapi.h"

int main(void){
	Init_iniFile("./haha.ini");
	
	char *p = GetValue("tmp1","t2");
	//printf("get = %s\n",p);
	SetValue("tmp1","t2","990808");
	SetValue("tmp1","t3","11355");
	SetValue("tmp2","t1","1231344");
	p = GetValue("tmp1","t2");
	//printf("get = %s\n",p);
	SetValue("tmp3","t1","fsfsgas");
	//print_Ahead();
	Commit_File();
	return 0;
}

