//Grupo 1: Caldez, Rosalba. Iriarte, Josefina. Orellana, Gonzalo Leandro. Perelmut, Valentina

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> //Se incluye para identificar IPv4
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define size 200 //Se define una constante para el tamaño del mensaje, por si se lo quisiera cambiar

void FechaHora(); //Funcion para mostrar fecha y hora de recepcion del mensaje

void FechaHora() {
    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);
    char output[128];
    strftime(output,128,"\n\nFecha y hora de recepcion: %d/%m/%y %H:%M:%S",local_time);
    printf("%s\n",output); 
    return;
}

int main()
{
    int descriptor; //Descriptor del socket
    struct sockaddr_in server; //Estructuras para direccion
    char server_message[size]; //Mensaje de hasta 200 caracteres
    int server_length = sizeof(server); //Tener a mano el tamaño de la estructura
    int active = 1;
    
	//Limpieza de buffers
    memset(server_message, '\0',size);

    //Se crea el socket UDP con su indentificador
    descriptor = socket(PF_INET, SOCK_DGRAM, 0);

    if (descriptor < 0) {
        printf("Error creando el socket. \n");
        return -1;
    }
    printf("Socket creado con exito. Numero: %i \n",descriptor);

    //Se configura el puerto e IP del server
    server.sin_family = AF_INET;
    server.sin_port = htons(2500);
    server.sin_addr.s_addr = INADDR_ANY; //Configura IP de la pc


    if(bind(descriptor, (struct sockaddr*)&server,server_length) < 0) {
        printf("Bind fallido. \n"); 
    }
    printf("Bind realizado con exito. \n");   
    

    printf("\n**********Bienvenido/a**********\n");

    while(active) {
       
        if (recvfrom(descriptor,server_message,sizeof(server_message), 0,(struct sockaddr*)&server,&server_length) < 0 ) {
            printf("No se puede recibir el mensaje. \n");
            return -1;   
        }
        
        FechaHora();
        printf("IP origen: %s, puerto origen: %i\n",inet_ntoa(server.sin_addr), ntohs(server.sin_port));
        printf("El mensaje recibido es: %s \n",server_message);
        memset(server_message, '\0',size);
            
    }
   
return 0;

}

