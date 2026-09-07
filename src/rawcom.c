#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char port[32] = "";
    int baud = 9600;
    for (int i=1;i<argc;i++){
        if(strcmp(argv[i],"-p")==0 && i+1<argc){
            strcpy(port, argv[i+1]);
            i++;
        }else if((strcmp(argv[i],"-s")== 0 || strcmp(argv[i],"-b")==0) && i+1<argc){
            baud = atoi(argv[i+1]);
            i++;
        }
    }
    if(strlen(port)==0){
        printf("Use %s -p COM1 -s 9600\n", argv[0]);
        return 1;
    }
    char fullPortName[64];
    sprintf(fullPortName, "\\\\.\\%s", port);
    HANDLE h = CreateFileA(fullPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(h == INVALID_HANDLE_VALUE){
        printf("Error open %s (error %d)\n",port,(int)GetLastError());
        return 1;
    }
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if(!GetCommState(h, &dcb)){
        printf("Failed state\n");
        CloseHandle(h);
        return 1;
    }

    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    if(!SetCommState(h, &dcb)){
        printf("Failed baudrate %d\n",baud);
        CloseHandle(h);
        return 1;
    }
    COMMTIMEOUTS ct;
    ct.ReadIntervalTimeout =         20;
    ct.ReadTotalTimeoutConstant =    20;
    ct.ReadTotalTimeoutMultiplier =  1;
    ct.WriteTotalTimeoutConstant =   20;
    ct.WriteTotalTimeoutMultiplier = 1;
    SetCommTimeouts(h, &ct);

    DWORD status = 0;
    if (GetCommModemStatus(h, &status)) {
        printf("Port: %s\n",port);
        printf("Speed: %d\n",baud);
        printf("CTS: %d\n",(status & MS_CTS_ON) ? 1 : 0);
        printf("DSR: %d\n",(status & MS_DSR_ON) ? 1 : 0);
        printf("DCD: %d\n",(status & MS_RLSD_ON) ? 1 : 0);
        printf("---\n");
    }
    char buffer[256];
    DWORD bytesRead = 0;
    while(1){
        BOOL res = ReadFile(h, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        if(res){
            if(bytesRead > 0){
                buffer[bytesRead] = '\0';
                printf("%s", buffer);
                fflush(stdout);
            }
        }else{
            DWORD err = GetLastError();
            if(err == ERROR_DEVICE_NOT_CONNECTED || err == ERROR_OPERATION_ABORTED || err == ERROR_GEN_FAILURE){
                printf("\nDevice disconnected\n");
                break;
            }
        }
        Sleep(1);
    }
    CloseHandle(h);
    return 0;
}