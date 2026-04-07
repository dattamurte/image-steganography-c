var := $(patsubst %.c, %.o, $(wildcard *.c))

a.out: $(var)
	gcc -o a.out $(var)

clean:
	rm *.o a.out