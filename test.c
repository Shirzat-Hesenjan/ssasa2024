/* Borrow from https://github.com/euugenechou/sha3-256 to do basic IO for the programme and print test results*/
/* include the reference implementation*/
#include "reference/sha3.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdarg.h>

/* this is my implementation*/
#include "sha3.h"


bool verbose = false;

void output(char *fmt, ...)
{
    if (verbose)
    {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
    }
}

void check(bool cond, char *fmt, ...)
{
    if (!cond)
    {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
        exit(EXIT_FAILURE);
    }
}

void hexprint(uint64_t nbytes, uint8_t bytes[static nbytes])
{
    for (uint64_t i = 0; i < nbytes; i += 1)
    {
        output("%02" PRIx8, bytes[i]);
    }
    output("\n");
}

/*This is the method to call the reference implementation using `hashfile`*/
void hash_file(int64_t length, FILE *infile, FILE *outfile) {
    uint8_t *msg = (uint8_t *)calloc(length ? length : 1, sizeof(uint8_t));
    check(msg, "Failed to allocate array for message.\n");

    output("Len = %" PRId64 "\n", length);
    fread(msg, sizeof(uint8_t), length, infile);

    output("Msg = ");
    hexprint(length, msg);

    uint8_t md[SHA3_256_MD_LEN] = { 0 };
    // sha3_256_digest(msg, length, md); TODO let Makefile compile reference implementation

    output("MD = ");
    hexprint(SHA3_256_MD_LEN, md);

    fwrite(md, sizeof(uint8_t), SHA3_256_MD_LEN, outfile);
    free(msg);
}


void print_usage(char **argv)
{
    fprintf(stderr,
            "SYNOPSIS\n"
            "   Produce hash-codes for input bytes using the SHA-3 256 algorithm.\n"
            "\n"
            "USAGE\n"
            "   %s [-hv] -l length [-i input] [-o output]\n"
            "\n"
            "OPTIONS\n"
            "   -h          Display program help and usage\n"
            "   -v          Display verbose program output\n"
            "   -l length   Length of input in bytes\n"
            "   -i input    Specify input to hash (stdin by default)\n"
            "   -o output   Specify output of hashed input (stdout by default)\n",
            argv[0]);
}

#define OPTIONS "hvl:i:o"

int main(int argc, char **argv)
{
    int opt = 0;
    int64_t length = -1;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_usage(argv);
            return 0;
        case 'v':
            verbose = true;
            break;
        case 'l':
            length = (int64_t)strtoll(optarg, NULL, 10);
            break;
        case 'i':
            infile = fopen(optarg, "rb");
            check(infile, "Failed to open %s.\n", optarg);
            break;
        case 'o':
            outfile = fopen(optarg, "wb");
            check(outfile, "Failed to open %s.\n", optarg);
            break;
        default:
            print_usage(argv);
            return 1;
        }
    }

    check(length >= 0, "Valid message length must be supplied.\n");

    hash_file(length, infile, outfile);

    fclose(infile);
    fclose(outfile);
    return 0;
}
