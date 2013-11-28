/* Record debug info in memory */
/* Note: there is no upper limit on the memory area, so
   there's a potential for overwriting memory objects
   higher in memory than this. */

#include "debuglog.h"

static char *debug_log_area = 0;
static int debug_log_length;
static char *debug_log_cursor;  /* current pointer into log area */ 
static int debug_log_wrapcount;

void init_debuglog(char *debug_area, int length)
{
    debug_log_area = debug_area;  /* remember starting point */
    debug_log_cursor = debug_area; /* initialize log's end pointer */
    debug_log_length = length;
    debug_log_wrapcount = 0;
}

/* append msg to memory log */
void debuglog(char *msg)
{
    int length = strlen(msg);
    if (length > debug_log_length) {
	msg = "****debug message too long***";
	length = strlen(msg);
    }
    if (debug_log_cursor + length >= debug_log_area + debug_log_length) {
	debug_log_cursor = debug_log_area; /* wrap around in provided memory */
	debug_log_wrapcount++;
    }
    strcpy(debug_log_cursor, msg);  /* append msg to end of log */
    debug_log_cursor += length;  /* advance cursor */
}

/* for access to filled-in log: return start point of log area */
char * get_debuglog_string()
{
    return debug_log_area;
}
int get_debuglog_wrapcount()
{
    return debug_log_wrapcount;
}
