CC = gcc

all: filter encoder tester

filter: filter.c
	$(CC) -m32 -g -fno-stack-protector -z execstack filter.c -o filter

encoder: encoder.c
	$(CC) encoder.c -o encoder

tester:	tester.c
	$(CC) -m32 -g -fno-stack-protector -z execstack tester.c -o tester

.Phony: clean
clean:
	rm -f tester filter encoder
