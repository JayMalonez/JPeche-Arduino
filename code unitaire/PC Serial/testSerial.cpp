#define UNICODE 0
#define _UNICODE 0

#include <windows.h>
#include <iostream>
#include <string>

#include "include/json.hpp"
using json = nlohmann::json;

using namespace std;

int main()
{

    // Initializing a string object
    string str = "\\\\.\\COM10";

    // Initializing an object of wstring
    wstring temp = wstring(str.begin(), str.end());

    // Applying c_str() method on temp
    LPCWSTR wideString = temp.c_str();

    // Open COM10
    HANDLE hSerial = CreateFile(wideString,  GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error opening COM10\n";
        return 1;
    }

    // Configure serial port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    SetCommState(hSerial, &dcbSerialParams);

    // Set timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    SetCommTimeouts(hSerial, &timeouts);

    json j_msg_send, j_msg_rcv;
    j_msg_send["led"] = 0;

    while(true)
    {
        try {
            if (j_msg_send["led"] == 0)
                j_msg_send["led"] = 1;
            else
                j_msg_send["led"] = 0;

            // Send data
            string tempString = j_msg_send.dump();
            const char* tempJson = tempString.c_str();
            DWORD bytesWritten;
            WriteFile(hSerial, tempJson, strlen(tempJson), &bytesWritten, NULL);

            // Read data
            char buffer[128];
            DWORD bytesRead;
            ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);

            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                std::cout << "Received: " << buffer << std::endl;
            }

            Sleep(1000);
        }
        catch (int errorCode) {
            cout << "Error occurred: " << errorCode;
            break;
        } 
    }

    CloseHandle(hSerial);
    return 0;
}
