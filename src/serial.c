#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

port_t my_open_port(char *name) {
    char full[64];
    if (strstr(name, "\\\\.\\") == NULL) {
        sprintf(full, "\\\\.\\%s", name);
    } else {
        strcpy(full, name);
    }

    HANDLE h = CreateFileA(full, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        return BAD_PORT;
    }
    return h;
}

int set_baud(port_t fd, int speed) {
    DCB d;
    memset(&d, 0, sizeof(d));
    d.DCBlength = sizeof(d);

    if (!GetCommState(fd, &d)) {
        return 0;
    }

    d.BaudRate = speed;
    d.ByteSize = 8;
    d.StopBits = ONESTOPBIT;
    d.Parity = NOPARITY;

    if (!SetCommState(fd, &d)) {
        return 0;
    }

    COMMTIMEOUTS to;
    to.ReadIntervalTimeout = 50;
    to.ReadTotalTimeoutConstant = 50;
    to.ReadTotalTimeoutMultiplier = 10;
    to.WriteTotalTimeoutConstant = 50;
    to.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(fd, &to);

    return 1;
}

int my_read_data(port_t fd, char *buf, int len) {
    DWORD r = 0;
    if (!ReadFile(fd, buf, len, &r, NULL)) {
        return -1;
    }
    return (int)r;
}

int my_send_data(port_t fd, char *buf, int len) {
    DWORD w = 0;
    if (!WriteFile(fd, buf, len, &w, NULL)) {
        return -1;
    }
    return (int)w;
}

void my_close_port(port_t fd) {
    if (fd != INVALID_HANDLE_VALUE) {
        CloseHandle(fd);
    }
}

void check_pins(port_t fd) {
    DWORD s = 0;
    if (!GetCommModemStatus(fd, &s)) {
        printf("cant read pins\n");
        return;
    }
    printf("CTS: %s | DSR: %s | RING: %s | RLSD: %s\n",
           (s & MS_CTS_ON) ? "1" : "0",
           (s & MS_DSR_ON) ? "1" : "0",
           (s & MS_RING_ON) ? "1" : "0",
           (s & MS_RLSD_ON) ? "1" : "0");
}

#else

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

port_t my_open_port(char *name) {
    int fd = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return BAD_PORT;
    }
    fcntl(fd, F_SETFL, 0);
    return fd;
}

int set_baud(port_t fd, int speed) {
    struct termios opt;
    if (tcgetattr(fd, &opt) != 0) {
        return 0;
    }

    speed_t sp;
    if (speed == 9600) sp = B9600;
    else if (speed == 19200) sp = B19200;
    else if (speed == 38400) sp = B38400;
    else if (speed == 57600) sp = B57600;
    else if (speed == 115200) sp = B115200;
    else {
        printf("unknown speed,now using 9600\n");
        sp = B9600;
    }

    cfsetispeed(&opt, sp);
    cfsetospeed(&opt, sp);

    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~CSTOPB;
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);
    opt.c_oflag &= ~OPOST;

    opt.c_cc[VMIN] = 0;
    opt.c_cc[VTIME] = 5;

    if (tcsetattr(fd, TCSANOW, &opt) != 0) {
        return 0;
    }
    return 1;
}

int my_read_data(port_t fd, char *buf, int len) {
    return read(fd, buf, len);
}

int my_send_data(port_t fd, char *buf, int len) {
    return write(fd, buf, len);
}

void my_close_port(port_t fd) {
    if (fd >= 0) {
        close(fd);
    }
}

void check_pins(port_t fd) {
    int status;
    if (ioctl(fd, TIOCMGET, &status) < 0) {
        printf("cant read pins\n");
        return;
    }
    printf("CTS: %s | DSR: %s | RING: %s | DCD: %s\n",
           (status & TIOCM_CTS) ? "1" : "0",
           (status & TIOCM_DSR) ? "1" : "0",
           (status & TIOCM_RNG) ? "1" : "0",
           (status & TIOCM_CD) ? "1" : "0");
}

#endif