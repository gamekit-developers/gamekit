#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "opts.h"
#include "version.h"

char *progname;

char *tra = "tra";
char *str;
short shr;
long loong;
int foo, bar, xflag, yflag;
char buf[80];
double d = 0.3;
struct { int xv, yv; double dv; } z;

int dblver(option *, opt_value *);
int strver(option *, opt_value *);

option options[] = {
	{ OTYPE_INT, '\0', "bar", "barvalue",
		OFLAG_NONE, &bar, 0, 0, 0 },
	{ OTYPE_DBL, 'd', "double", "dblvalue",
		OFLAG_SAVE, &d, 0, dblver, 0 },
	{ OTYPE_INT, 'f', "foo", "foovalue",
		OFLAG_SAVE, &foo, 0, 0, 0 },
	{ OTYPE_SHR, '\0', "hort", "short value",
		OFLAG_SAVE, &shr, 0, 0, 0 },
	{ OTYPE_LNG, 'l', "long", "long value",
		OFLAG_SAVE, &loong, 0, 0, 0 },
	{ OTYPE_BOL, 'x', "x", "xflag",
		OFLAG_CONST, &xflag, 0, 0, 0 },
	{ OTYPE_BOL, 'y', "y", "yflag",
		OFLAG_SAVE, &yflag, 0, 0, 0 },
	{ OTYPE_ARG, 'z', "zelda", "manyargs",
		OFLAG_SAVE, 0, 0, 0, 0 },
		{ OTYPE_INT, 'X', "z-x", "x val",
			OFLAG_SAVE | OFLAG_ARG, &z.xv,
			0, 0, 0 },
		{ OTYPE_INT, 'Y', "z-y", "y val",
			OFLAG_SAVE | OFLAG_ARG, &z.yv,
			0, 0, 0 },
		{ OTYPE_DBL, 'D', "z-d", "d val",
			OFLAG_SAVE | OFLAG_ARG, &z.dv,
			0, 0, 0 },
		{ OTYPE_NUL, '\0', "", "",
			OFLAG_SAVE | OFLAG_ARG, 0, 0, 0, 0 },
	{ OTYPE_STR, 's', "string", "strflag",
		OFLAG_DFL | OFLAG_SAVE, &str, 
		(opt_value *) &tra, strver, 0 },
	{ OTYPE_END, '\0', "",
		OFLAG_NONE, 0, 0, 0, 0 },
};

int
strver(option *op, opt_value *ov) {
	char *s = ov->v_str;
	fprintf(stderr, "strver called, op: %p [%s]\n",
		(void *) op, s ? s : "NULL string");
	return (s && *s != 'a');
}

int
dblver(option *op, opt_value *ov) {
	double dbl = ov->v_double;

	fprintf(stderr, "dblver called, op: %p [%f]\n",
		(void *) op, dbl);
	if (op->abbr != 'd')
		return 0;
	if (dbl > 25.0 || dbl < 1.0)
		return 0;
	return 1;
}

int
main(int argc, char *argv[])
{
	option *op;
	int i;

	progname = strrchr(argv[0], '/');

	optsetstyle(0);

	if (progname)
		++progname;
	else
		progname = argv[0];

	optsload(options, progname);

	printf("getopts version: %s\n", OPTS_REV);

	printf("options loaded:\n");
	for (op = &options[0]; op->type; ++op) {
		if (!((op->type == OTYPE_NUL) &&
		  (op->flags & OFLAG_ARG))) {
			printf("\t%s\n", optsprint(op));
		}
		op->flags &= ~OFLAG_SET;
	}

	if (optsgets(argc, argv, options)) {
		fprintf(stderr, "optsgets() errored.\nUsage:");
		for (op = &options[0]; op->type; ++op)
			if (!((op->type == OTYPE_NUL) &&
			  (op->flags & OFLAG_ARG)))
				fprintf(stderr, "%s\n", optsusage(op));
	}

	printf("remaining args:\n");
	for (i = optsind; i < argc; ++i) {
		printf("\t'%s'\n", argv[i]);
	}

	printf("options after getopts:\n");
	for (op = &options[0]; op->type; ++op)
		if (!((op->type == OTYPE_NUL) &&
		  (op->flags & OFLAG_ARG)))
		printf("\t%s [%s]\n", optsprint(op),
			op->flags & OFLAG_SET ? "set" : "not set");
	printf("xflag: %d\n", xflag);

	optssave(options, progname);
	return 0;
}
