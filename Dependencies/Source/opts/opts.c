/* This software is Copyright 1995-1998 Peter Seebach.  Please see the
 * associated file "COPYRIGHT" for copyright information, or write
 * seebs@plethora.net
 */
#include "opts.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char **oldargv = NULL;
static option *opts = NULL;
static char *last_optstring = NULL;
char *optsarg;
opt_value optsval;
option optsdummy;
int optsind = 1, optserr = 1;
int optsok = 1;
static enum { STYLE_POSIX, STYLE_TAR } optstyle;
static int curind = 1;
static int curloc = 0;
static size_t curlen = 0;
static int sense = 1;
static int ac;
static char **av;
static int cleanflag = 0;
static char *getarg(int, char**);

#define C(ptr, type) (*((type *) (ptr)))
#define P(ptr, type) ((type *) (ptr))

static char *typenames[] = {
	"end of options",
	"flag",
	"flag",
	"short integer",
	"integer",
	"long integer",
	"string",
	"buffer",
	"real",
	"multiple arguments",
	"invalid type"
};

static int debug_flag = 0;

static option *incr_opt(void);
static int op_set(option *, char *);
static int optgetopt(int, char **);

static void warn(char *, ...);
static void error(char *, ...);
static void debug(char *, ...);

static void clean(void);

static char *rcname(char *);

static int badopt(void);

static void
error(char *s, ...)
{
	va_list ap;
	if (!optserr)
		return;
	fprintf(stderr, "error: ");
	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);
}

static void
debug(char *s, ...)
{
	va_list ap;
	if (!debug_flag)
		return;
	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);
}

static void
warn(char *s, ...)
{
	va_list ap;
	if (!optserr)
		return;
	fprintf(stderr, "warning: ");
	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);
}

/* optssave() assumes that options refers to a valid array
 * of options, or is NULL and that opts is valid. */
int
optssave(option *options, char *progname) {
	option *op, *aop = NULL;
	char *s = rcname(progname);
	FILE *fp;
	int ok = 1;

	clean();
	if (options)
		opts = options;

	if (!opts)
		return -1;

	fp = fopen(s, "w");
	if (!fp) {
		error("Couldn't open '%s' to save options\n", s);
		return -1;
	}

	for (op = &options[0]; op->type; ++op) {
		/* Preserve last argument met which is not a subarg... */
		if (!(op->flags & OFLAG_ARG))
			aop = op;

		/* We cannot save:
		 * NUL options used to mark ends of lists
		 * Options not meant to be saved.
		 * Options with no value.
		 * Options which have the no-value flag.
		 */
		if (op->type == OTYPE_NUL && (op->flags & OFLAG_ARG))
			continue;
		if (!(op->flags & OFLAG_SAVE))
			continue;
		if ((op->type != OTYPE_NUL) && !op->value)
			continue;
		if (op->flags & OFLAG_NOVAL)
			continue;

		if (aop) {
			if (aop->abbr) {
				fprintf(fp, "%c", aop->abbr);
			} else {
				fprintf(fp, "%s", aop->name);
			}
			if ((op->type == OTYPE_ARG) ||
			    (op->flags & OFLAG_ARG)) {
				fprintf(fp, "%c", '0' + (int) (op - aop));
			}
		}

		switch (op->type) {
		case OTYPE_NUL:
			if (op->value)
				fprintf(fp, ":\n");
			else
				fprintf(fp, "\n");
			break;
		case OTYPE_BOL:
			if (C(op->value, int))
				fprintf(fp, ":\n");
			else
				fprintf(fp, "\n");
			break;
		case OTYPE_SHR:
			fprintf(fp, ":%d\n", C(op->value, short));
			break;
		case OTYPE_INT:
			fprintf(fp, ":%d\n", C(op->value, int));
			break;
		case OTYPE_LNG:
			fprintf(fp, ":%ld\n", C(op->value, long));
			break;
		case OTYPE_ARG:
			fprintf(fp, ":\n");
			break;
		case OTYPE_STR:
			if (C(op->value, char *)) {
				fprintf(fp, ":%s\n", C(op->value, char *));
			} else {
				fprintf(fp, ":\n");
				debug("invalid data for option '%s'.\n",
					op->desc);
			}
			break;
		case OTYPE_BUF:
			fprintf(fp, ":%s\n", P(op->value, char));
			break;
		case OTYPE_DBL:
			fprintf(fp, ":%f\n", C(op->value, double));
			break;
		default:
			fprintf(fp, ":\n");
			debug("option '%s' has invalid type %d\n",
				op->desc, op->type);
			ok = 0;
			break;
		}
	}
	fclose(fp);
	return !ok;
}

int
optscheck(option *options) {
	option **opttest;
	option *op;

	clean();
	if (options) {
		opts = options;
	}
	if (!opts) {
		return -1;
	}

	opttest = calloc(256, sizeof(option *));
	if (!opttest) {
		warn("error allocating option coherence array\n");
		return 1;
	}
	for (op = opts; op && op->type; ++op) {
		if (op->abbr && opttest[op->abbr]) {
			error("options '%s' and '%s' share option letter '%c'\n",
				opttest[op->abbr]->desc,
				op->desc,
				op->abbr);
			return 1;
		} else {
			opttest[op->abbr] = op;
		}
		if (op->value == NULL && op->type != OTYPE_NUL &&
			op->type != OTYPE_ARG &&
			!(op->flags & OFLAG_NOVAL)) {
			error("option '%s' has no value to store anything in.\n", op->name);
			return 1;
		}
		if (op->type == OTYPE_ARG) {
			option *top = op, *aop = op;
			for (top = top + 1; top->type && (top->type != OTYPE_NUL);
				++top) {
				if (!(top->flags & OFLAG_ARG)) {
					error("suboption '%s' not an argument.\n",
						top->name);
					return 1;
				}
			}
			if (top->type != OTYPE_NUL) {
				error("unterminated list for option '%s'.\n",
					aop->name);
				return 1;
			}
					
		}
	}
	free(opttest);
	opttest = NULL;
	return 0;
}

int
optsload(option *options, char *progname) {
	option *op;
	FILE *fp = NULL;
	char buf[256], *s;
	int ok = 1;
	int count = 0;

	clean();
	if (options)
		opts = options;

	if (!opts)
		return -1;

	fp = fopen(rcname(progname), "r");

	if (!fp) {
		debug("no options yet.\n");
		return -1;
	}

	while (fgets(buf, 256, fp) != NULL) {
		/* eliminate trailing newline, if any */
		s = strrchr(buf, '\n');
		if (s)
			*s = '\0';

		/* split around 1st ':' */

		s = strchr(buf, ':');
		if (s) {
			*s = 0;
			++s; /* ...and leave s pointing to what's after it */
		}


		if ((isdigit(buf[1]) && !buf[2]) || !buf[1]) {
			if (buf[1]) {
				count = buf[1] - '0';
			} else {
				count = 0;
			}

			for (op = &opts[0]; op->type; ++op) {
				if (op->abbr == *buf)
					break;
			}
		} else {
			option *fop = NULL;
			int i = strlen(buf);

			if (isdigit(buf[i - 1])) {
				count = buf[i - 1] - '0';
			} else {
				count = 0;
			}

			for (op = opts; op->type; ++op) {
				if (!strncmp(op->name, buf, i)) {
					if (!fop) {
						fop = op;
					} else {
						error("ambiguous option '%s'\n",
							buf);
						warn("matches '%s' and '%s'\n",
							fop->name, op->name);
					}
				}
			}
			if (fop)
				op = fop;

		}
		/* if we found a valid option matching buf, we
		 * set it.  s of null is valid for some options,
		 * we let op_set determine if this is ok.
		 */
		if (op->type) {
			if ((op->type != OTYPE_ARG) || (count > 0)) {
				/* 3rd arg of 1 -> set, not unset */
				if (op_set(op + count, s))
					ok = 0;
			}
		}
	}
	return !ok;
}

static char *
rcname(char *progname) {
	static char buf[256], *s;

	s = getenv("HOME");
	sprintf(buf, "%s/.%src", s ? s : "", progname);

	return buf;
}

char *
optsusage(option *op) {
	static char buf[1024];
	int offset;

	clean();
	sprintf(buf, "\t%s", op->flags & OFLAG_ARG ? "   " : "");
	/* this would not be needed if Sun didn't suck.  */
	/* (sun's older systems have a defective sprintf which
	 * does not return the # of chars written. */
	offset = strlen(buf);

	if (op->abbr)
		sprintf(buf + offset, "-%c | --%s\t'%s' [%s]",
			op->abbr, op->name, op->desc, typenames[op->type]);
	else
		sprintf(buf + offset, "--%s\t'%s' [%s]",
			op->name, op->desc, typenames[op->type]);

	return buf;
}

char *
optsprint(option *op) {
	static char buf[1024];
	if ((op->type != OTYPE_NUL) && (op->type != OTYPE_ARG) && !op->value) {
		sprintf(buf, "option '%s': invalid data",
			op->desc);
		return buf;
	}

	switch (op->type) {
		case OTYPE_NUL:
			sprintf(buf, "option '%s' %s.", op->desc,
				op->value ? "set" : "unset");
			break;
		case OTYPE_BOL:
			if (C(op->value, int))
				sprintf(buf, "option '%s' set.",
					op->desc);
			else
				sprintf(buf, "option '%s' unset.",
					op->desc);
			break;
		case OTYPE_ARG:
			sprintf(buf, "multi-arg option '%s':", op->desc);
			break;
		case OTYPE_SHR:
			sprintf(buf, "option '%s': %d.",
				op->desc, C(op->value, short));
			break;
		case OTYPE_INT:
			sprintf(buf, "option '%s': %d.",
				op->desc, C(op->value, int));
			break;
		case OTYPE_LNG:
			sprintf(buf, "option '%s': %ld.",
				op->desc, C(op->value, long));
			break;
		case OTYPE_STR:
			if (C(op->value, char *))
				sprintf(buf, "option '%s': %s.",
					op->desc, C(op->value, char *));
			else
				sprintf(buf, "option '%s': invalid data",
					op->desc);
			break;
		case OTYPE_BUF:
			sprintf(buf, "option '%s': %s.",
				op->desc, P(op->value, char));
		case OTYPE_DBL:
			sprintf(buf, "option '%s': %f.",
				op->desc, C(op->value, double));
			break;
		default:
			sprintf(buf, "invalid type %d for '%s'",
				op->type, op->desc);
	}
	return buf;
}

option *
optsmake(char *str) {
	option *ops, *op;
	char *s;
	int count = 0;

	clean();

	for (s = str; *s; ++s)
		if (!strchr(":#", *s))
			++count;

	ops = malloc((count + 1) * sizeof(option));

	if (!ops)
		return NULL;

	count = 0;
	for (s = str; *s; ++s) {
		op = &ops[count++];

		op->abbr = *s;
		op->name = "";
		op->desc = "";
		op->flags = OFLAG_NOVAL;
		op->value = &optsval;
		op->defalt = 0;
		op->verify = 0;
		op->settor = 0;

		if (s[1] == ':') {
			op->type = OTYPE_STR;
			++s;
		} else if (s[1] == '#') {
			op->type = OTYPE_INT;
			++s;
		} else {
			op->type = OTYPE_BOL;
		}
	}
	op = &ops[count];
	op->type = OTYPE_END;
	op->abbr = '\0';
	op->name = "";
	op->desc = "";
	op->flags = OFLAG_NONE;
	op->value = 0;
	op->defalt = 0;
	op->verify = 0;
	op->settor = 0;

	return ops;
}

int
optsget(int argc, char **argv, option *op) {
	if (argv != oldargv) {
		optsind = 1;
		curind = 1;
		curloc = 0;
		oldargv = argv;
	}

	if (op) {
		opts = op;
		if (optscheck(op))
			return badopt();
	} else {
		return badopt();
	}

	return optgetopt(argc, argv);
}

int
optsopt(int argc, char **argv, char *str) {
	static option *op = NULL;
	clean();

	if (argv != oldargv) {
		optsind = 1;
		curind = 1;
		curloc = 0;
		oldargv = argv;
	}

	if (!str) {
		return badopt();
	}

	if (!last_optstring || strcmp(str, last_optstring) || !op) {
		if (last_optstring) {
			free(last_optstring);
			last_optstring = malloc(strlen(str) + 1);
			strcpy(last_optstring, str);
		}
		if (op) /* tolerance for broken sunos */
			free(op);
		op = optsmake(str);
	}

	if (op) {
		opts = op;
		return optgetopt(argc, argv);
	} else {
		return badopt();
	}
}

/* do the getopts thang. */
int
optsgets(int argc, char **argv, option *options) {
	int ok = 1;
	int arg;

	if (argv != oldargv) {
		optsind = 1;
		curind = 1;
		curloc = 0;
		oldargv = argv;
	}

	if (options) {
		if (optscheck(options))
			return 1;
		opts = options;
	}

	while (ok && ((arg = optgetopt(argc, argv)) != -1)) {
		debug("ogo loop, curind/optsind is now %d/%d\n",
			curind, optsind);
		if (arg == '?') {
			ok = 0;
		}
	}
	return !ok || !optsok;
}

static void
clean(void) {
	if (cleanflag) {
		if (C(&optsval, char *))
			free(C(&optsval, char *));
		cleanflag = 0;
	}
}

static int
op_set(option *op, char *s) {
	opt_value backup;
	opt_value *saveval = NULL;
	char *t;
	long l;
	double d;
	int ret = 0;
	char buf_temp[1024];

	if (!op) {
		warn("internal error, op_set called without option\n");
		return 1;
	}

	debug("op_set: option '%s', set from '%s'\n", op->name,
		s ? s : "<null pointer>");


	if ((op->flags & OFLAG_SET) && (op->flags & OFLAG_CONST)) {
		warn("option '%s' was already set.\n", op->name);
		return 1;
	}

	if (op->flags & OFLAG_NOVAL) {
		saveval = op->value;
		op->value = &optsval;
		/* and we have to initialize it, so it will be safe to read */
		switch (op->type) {
		case OTYPE_SHR:
			C(op->value, short) = 0;
			break;
		case OTYPE_INT: case OTYPE_BOL:
			C(op->value, int) = 0;
			break;
		case OTYPE_LNG:
			C(op->value, long) = 0;
			break;
		case OTYPE_DBL:
			C(op->value, double) = 0;
			break;
		case OTYPE_STR:
			C(op->value, char *) = NULL;
			break;
		case OTYPE_ARG:
			break;
		}
	}

	if (op->verify && ((op->type == OTYPE_NUL) || op->value)) {
		switch (op->type) {
		default: break;
		case OTYPE_NUL: backup.v_int = !!op->value; break;
		case OTYPE_BOL:
		case OTYPE_INT: backup.v_int = C(op->value, int); break;
		case OTYPE_SHR: backup.v_short = C(op->value, short); break;
		case OTYPE_LNG: backup.v_long = C(op->value, long); break;
		case OTYPE_STR: backup.v_str = C(op->value, char *); break;
		case OTYPE_BUF: buf_temp[0] = '\0';
				strncpy(buf_temp, P(op->value, char), 1024);
				break;
		case OTYPE_DBL: backup.v_double = C(op->value, double); break;
		}
	}

	if (!s) {
		if (((op->type != OTYPE_BOL) && (op->type != OTYPE_NUL)) &&
			!(op->flags & OFLAG_DFL)) {
			warn("option '%c' ['%s'] improperly specified,"
				" or wrongly negated.\n", op->abbr, op->desc);
			ret = 1;
		}

		if (op->defalt) {
			switch (op->type) {
			case OTYPE_NUL:
				op->value = &optsval;
				break;
			case OTYPE_SHR:
				C(op->value, short) = C(op->defalt, short);
				break;
			case OTYPE_INT: case OTYPE_BOL:
				C(op->value, int) = C(op->defalt, int);
				break;
			case OTYPE_LNG:
				C(op->value, long) = C(op->defalt, long);
				break;
			case OTYPE_DBL:
				C(op->value, double) = C(op->defalt, double);
				break;
			case OTYPE_STR:
				if (!(op->flags & OFLAG_NOVAL) &&
				    C(op->value, char *))
					free(C(op->value, char *));
				C(op->value, char *) =
					malloc(strlen(C(op->defalt, char *)) + 1);
				if (C(op->value, char *))
					strcpy(C(op->value, char *),
						C(op->defalt, char *));
				break;
			case OTYPE_ARG:
				break;
			}
		} else {
			switch (op->type) {
			case OTYPE_NUL:
				op->value = NULL;
				break;
			case OTYPE_SHR:
				C(op->value, short) = 0;
				break;
			case OTYPE_INT: case OTYPE_BOL:
				C(op->value, int) = 0;
				break;
			case OTYPE_LNG:
				C(op->value, long) = 0;
				break;
			case OTYPE_DBL:
				C(op->value, double) = 0;
				break;
			case OTYPE_STR:
				if (!(op->flags & OFLAG_NOVAL) && C(op->value, char *))
					free(C(op->value, char *));
				C(op->value, char *) = NULL;
			case OTYPE_ARG:
				break;
			}
		}
	} else {
		if (((op->type != OTYPE_NUL) && (op->type != OTYPE_BOL))
			&& !op->value) {
			error("invalid data for option '%s'\n",
				op->name);
			ret = 1;
		}
		switch (op->type) {
		case OTYPE_NUL:
			op->value = &optsval;
			break;
		case OTYPE_SHR:
			l = strtol(s, &t, 0);
			if ((l > (long) SHRT_MAX) || (l < (long) SHRT_MIN)) {
				warn("'%s' is out of range for a short.\n", s);
			}
			if (*t) {
				error("'%s' does not look like an integer.\n",
					s);
				ret = 1;
			} else {
				C(op->value, short) = (short) l;
			}
			break;
		case OTYPE_BOL:
			C(op->value, int) = 1;
			break;
		case OTYPE_INT:
			l = strtol(s, &t, 0);
			if ((l > (long) INT_MAX) || (l < (long) INT_MIN)) {
				warn("'%s' is out of range for an int.\n", s);
			}
			if (*t) {
				error("'%s' does not look like an integer.\n",
					s);
				ret = 1;
			} else {
				C(op->value, int) = (int) l;
			}
			break;
		case OTYPE_LNG:
			l = strtol(s, &t, 0);

			if (*t) {
				error("'%s' does not look like an integer.\n",
					s);
				return 1;
			} else {
				C(op->value, long) = l;
			}
			break;
		case OTYPE_STR:
			/* if we verify, we can't free this here */
			if (!(op->flags & OFLAG_NOVAL) &&
			    C(op->value, char *) && !op->verify)
				free(C(op->value, char *));

			t = malloc(strlen(s) + 1);
			strcpy(t, s);
			C(op->value, char *) = t;
			break;
		case OTYPE_BUF:
			strcpy(P(op->value, char), s);
			break;
		case OTYPE_DBL:
			d = strtod(s, &t);
			if (*t) {
				error("'%s' does not look like a real.\n", s);
				ret = 1;
			} else {
				C(op->value, double) = d;
			}
			break;
		case OTYPE_ARG:
			error("cannot set 'argument' option '%s'.\n",
				op->desc);
			ret = 1;
			break;
		default:
			error("option '%s' has invalid type %d\n",
				op->desc, op->type);
			ret = 1;
			break;
		}
	}

	if (op->type == OTYPE_NUL)
		optsval.v_int = !!op->value;
	else if (op->value && (op->value != &optsval)) {
		switch (op->type) {
		default:
		case OTYPE_INT: case OTYPE_BOL:
			optsval.v_int = C(op->value, int); break;
		case OTYPE_SHR:
			optsval.v_short = C(op->value, short); break;
		case OTYPE_LNG:
			optsval.v_long = C(op->value, long); break;
		case OTYPE_STR:
			optsval.v_str = C(op->value, char *); break;
		case OTYPE_DBL:
			optsval.v_double = C(op->value, double); break;
		}
	}

	if (op->verify && !op->verify(op, &optsval)) {
		switch (op->type) {
		default:
		case OTYPE_NUL:
			op->value = backup.v_int ? &optsval : NULL;
			break;
		case OTYPE_INT: case OTYPE_BOL:
			C(op->value, int) = backup.v_int;
			break;
		case OTYPE_SHR: C(op->value, short) = backup.v_short; break;
		case OTYPE_LNG: C(op->value, long) = backup.v_long; break;
		case OTYPE_STR:
			if (!(op->flags & OFLAG_NOVAL) && C(op->value, char *))
				free(C(op->value, char *));
			C(op->value, char *) = backup.v_str;
			break;
		case OTYPE_BUF:
			P(op->value, char)[0] = '\0';
			strncat(P(op->value, char), buf_temp, 1024);
			break;
		case OTYPE_DBL: C(op->value, double) = backup.v_double; break;
		}
	} else {
		/* if we were verifying, now we need to do this... */
		if (op->verify && (op->type == OTYPE_STR))
			free(backup.v_str);
		if (op->settor)
			op->settor(op, &optsval);
		if (op->flags & OFLAG_NOVAL)
			op->value = saveval;
		op->flags |= OFLAG_SET;
	}

	if (op->type == OTYPE_NUL)
		optsval.v_int = !!op->value;
	else if (op->value && (op->value != &optsval)) {
		switch (op->type) {
		default:
		case OTYPE_INT: case OTYPE_BOL:
			optsval.v_int = C(op->value, int); break;
		case OTYPE_SHR:
			optsval.v_short = C(op->value, short); break;
		case OTYPE_LNG:
			optsval.v_long = C(op->value, long); break;
		case OTYPE_STR:
			optsval.v_str = C(op->value, char *); break;
		case OTYPE_DBL:
			optsval.v_double = C(op->value, double); break;
		}
		if (op->is_set)
			*op->is_set = 1;
	}

	if ((op->value == &optsval) && (op->type == OTYPE_STR))
		cleanflag = 1;
	else
		cleanflag = 0;

	return ret;
}

option *
optsfind(int c, char *s, option *options) {
	int i;
	option *op, *fop = NULL;

	if (!options)
		options = opts;
	if (!options)
		return 0;

	if (c) {
		for (op = options; op->type; ++op) {
			if (op->abbr == c) {
				optsok = 1;
				return op;
			}
		}
		optsok = 0;
		return NULL;
	} else {
		i = strlen(s);
		for (op = options; op->type; ++op) {
			if (op->name && !strncmp(op->name, s, i)) {
				if (!fop) {
					fop = op;
				} else {
					error("ambiguous option '%s'\n", s);
					warn("matches '%s' and '%s'\n",
						fop->name, op->name);
					curloc = 0;
					optsok = 0;
					return NULL;
				}
			}
		}
	}
	if (fop)
		optsok = 1;
	else
		optsok = 0;
	return fop;
}

static option *
incr_opt(void) {
	char *s = av[curind];

	if (curind >= ac || !s)
		return NULL;

	if (s)
		curlen = strlen(s);

	debug("incr_opt: s: %s, curind %d, curloc %d, curlen %d\n",
		s ? s : "null",
		curind, curloc, curlen);
	if ((curloc > 0) && ((curloc >= curlen) || !s[curloc])) {
		/* we think we're in a word, but it's done */
		curloc = -1;
		if (curind < optsind)
			curind = optsind;
		else {
			++curind;
			++optsind;
		}
		s = av[curind];
		if (!s)
			return NULL;
		curlen = strlen(s);
	} else if (curloc > 0) {
		if (s[curloc] == '!') {
			++curloc;
			if (sense == 0) {
				warn("ain't no double negatives allowed.\n");
			}
			sense = !sense;
			return incr_opt();
		} else {
			return optsfind(s[curloc++], 0, opts);
		}
	}
	/* at this point curloc is 0 or lower, i.e., we're at the
	 * beginning of a word.
	 */
	if (s[0] == '-') {
		/* maybe an option */
		if (s[1] == '-') {
			if (s[2] == '\0') {
				if (optsind <= curind)
					optsind = curind + 1;
				return NULL;
			}
			/* we think we are in a -- option */

			debug("incr_opt (--) opts/cur %s/%s\n",
				av[optsind], av[curind]);
			if (strncmp(s + 2, "no-", 3) == 0) {
				if (sense == 0) {
					warn("ain't no double negatives allowed.\n");
				}
				sense = 0;
				curloc = curlen;
				return optsfind(0, s + 5, opts);
			} else {
				curloc = curlen;
				return optsfind(0, s + 2, opts);
			}
		}
		if (s[1] == '\0')
			return NULL;
		curloc = 1;
		return incr_opt();
	} else {
		optsok = 1; /* we can handle this. */
		return NULL;
	}
}

static int
optgetopt(int argc, char *argv[]) {
	option *op, *fop = NULL; /* fop is our "found" option pointer */

	ac = argc;
	av = argv;

	clean();
	/* check for end of args */
	fop = incr_opt();

	if (fop == NULL) {
		return optsok ? -1 : '?';
	}

	if (optsind <= curind)
		optsind = curind + 1;

	debug("ogo processing arg %d [oi %d], '%s' [pos %d]\n",
		curind, optsind, argv[curind], curloc);

	/* here, we have "fop" -> our current option */

	op = fop; /* simplify our typing */

	debug("ogo: op->name %s\n", op->name);

	if (op->type == OTYPE_ARG) {
		for (op = op + 1; op->type != OTYPE_NUL; ++op) {
			optsarg = getarg(argc, argv);
			debug("arg, setting from '%s', optsind %d.\n",
				optsarg, optsind);
			if (op_set(op, optsarg)) {
				if (curloc < 1) {
					curind = optsind;
					curloc = 0;
					++optsind;
				}
				sense = 1;
				return '?'; /* failure, but documented above */
			} else {
				if (curloc < 1) {
					curind = optsind;
					curloc = 0;
				}
				sense = 1;
			}
		}
		return fop->abbr;
	} else {
		if ((op->type == OTYPE_NUL) || (op->type == OTYPE_BOL)) {
			optsarg = NULL;
			if (op_set(op, sense ? "" : NULL)) {
				sense = 1;
				return '?'; /* failure, but documented above */
			} else {
				sense = 1;
				return op->abbr;
			}

		} else {
			optsarg = getarg(argc, argv);

			if (op_set(op, sense ? optsarg : NULL)) {
				sense = 1;
				return '?'; /* failure, but documented above */
			} else {
				sense = 1;
				return op->abbr;
			}
		}
	}
}

static char *
getarg(int argc, char **argv) {
	char *ret = NULL;
	if (optstyle == STYLE_POSIX) {
		if (curloc && curloc < curlen) {
			ret = &(argv[curind][curloc]);
			curloc = curlen;
		}
	}
	if (!ret && optsind < argc) {
		ret = argv[optsind++];
	}
	if (!ret)
		warn("insufficent arguments.\n");
	return ret;
}

int
optsetstyle(int posix) {
	static int was_posix;
	was_posix = !optstyle;
	optstyle = !posix;
	return was_posix;
}

static int
badopt(void) {
	static int failed = 0;
	if (failed) {
		return -1;
	} else {
		failed = 1;
		return '?';
	}
}

void optsclean(option *options) 
{
	int i = 0;
	option *o = 0;

	if (last_optstring)
	{
		free(last_optstring);
		last_optstring = 0;
	}

	while (options[i].type != OTYPE_END)
	{
		o = &options[i];
		if (o->type == OTYPE_STR && o->value && *(char**)o->value)
		{
			free(*(char**)(o->value));
			options[i].value = 0;
		}
		i++;
	}
}