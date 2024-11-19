#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <math.h>

typedef double (*mathematical_function_t)(double);

typedef struct {
	double An, Bn;
} fcoeff_t;

int n_term = 100;
static int verbose, ognuplot;
double ll, ul, precision = 0.001;
char *flib_path = NULL, *progname = NULL, *output = "text";
bool is_limit_exists = false;

void usage(const char* note)
{
	fputs(note, stderr);
	fprintf(stderr, "%s: [OPTION]... [-a] [-b] LIB_PATH\n", progname);
	fputs("-h, --help              Getting help\n", stderr);
	fputs("-a, --lower_limit       Lower integral limit\n", stderr);
	fputs("-b, --upper_limit       Upper integral limit\n", stderr);
	fputs("-v, --verbose           Show more details\n", stderr);
	fputs("-p, --precision         Change precision of result\n", stderr);
	fputs("-n, --nterm             Specify N in discret sum\n", stderr);
	fputs("-g, --gnuplot           Generate gnuplot syntax output\n", stderr);

	exit(EXIT_FAILURE);
}

void parse_arguments(int argc, char **argv)
{
	int  c;
	const char *short_options;

	progname = argv[0];

	short_options = "a:b:hpn:";
	static struct option long_options[] = {
		{"verbose", no_argument, &verbose, 1},
		{"gnuplot", no_argument, &ognuplot, 1},
		{"lower_limit", required_argument, 0, 'a'},
		{"upper_limit", required_argument, 0, 'b'},
		{"precision", required_argument, 0, 'p'},
		{"nterm", required_argument, 0, 'n'},

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

		case 'n':
			n_term = atof(optarg);
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

fcoeff_t calculate_fourier_coefficient(mathematical_function_t f, double L, int N)
{
	fcoeff_t fc = {};
	for (double x = ll; x <= ul; x += precision)
		fc.An += f(x) * cos(N * M_PI * x / L);
	fc.An *= 1/L;

	for (double x = ll; x <= ul; x += precision)
		fc.An += f(x) * sin(N * M_PI * x / L);
	fc.Bn *= 1/L;

	return fc;
}

int main(int argc, char **argv)
{
	void* dlobj = NULL;
	mathematical_function_t f = NULL;

	parse_arguments(argc, argv);

	if (!(dlobj = dlopen(flib_path, RTLD_LAZY))) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	if (!(f = dlsym(dlobj, "f"))) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	if (ognuplot)
		printf("plot 0");

	for (int n = 0; n < n_term; n++) {
		double L = (ul - ll)/2;

		fcoeff_t c = calculate_fourier_coefficient(f, L, n);

		if (ognuplot)
			printf("+%f*cos(x*%d*pi/%f)+%f*sin(x*%d*pi/%f)", c.An, n, L, c.Bn, n, L);
		else
			printf("A%d = %f, B%d = %f\n", n, c.An, n, c.Bn);
	}

	if (ognuplot)
		printf(" title \"Fourier serie\"\n");

	return (EXIT_SUCCESS);
}
