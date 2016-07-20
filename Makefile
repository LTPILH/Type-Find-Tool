# $@  目标文件
# $^  所有依赖文件
# $<  第一个依赖文件

CFLAGS=-g

tman:t_man.o t_trie.o t_sem.o t_shm.o t_foperator.o t_fbuild.o
	gcc -o tman $^

t_fbuild.o:t_fbuild.c

t_foperator.o:t_foperator.c
#	gcc -c $< -o $@

t_trie.o:t_trie.c

t_sem.o:t_sem.c

t_shm.o:t_shm.c

clean:
	rm *.o
