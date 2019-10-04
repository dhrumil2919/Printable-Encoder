all:	clean filter
filter : filter.c
	gcc -m32 -fno-stack-protector -z execstack filter.c -o filter -g
clean:
	rm -rf filter
