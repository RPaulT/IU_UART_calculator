#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>

bool serial_open(const char *portname);
void serial_close();
bool serial_write_line(const char *line);
bool serial_read_line(char *buffer, int maxlen);

#endif