#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

#define BUFFER_SIZE 4096

char *input_filename = NULL;
char *output_filename = NULL;
int debug = 0;

/* The list of long options */
static struct option long_options[] =
{
    { "output", required_argument, NULL, 'o' },
    { "debug", no_argument, NULL, 'd' },
    { "version", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { NULL, no_argument, NULL, '\0' }
};

#if 0
/* Print the production rule */
static void Callback(void *debug, Grammar grammar)
{
    if (debug != NULL)
    {
	Grammar_printKernels(grammar, stdout);
    }
    else
    {
	Grammar_printTable(grammar, stdout);
    }
}
#endif

/* Prints out the usage message */
static void usage(int argc, char *argv[])
{
    fprintf(stderr, "usage: %s [OPTION]... [FILE]\n", argv[0]);
    fprintf(stderr, "  -o file,     --output=file\n");
    fprintf(stderr, "  -d,          --debug\n");
    fprintf(stderr, "  -v,          --version\n");
    fprintf(stderr, "  -h,          --help\n");
}


/* Parse args and go */
int main(int argc, char *argv[])
{
    parser_t parser;
    int choice;
    int fd;

    /* Read options from the command line */
    while ((choice = getopt_long(argc, argv, "o:dvh", long_options, NULL)) > 0)
    {
	switch (choice)
	{
	    /* --output or -o */
	    case 'o':
	    {
		output_filename = optarg;
		break;
	    }

	    /* --debug or -d */
	    case 'd':
	    {
		debug = 1;
		break;
	    }

	    /* --version or -v */
	    case 'v':
	    {
		printf("%s version %s\n", PACKAGE, VERSION);
		exit(0);
	    }

	    /* --help or -h */
	    case 'h':
	    {
		usage(argc, argv);
		exit(0);
	    }

	    /* bogus option */
	    default:
	    {
		usage(argc, argv);
		exit(1);
	    }
	}
    }

    /* Look for an input file name */
    if (optind < argc)
    {
	input_filename = argv[optind++];
    }

    /* Make sure we don't have any extra args */
    if (optind < argc)
    {
	usage(argc, argv);
	exit(1);
    }
    
    /* Create the parser */
    if ((parser = parser_alloc(NULL, NULL)) == NULL)
    {
	perror("parser_alloc(): failed");
	exit(1);
    }

    /* Open up the input file */
    if (input_filename != NULL)
    {
	if ((fd = open(input_filename, O_RDONLY)) < 0)
	{
	    perror("open(): failed");
	    exit(1);
	}
    }
    else
    {
	fd = STDIN_FILENO;
    }

    /* Read characters from stdin and give them to the Lexer */
    while (1)
    {
	char buffer[BUFFER_SIZE];
	ssize_t length;

	if ((length = read(fd, buffer, 2048)) < 0)
	{
	    perror("read(): failed");
	    abort();
	}

	parser_parse(parser, buffer, length);

	if (length == 0)
	{
	    close(fd);
	    return 0;
	}
    }
}
