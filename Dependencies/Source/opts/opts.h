/* This software is Copyright 1995-2005  Peter Seebach.  Please see the
 * associated file "COPYRIGHT" for copyright information, or write
 * seebs@plethora.net
 */

#ifndef _opts_h_
#define _opts_h_

#ifdef __cplusplus
extern "C" {
#endif 

/* option library definitions */
enum {
	OFLAG_NONE = 0x0,
	OFLAG_SAVE = 0x1,
	OFLAG_DFL = OFLAG_SAVE << 1,
	OFLAG_SET = OFLAG_DFL << 1,
	OFLAG_ARG = OFLAG_SET << 1,
	OFLAG_CONST = OFLAG_ARG << 1,
	OFLAG_NOVAL = OFLAG_CONST << 1,
	OFLAG_MAX = OFLAG_NOVAL << 1
};

enum {
	OTYPE_END, /* end of options */
	OTYPE_NUL, /* no content */
	OTYPE_BOL, /* boolean */
	OTYPE_SHR,
	OTYPE_INT,
	OTYPE_LNG,
	OTYPE_STR,
	OTYPE_BUF,
	OTYPE_DBL,
	OTYPE_ARG, /* multiple args */
	OTYPE_MAX
};

struct option_struct;

typedef union {
	double v_double;
	char *v_str;
	char v_buf; /* ick */
	int v_int;
	short v_short;
	long v_long;
} opt_value;

typedef int (*optsverify)(struct option_struct *, opt_value *);
typedef void (*optssettor)(struct option_struct *, opt_value *);

typedef struct option_struct {
	int type;
	char abbr;
	char *name;
	char *desc;
	int flags;
	void *value;
	void *defalt; /* sic. */
	optsverify verify;
	optssettor settor;
	int *is_set;
} option;

/* our provided interface */
extern int optsload(option *, char *), optssave(option *, char *);
extern char *optsprint(option *);
extern char *optsusage(option *);
extern int optscheck(option *);
extern int optsgets(int, char **, option *);
extern int optsget(int, char **, option *);
extern int optsopt(int, char **, char *);
extern int optsetstyle(int);
extern option *optsmake(char *);
extern option *optsfind(int, char *, option *);
extern void optsclean(option *);
extern int optsind, optserr, optsok;
extern opt_value optsval;
extern char *optsarg;

#ifdef __cplusplus
}
#endif 

#endif //_opts_h_