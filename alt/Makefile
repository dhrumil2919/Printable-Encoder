all:	clean tester
	
tester:	tester.c
	gcc -m32 -fno-stack-protector -z execstack tester.c -o tester -g
clean:
	rm -rf tester
