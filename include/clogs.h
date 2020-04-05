#ifndef _CLOGS_H
#define _CLOGS_H

#include <stdio.h>
#include <time.h>

extern int log_limit;

#define _tlog(file) fprintf(file, "[%d] ", (int)time(NULL));

#define flogf(level, outfile, fmt, ...) \
if ((level) < log_limit) { \
	_tlog(outfile); \
	fprintf(outfile, "%s: ", __FUNCTION__); \
	fprintf(outfile, fmt, ##__VA_ARGS__); \
}

#define logf(level, fmt, ...) flogf(level, stderr, fmt, ##__VA_ARGS__);

#endif
