#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opts.h"

char *progname;

option options[8];

int
main(int argc, char *argv[])
{
	int i, c;
	option *op;

	progname = strrchr(argv[0], '/');

	if (progname)
		++progname;
	else
		progname = argv[0];

	while ((c = optsopt(argc, argv, "b:cd#")) != -1) {
		printf("op: '%c', '%s'\n", c, optsarg ? optsarg : "N/A");
		if (c == 'd')
			printf("\toptsval.v_int: %d\n", optsval.v_int);
		if (c == '?') {
			printf("\terror; '%s' not an option.\n", argv[optsind]);
			break;
		}
	}

	printf("optsind: %d\n", optsind);
	printf("remaining args:\n");
	for (i = optsind; i < argc; ++i) {
		printf("\t'%s'\n", argv[i]);
	}

	op = optsmake("b:cd#");
	while ((c = optsget(argc, argv, op)) != -1) {
		printf("op: '%c', '%s'\n", c, optsarg ? optsarg : "N/A");
		if (c == 'd')
			printf("\toptsval.v_int: %d\n", optsval.v_int);
		if (c == '?') {
			printf("\terror; '%s' not an option.\n", argv[optsind]);
			break;
		}
	}

	printf("optsind: %d\n", optsind);
	printf("remaining args:\n");
	for (i = optsind; i < argc; ++i) {
		printf("\t'%s'\n", argv[i]);
	}

	return 0;
}
