#ifndef SERVER_SOCKETSERVER_H
#define SERVER_SOCKETSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>
using namespace std;

//estructura para repreesntar a los clientes
struct dataSocket{
    int descriptor;
    sockaddr_in info;
};

class SocketServer{
public:
    SocketServer();
    void run(); //inicia lo que tiene que ver con el server (aceptar clientes, crear el socket, recibir mensajes, etc)
    void setMensaje(const char* msn);

private:
    int descriptor; //identifica a los sistemas
    sockaddr_in info; //informacion del socket servidor
    vector<int> clientes; //almacena los clientes que se conecten

    bool crearSocket();
    bool enlazarKernel(); // escucha clientes y define cuantos va a escuchar
    static void* controladorCliente(void* obj); //lleva el control de los mensajes de los clientes, cada cliente es un hilo

};
#endif //SERVER_SOCKETSERVER_H