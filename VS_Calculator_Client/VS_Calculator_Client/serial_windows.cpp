#include "serial.h"
#include <windows.h>

// Handle für die serielle Verbindung
static HANDLE hSerial = NULL;

// Öffnet den seriellen Port mit dem angegebenen Namen (z.B. "COM3")
// und konfiguriert ihn auf 9600 Baud, 8 Datenbits, keine Parität, 1 Stoppbit
bool serial_open(const char* portname) {
    // Öffnet die serielle Schnittstelle (nur Lese-/Schreibzugriff, keine Weitergabe)
    hSerial = CreateFileA(portname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) return false;

    // Struktur zur Konfiguration der seriellen Schnittstelle
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // Liest aktuelle Einstellungen
    if (!GetCommState(hSerial, &dcbSerialParams)) return false;

    // Setzt gewünschte Parameter
    dcbSerialParams.BaudRate = CBR_9600;      // Baudrate: 9600
    dcbSerialParams.ByteSize = 8;             // Datenbits: 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // 1 Stoppbit
    dcbSerialParams.Parity = NOPARITY;      // keine Parität

    // Wendet neue Einstellungen an
    if (!SetCommState(hSerial, &dcbSerialParams)) return false;

    return true;
}

// Schließt den seriellen Port, falls geöffnet
void serial_close() {
    if (hSerial) CloseHandle(hSerial);
    hSerial = NULL;
}

// Sendet eine Zeichenkette (Zeile) über den seriellen Port, gefolgt von einem Zeilenumbruch ('\n')
bool serial_write_line(const char* line) {
    if (!hSerial) return false;
    DWORD written;
    WriteFile(hSerial, line, strlen(line), &written, NULL);  // Sendet Nutzdaten
    WriteFile(hSerial, "\n", 1, &written, NULL);              // Sendet Zeilenumbruch
    return true;
}

// Liest eine komplette Zeile vom seriellen Port ein (bis '\n')
// Entfernt '\r' falls vorhanden, liest maximal (maxlen - 1) Zeichen + Nullterminator
bool serial_read_line(char* buffer, int maxlen) {
    if (!hSerial) return false;
    DWORD read;
    char c;
    int i = 0;

    while (i < maxlen - 1) {
        if (!ReadFile(hSerial, &c, 1, &read, NULL) || read == 0) break;
        if (c == '\n') break;         // Zeilenende erkannt
        if (c == '\r') continue;      // Wagenrücklauf ignorieren
        buffer[i++] = c;              // Zeichen zum Puffer hinzufügen
    }

    buffer[i] = '\0'; // Nullterminator setzen
    return i > 0;     // Rückgabe true, wenn mindestens ein Zeichen gelesen wurde
}
