#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <math.h>

double precision = 0.00001;
char *progname = NULL;

double ll, ul;
char *flib_path = NULL;
bool is_limit_exists = false;
static int verbose;


void usage(const char* note)
{

	fputs(note, stderr);
	fprintf(stderr, "%s: [OPTION]... [-a] [-b] LIB_PATH\n", progname);
	fputs("-h, --help              Getting help\n", stderr);
	fputs("-a, --lower_limit       Lower integral limit\n", stderr);
	fputs("-b, --upper_limit       Upper integral limit\n", stderr);
	fputs("-v, --verbose           Show more details\n", stderr);
	fputs("-p, --precision         Change precision of result\n", stderr);

	exit(EXIT_FAILURE);
}

void parse_arguments(int argc, char **argv)
{
	int  c;

	progname = argv[0];

	const char *short_options = "a:b:hp";
	static struct option long_options[] = {
		{"verbose", no_argument, &verbose, 'v'},
		{"lower_limit", required_argument, 0, 'a'},
		{"upper_limit", required_argument, 0, 'b'},
		{"precision", required_argument, 0, 'p'},

		{"help", required_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
		case 'a':
			ll = atof(optarg);
			is_limit_exists = true;
			break;

		case 'b':
			ul = atof(optarg);
			is_limit_exists = true;
			break;

		case 'p':
			precision = atof(optarg);
			break;

		case 'h':
			usage("");
			break;

		case '?':
			fprintf(stderr, "Try '%s --help' for more information.", progname);
			exit(EXIT_FAILURE);
		}
	}

	argc -= optind;
	argv += optind;

	flib_path = argv[0];

	if (!is_limit_exists)
		usage("-a/-b is required. you should pass least one of limits!\n");

	if (!flib_path)
		usage("You should pass LIB_PATH\n");
}

int main(int argc, char **argv)
{
	parse_arguments(argc, argv);

	return (EXIT_SUCCESS);
}
