#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
double ll, ul, precision = 0.001;
char *flib_path = NULL, *progname = NULL, *output = "text";
bool is_limit_exists = false, verbose = false, ognuplot = false, dump_terminal = false;

void usage(const char* note)
{
	fputs(note, stderr);
	fprintf(stderr, "%s: [OPTION]... [-a] [-b] LIB_PATH\n", progname);
	fputs("-h       Getting help\n", stderr);
	fputs("-a       Lower integral limit\n", stderr);
	fputs("-b       Upper integral limit\n", stderr);
	fputs("-v       Show more details\n", stderr);
	fputs("-p       Change precision of result\n", stderr);
	fputs("-n       Specify N in discret sum\n", stderr);
	fputs("-g       Generate gnuplot syntax output\n", stderr);
	fputs("-d       Show output in terminal (dump terminal)\n", stderr);

	exit(EXIT_FAILURE);
}

void parse_arguments(int argc, char **argv)
{
	int  c;

	progname = argv[0];
	while ((c = getopt(argc, argv, "vgda:b:hp:n:")) != -1) {
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

		case 'v':
			verbose = true;
			break;


		case 'g':
			ognuplot = true;
			break;


		case 'd':
			dump_terminal = true;
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

	if (dump_terminal)
		ognuplot = 1;
}

fcoeff_t calculate_fourier_coefficient(mathematical_function_t f, double L, int N)
{
	fcoeff_t fc = {};
	for (double x = ll; x <= ul; x += precision)
		fc.An += (f(x) * cos(N * M_PI * x / L)) * precision;
	fc.An *= 1/L;

	for (double x = ll; x <= ul; x += precision)
		fc.Bn += (f(x) * sin(N * M_PI * x / L)) * precision;
	fc.Bn *= 1/L;

	fc.An = (signed long)(fc.An * 100000) * 0.00001f;
	fc.Bn = (signed long)(fc.Bn * 100000) * 0.00001f;

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

	if (dump_terminal)
		printf("set terminal dumb;");

	if (ognuplot)
		printf("plot 0");

	for (int n = 0; n < n_term; n++) {
		double L = (ul - ll)/2;

		fcoeff_t c = calculate_fourier_coefficient(f, L, n);

		if (ognuplot) {
			printf(c.An ? "+%f*cos(x*%d*pi/%f)" : "", c.An, n, L);
			printf(c.Bn ? "+%f*sin(x*%d*pi/%f)" : "", c.Bn, n, L);
		} else {
			printf(c.An ? "A%d = %f" : "", n, c.An);
			printf(c.An && c.Bn ? ", " : "");
			printf(c.Bn ? "B%d = %f" : "", n, c.Bn);
			printf(c.An || c.Bn ? "\n" : "");
		}
	}

	if (ognuplot)
		printf(" title \"Fourier serie\"\n");

	return (EXIT_SUCCESS);
}
