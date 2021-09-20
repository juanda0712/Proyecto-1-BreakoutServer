#include "SocketServer.h"

SocketServer::SocketServer() {}

bool SocketServer::crearSocket() {
    //Crear un descriptor
    descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (descriptor < 0) //valida la creacion del socket
        return false;
    info.sin_family = AF_INET; //IPV4 tipo de conexion
    info.sin_addr.s_addr = INADDR_ANY; //acepta a cualquiera con mi ip y mi puerto
    info.sin_port = htons(4050); //definimos el puerto
    memset(&info.sin_zero, 0, sizeof(info.sin_zero)); //limpiamos la estructura
    return true;
}

bool SocketServer::enlazarKernel() {
    if ((bind(descriptor, (sockaddr *)&info, (socklen_t)sizeof(info))) < 0) //enlaza al socket con el kernel
    return false;
    // escuchar a los clientes
    listen(descriptor,4);
    return true;
}

void SocketServer::run() {
    //validar errores al crear el socket y enlazar kernel
    if (!crearSocket())
        throw string("Hubo un error al crear el socket");

    if (!enlazarKernel())
        throw string("Hubo un error al enlazar el kernel");

    //ciclo infinito para aceptar a los clientes
    while(true){
        dataSocket clientData;
        socklen_t clientLen = sizeof(clientData.info);
        cout << "Esperando a que conecte un cliente" << endl;
        clientData.descriptor = accept(descriptor,(sockaddr *)&clientData.info,&clientLen); //funcion bloqueante

        if (clientData.descriptor < 0){
            cout << "Error al aceptar el cliente" << endl;
            break;
        }
        else{
            clientes.push_back(clientData.descriptor);
            cout << "Cliente conectado" << endl;

            pthread_t hilo;
            pthread_create(&hilo,0,SocketServer::controladorCliente,(void *)&clientData);
            pthread_detach(hilo);
        }
    }
    close(descriptor);
}

void* SocketServer::controladorCliente(void* obj) {
    dataSocket* clientData = (dataSocket * )obj;
    while(true){
        string mensaje;
        char buffer[1024] = {0};
        while(true){
            memset(buffer,0,1024);
            int bytes = recv(clientData->descriptor, buffer, 1024, 0); //funcion bloqueante
            mensaje.append(buffer, bytes);
            if (bytes <= 0 ){
                close(clientData->descriptor);
                pthread_exit(NULL);
            }
            if(bytes < 1024){
                break;
            }
        }
        cout << mensaje << endl; //mensaje del cliente
        //mensaje es la variable que me llega del cliente y tengo que ver que hago con ella en el servidor
        // se puede usar para el juego o para una clase
    }
    close(clientData->descriptor);
    pthread_exit(NULL);
}