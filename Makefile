#指定编译器
CC = gcc
#指定编译参数
#CFLAGS = -std=c++11
 
#指定头文件路径，此处用于指定非标准库的头文件路径
INC = -I./ 
 
#指定要链接的动态库或静态库 -L是指定动态库路径，非标准的动态库需要此参数
#LIBS = -lm -ldl -L/mylibs/xxx -lxxx
 
#把上面放一起
CXXFLAGS = ${CFLAGS} ${INC} ${LIBS}
 
#指定生成可执行文件名称
EXEC = a.out
 
#指定由哪些文件来生成可执行文件
OBJS = iniapi.o list.o test.o
 
#make执行动作
${EXEC} : ${OBJS}
	${CC} -o $@ $^ -g
 
 
#make clean动作
clean:
	rm -rf ${OBJS} ${EXEC}
 
#中间文件的生成
%.o:%.c
	$(CC) $(CXXFLAGS) $(INC) -o $@ -c $<

