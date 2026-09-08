# rawcom

Minimal CLI tool for Windows to monitor raw serial (COM) port data and modem control lines

## Features

- Zero third-party dependencies (pure Win32 API).
- Custom port selection and baud rate configuration via CLI flags.
- Reports initial modem control pin states (`CTS`, `DSR`, `DCD`).
- Flushes raw bytes directly to `stdout`.

## Build

Using MinGW / GCC:

```bash
make
```

Or manual compilation:

```bash
gcc -O2 src/rawcom.c -o rawcom.exe
```

## Usage

```text
rawcom.exe -p <PORT> -s <BAUDRATE>
```

### Examples

Listen on `COM1` at default 9600 baud:

```cmd
rawcom.exe -p COM1 -s 9600
```

Listen on `COM3` at 115200 baud:

```cmd
rawcom.exe -p COM3 -s 115200
```

## Output

```text
Port: COM1
Speed: 9600
CTS: 1
DSR: 0
DCD: 0
---
<raw incoming stream>
```

Press `Ctrl+C` to terminate the session.

## License

MIT