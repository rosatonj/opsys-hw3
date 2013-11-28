/* Debug logging API */

/* initialize debug log using memory starting at address "debug_area" */
void init_debuglog(char *debug_area, int debug_area_length);

/* append msg to memory log */
void debuglog(char *msg);

/* for access to filled-in log: return start point of log area */
char * get_debuglog_string(void);

/* to find out if debuglog had to wrap around in its memory */
int get_debuglog_wrapcount(void);


