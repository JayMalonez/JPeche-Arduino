#define UNICODE 0
#define _UNICODE 0

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

class COM_Serial {
private:
    HANDLE hSerial;
    string portName;
    bool connected;

public:
    // Constructeur : Ouvre et configure le port
    COM_Serial(string port, DWORD baudRate = CBR_115200) {
        connected = false;
        portName = "\\\\.\\" + port;
        


        // Initializing an object of wstring
        wstring temp = wstring(portName.begin(), portName.end());

        // Applying c_str() method on temp
        LPCWSTR wideString = temp.c_str();

        hSerial = CreateFile(wideString, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (hSerial == INVALID_HANDLE_VALUE) {
            cerr << "Erreur : Impossible d'ouvrir le port " << port << endl;
            return;
        }

        // Configuration du port
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            cerr << "Erreur : Impossible d'obtenir l'etat du port." << endl;
            return;
        }

        dcbSerialParams.BaudRate = baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity   = NOPARITY;

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            cerr << "Erreur : Impossible de configurer les parametres serie." << endl;
            return;
        }

        // Configuration des timeouts
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout         = 50;
        timeouts.ReadTotalTimeoutConstant    = 50;
        timeouts.ReadTotalTimeoutMultiplier  = 10;
        SetCommTimeouts(hSerial, &timeouts);

        connected = true;
    }

    // Destructeur : Ferme proprement le handle
    ~COM_Serial() {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
        }
    }

    bool isConnected() const { return connected; }

    // Fonction writeMSG : Envoie un objet JSON
    bool writeMSG(const json& j_msg) {
        if (!connected) return false;

        string payload = j_msg.dump();
        DWORD bytesWritten;
        bool result = WriteFile(hSerial, payload.c_str(), (DWORD)payload.length(), &bytesWritten, NULL);
        
        return result && (bytesWritten == payload.length());
    }

    // Fonction readMSG : Lit les données et retourne un objet JSON
    json readMSG() {
        if (!connected) return json();

        char buffer[256]; // Augmenté un peu pour plus de sécurité
        DWORD bytesRead;
        json j_rcv;

        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                try {
                    j_rcv = json::parse(buffer);
                } catch (json::parse_error& e) {
                    // Si ce n'est pas du JSON valide, on peut retourner le texte brut ou logger l'erreur
                    cerr << "Erreur de parsing JSON : " << e.what() << endl;
                }
            }
        }
        return j_rcv;
    }
};


int main() {
    // On instancie la classe pour le port COM10
    COM_Serial mySerial("COM10");

    if (!mySerial.isConnected()) {
        return 1;
    }

    json message;
    message["LED"] = 1;
    message["BARGRAPH"] = 10;

    while (true) {
        // Envoi
        if (mySerial.writeMSG(message)) {
            cout << "Message envoye !" << endl;
        }

        // Lecture
        json response = mySerial.readMSG();
        if (!response.empty()) {
            cout << "Recu : " << response.dump(4) << endl;
        }

        // Logique alternance pour le test
        message["LED"] = (message["LED"] == 0) ? 1 : 0;

        Sleep(1000);
    }

    return 0;
}



