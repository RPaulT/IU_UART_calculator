#include <iostream>     // Für Ein- und Ausgabe im Terminal
#include <fstream>      // Für Dateioperationen (Log-Datei)
#include <string>       // Für die Verwendung von std::string
#include <windows.h>    // Für Sleep-Funktion unter Windows
#include "serial.h"     // Eigene Headerdatei für serielle Kommunikation

int main(int argc, char* argv[]) {
    // Prüfen, ob der COM-Port als Argument übergeben wurde
    if (argc < 2) {
        std::cerr << "Nutzung: " << argv[0] << " <SERIAL_PORT>" << std::endl;
        return 1;
    }

    std::string port = argv[1];

    // Versucht, den seriellen Port zu öffnen
    if (!serial_open(port.c_str())) {
        std::cerr << "Fehler: Konnte Port " << port << " nicht öffnen." << std::endl;
        return 1;
    }

    // Liest die Startnachricht des Mikrocontrollers (z.B. "Bereit für Eingaben ...")
    char startupMsg[128];
    serial_read_line(startupMsg, sizeof(startupMsg));

    // Öffnet (oder erstellt) die Log-Datei zum Mitschreiben der Ein- und Ausgaben
    std::ofstream log("log.txt", std::ios::app);

    std::cout << "Rechenausdruck eingeben (z. B. 12.5 * 3; 1,2+2,525), Nachkommastellen [0-10] (mit z.B. 'SETPREC 2') oder 'exit' zum Beenden:" << std::endl;

    std::string input;

    // Haupt-Eingabeschleife
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);     // Benutzereingabe lesen

        if (input == "exit") break;        // Bei 'exit' das Programm beenden

        serial_write_line(input.c_str());  // Eingabe an den Mikrocontroller senden
        Sleep(50);                         // Kurze Pause, um Antwort abzuwarten

        char response[128];
        if (serial_read_line(response, sizeof(response))) {
            std::cout << "Antwort: " << response << std::endl;

            // Falls Log-Datei geöffnet ist, Ein- und Ausgabe protokollieren
            if (log.is_open()) {
                log << "Eingabe: " << input << std::endl;
                log << "Antwort: " << response << std::endl << std::endl;
            }
        }
        else {
            std::cout << "Fehler beim Empfangen der Antwort." << std::endl;
        }
    }

    // Ressourcen freigeben
    log.close();
    serial_close();

    return 0;
}
