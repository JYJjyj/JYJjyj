.PHONY:clean

mycp:mycp.o
	gcc $^ -o $@
%.o:%.c
	gcc -c $^ -o $@
clean:
	rm -rf *.o mycp
