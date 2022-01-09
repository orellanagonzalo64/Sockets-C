//Grupo 1: Caldez, Rosalba. Iriarte, Josefina. Orellana, Gonzalo Leandro. Perelmut, Valentina

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> //Se incluye para identificar IPv4
#include <stdlib.h>
#include <errno.h>

#define size 200 //Se define una constante para el tamaño del mensaje, por si se lo quisiera cambiar

int main()
{
    int descriptor; //Descriptor del socket
    struct sockaddr_in server, client; //Estructuras para las direcciones
    char server_message[size]; //Mensaje de hasta 200 caracteres
    int server_length = sizeof(server); //Tener a mano el tamaño de la estructura
    int client_length = sizeof(client); //Tener a mano el tamaño de la estructura
    int active = 1; //Para el while, por si queremos salir de el
    int broadcastPermission = 1;
    
	//Limpieza de buffers
    memset(server_message, '\0',size);

    //Se crea el socket UDP con su indentificador
    descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (descriptor < 0) {
        printf("Error creando el socket. \n");
        return -1;
    }
    printf("Socket creado con exito. Numero: %i \n",descriptor);

    //Se configura el puerto e IP del server
    server.sin_family = AF_INET; //IPv4
    server.sin_port = htons(50000 + rand()%100); //Se suma un random para variar del 50.000 al 50.100
    server.sin_addr.s_addr = INADDR_ANY; //Configura IP de la pc


	//Habilitar mecanismo de broadcast
    if(setsockopt(descriptor,SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission,sizeof(broadcastPermission)) < 0 ) {
        printf("Error de habilitacion de broadcast. \n");
        return -1;
    }

    //Se configura el puerto e IP del cliente
    client.sin_family = AF_INET; //IPv4
    client.sin_port = htons(2500); //Puerto del cliente
    client.sin_addr.s_addr = htonl(INADDR_BROADCAST); //Configura IP de broadcast

    printf("El puerto del servidor es: %i \n", ntohs(server.sin_port));

    printf("El puerto del cliente es: %i \n", ntohs(client.sin_port));


    if(bind(descriptor, (struct sockaddr*)&server,server_length) < 0) {
        printf("Bind fallido. \n"); 
    }
    printf("Bind realizado con exito. \n");   

    printf("\n**********Bienvenido/a**********\n");

    while(active) {
	    do {
			printf("\nEscriba el mensaje a enviar (maximo 200 caracteres): \n");   
	        fflush(stdin);
	        gets(server_message);
	        
	        //Si se quiere mandar un mensaje vacio o de mas de 200 caracteres,
	        //da un mensaje de error y pide que se ingrese nuevamente el mensaje
			if(strlen(server_message) == 0 || strlen(server_message) > size) {
	        	printf("Longitud no valida.\n");
			}
	
	    }while(strlen(server_message) == 0 || strlen(server_message) > size);
        
	    if (sendto(descriptor,server_message,strlen(server_message), 0,(struct sockaddr*)&client,client_length) < 0 ) {
	        printf("No se puede enviar el mensaje. \n");
	        return -1;
	
	    }
	    
        printf("Mensaje enviado con exito. \n");
        memset(server_message, '\0', size);
	}

return 0;

}
