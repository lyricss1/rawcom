# rawcom

Small cross-platform CLI utility to read raw data from serial ports on Linux and Windows

## Features

* No heavy dependencies (pure POSIX / Win32 API)
* List available serial ports with --show
* Supports Linux (/dev/ttyUSB*, /dev/ttyACM*, /dev/ttyS*) and Windows (COM*)
* Custom baud rate configuration.
* Optional modem line inspection (CTS, DSR, RING, RLSD/DCD).
* Dumps incoming serial stream directly to stdout.

## Build

### Linux

```
make
```
Or manually:
```
gcc -Wall -I. -o rawcom rawcom.c src/serial.c
```
### Windows (MinGW / GCC)
```
make
```
Or manually:
```
gcc -Wall -I. -o rawcom.exe rawcom.c src/serial.c
```
## Usage
```
rawcom <port> [baud] [-pins]
rawcom --show
```
* <port> - Serial port name (COM3 on Windows, /dev/ttyUSB0 on Linux).
* [baud] - Baud rate (default: 9600). Common values: 9600, 19200, 38400, 57600, 115200.
* [-pins] - Optional flag to print modem pin states before listening.

### Examples

Linux:
```
./rawcom --show
./rawcom /dev/ttyUSB0 115200 -pins
./rawcom /dev/ttyACM0
```
Windows:
```
rawcom.exe --show
rawcom.exe COM3 115200 -pins
rawcom.exe COM1 9600
```
## Output Example
```
opening port /dev/ttyUSB0 at 115200...
pins status:
CTS: 0 | DSR: 0 | RING: 0 | DCD: 0
listening (ctrl+c to stop)...
[Serial output stream]
```
Press Ctrl+C to exit.

## License

MIT
