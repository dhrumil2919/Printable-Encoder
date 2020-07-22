all:	clean filter encoder
filter : Filter/filter.c
	gcc -m32 -fno-stack-protector -z execstack Filter/filter.c -o filter -g
encoder : encoder_c/encoder.c
	gcc encoder_c/encoder.c -o encoder
clean:
	rm -rf filter encoder
