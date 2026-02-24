/* 
 * Auteurs: Jean-Samuel Lauzon    
 * Date: Fevrier 2022
 * Modif : Janvier 2023, Compatible VisualStudio, JpGouin
 * Modif : Janvier 2025, 115200 BAUDS, JpGouin
*/

/*------------------------------ Librairies ---------------------------------*/
#include <iostream>
#include <string>


/*-------------------------- Librairies externes ----------------------------*/
#include "include/serial/SerialPort.hpp"
#include "include/json.hpp"

using json = nlohmann::json;

/*------------------------------ Constantes ---------------------------------*/
#define BAUD 115200           // Frequence de transmission serielle
#define MSG_MAX_SIZE 1024   // Longueur maximale d'un message


/*------------------------- Prototypes de fonctions -------------------------*/
bool SendToSerial(SerialPort *arduino, json j_msg);
bool RcvFromSerial(SerialPort *arduino, std::string &msg);


/*---------------------------- Variables globales ---------------------------*/
SerialPort * arduino; //doit etre un objet global!

/*----------------------------- Fonction "Main" -----------------------------*/
int main(){
    std::string raw_msg;

    // Initialisation du port de communication
    std::string com;
    //std::cout << "Entrer le port de communication du Arduino: ";
    //std::cin >> com;
    com = "10";
    arduino = new SerialPort(com.c_str(), BAUD);
    
    //const char com = "\\\\.\\COM3";
    //SerialPort arduino = SerialPort("\\\\.\\COM3");
    if(!arduino->isConnected()){
        std::cerr << "Impossible de se connecter au port "<< std::string(com) <<". Fermeture du programme!" <<std::endl;
        exit(1);
    }
    
    // Structure de donnees JSON pour envoie et reception
    int led_state = 1;
    json j_msg_send, j_msg_rcv;

    // Boucle pour tester la communication bidirectionnelle Arduino-PC
    for(int i=0; i<10; i++){
        // Envoie message Arduino
        j_msg_send["led"] = led_state;
        if(!SendToSerial(arduino, j_msg_send)){
            std::cerr << "Erreur lors de l'envoie du message. " << std::endl;
        }
        // Reception message Arduino
        j_msg_rcv.clear(); // effacer le message precedent
        if(!RcvFromSerial(arduino, raw_msg)){
            std::cerr << "Erreur lors de la reception du message. " << std::endl;
        }
        
        // Impression du message de l'Arduino si valide
        if(raw_msg.size()>0){
            //cout << "raw_msg: " << raw_msg << endl;  // debug
            // Transfert du message en json
            j_msg_rcv = json::parse(raw_msg);
            std::cout << "Message de l'Arduino: " << j_msg_rcv << std::endl;
        }
        
        //Changement de l'etat led
        led_state = !led_state;

        // Bloquer le fil pour environ 1 sec
        Sleep(1000); // 1000ms
    }
    return 0;
}



/*---------------------------Definition de fonctions ------------------------*/
bool SendToSerial(SerialPort *arduino, json j_msg){
    // Return 0 if error
    std::string msg = j_msg.dump();
    bool ret = arduino->writeSerialPort(msg.c_str(), msg.length());
    return ret;
}


bool RcvFromSerial(SerialPort *arduino, std::string &msg){
    // Return 0 if error
    // Message output in msg
    std::string str_buffer;
    char char_buffer[MSG_MAX_SIZE];
    int buffer_size;

    msg.clear(); // clear string
    // Read serialport until '\n' character (Blocking)

    // Version fonctionnel dans VScode, mais non fonctionnel avec Visual Studio
 
    while(msg.back()!='\n'){
        if(msg.size()>MSG_MAX_SIZE){
            return false;
        }

        buffer_size = arduino->readSerialPort(char_buffer, MSG_MAX_SIZE);
        str_buffer.assign(char_buffer, buffer_size);
        msg.append(str_buffer);
    }


    // Version fonctionnelle dans VScode et Visual Studio
    buffer_size = arduino->readSerialPort(char_buffer, MSG_MAX_SIZE);
    str_buffer.assign(char_buffer, buffer_size);
    msg.append(str_buffer);

    //msg.pop_back(); //remove '/n' from string

    return true;
}

