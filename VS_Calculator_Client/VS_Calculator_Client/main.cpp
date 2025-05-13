#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>  // Für Sleep unter Windows
#include "serial.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Nutzung: " << argv[0] << " <SERIAL_PORT>" << std::endl;
        return 1;
    }

    std::string port = argv[1];
    if (!serial_open(port.c_str())) {
        std::cerr << "Fehler: Konnte Port " << port << " nicht öffnen." << std::endl;
        return 1;
    }

    // Initialzeile vom Arduino abfangen (z. B. "Bereit für Eingaben ...")
    char startupMsg[128];
    serial_read_line(startupMsg, sizeof(startupMsg));

    std::ofstream log("log.txt", std::ios::app);
    std::cout << "Rechenausdruck eingeben (z. B. 12.5 * 3; 1,2+2,525), Nachkommastellen [0-10] (mit z.B. 'SETPREC 2') oder 'exit' zum Beenden:" << std::endl;

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        serial_write_line(input.c_str());
        Sleep(50);  // kurze Wartezeit zur Synchronisation

        char response[128];
        if (serial_read_line(response, sizeof(response))) {
            std::cout << "Antwort: " << response << std::endl;
            if (log.is_open()) {
                log << "Eingabe: " << input << std::endl;
                log << "Antwort: " << response << std::endl << std::endl;
            }
        }
        else {
            std::cout << "Fehler beim Empfangen der Antwort." << std::endl;
        }
    }

    log.close();
    serial_close();
    return 0;
}