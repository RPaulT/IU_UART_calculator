#include "serial.h"
#include <windows.h>

static HANDLE hSerial = NULL;

bool serial_open(const char *portname) {
    hSerial = CreateFileA(portname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) return false;

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) return false;

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) return false;

    return true;
}

void serial_close() {
    if (hSerial) CloseHandle(hSerial);
    hSerial = NULL;
}

bool serial_write_line(const char *line) {
    if (!hSerial) return false;
    DWORD written;
    WriteFile(hSerial, line, strlen(line), &written, NULL);
    WriteFile(hSerial, "\n", 1, &written, NULL);
    return true;
}

bool serial_read_line(char *buffer, int maxlen) {
    if (!hSerial) return false;
    DWORD read;
    char c;
    int i = 0;
    while (i < maxlen - 1) {
        if (!ReadFile(hSerial, &c, 1, &read, NULL) || read == 0) break;
        if (c == '\n') break;
        if (c == '\r') continue;
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return i > 0;
}