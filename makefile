#makefile for homework5

homework5: homework5.c
	gcc -o homework5 homework5.c

clean:
	rm homework5

tar:
	tar -cf homework5.tar homework5.c makefile
