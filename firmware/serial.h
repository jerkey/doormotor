#ifndef _SERIAL_H
#define _SERIAL_H

#define MAX_ARGS   4

/* Public variables */
extern int serial_argc;
extern char *serial_argv[MAX_ARGS];

/* Public functions */
void serial_init();
void serial_putc(unsigned char c);
void serial_write(const char *buf, int len);
void serial_rx_disable();
void serial_rx_enable();

#endif
