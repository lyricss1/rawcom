#ifndef SERIAL_H
#define SERIAL_H

#ifdef _WIN32
#include <windows.h>
typedef HANDLE port_t;
#define BAD_PORT INVALID_HANDLE_VALUE
#else
typedef int port_t;
#define BAD_PORT -1
#endif

port_t my_open_port(char *name);
int set_baud(port_t fd, int speed);
int my_read_data(port_t fd, char *buf, int len);
int my_send_data(port_t fd, char *buf, int len);
void my_close_port(port_t fd);
void check_pins(port_t fd);
void show_ports();

#endif