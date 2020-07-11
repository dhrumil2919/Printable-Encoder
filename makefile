all:	clean filter encoder
filter : filter.c
	gcc -m32 -fno-stack-protector execstack Filter/filter.c -o filter -g
encoder : encoder.c
	gcc encoder_c/encoder.c -o encoder
clean:
	rm -rf filter encoder
