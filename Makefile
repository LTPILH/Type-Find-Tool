# $@  目标文件
# $^  所有依赖文件
# $<  第一个依赖文件

CFLAGS=-g

tyfd:tyfd.o t_trie.o t_sem.o t_shm.o t_foperator.o t_fbuild.o t_wordparse.o
	gcc -o tyfd $^

tyfd.o:tyfd.c

t_wordparse.o:t_wordparse.c

t_fbuild.o:t_fbuild.c

t_foperator.o:t_foperator.c
#	gcc -c $< -o $@

t_trie.o:t_trie.c

t_sem.o:t_sem.c

t_shm.o:t_shm.c

clean:
	rm *.o
