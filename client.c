#include <getopt.h>
#include <limits.h>

#include <unistd.h>


#include "common.h"

void print_help(char *command)
{
	printf("Cliente simple envío de información del sistema.\n");
	printf("uso:\n %s <hostname> <puerto>\n", command);
	printf("     -t <tiempo de actualización en segundos>\n");
	printf(" %s -h\n", command);
	printf("Opciones:\n");
	printf(" -h\t\t\tAyuda, muestra este mensaje\n");
}

int main(int argc, char **argv)
{
	int opt;
	extern char* optarg;
	extern int optopt;
	
	int timeS=5;
	
	//Socket
	int clientfd;
	//Direcciones y puertos
	char *hostname, *port;
	

	//Lectura desde consola
	char *linea_consola;
	//char read_buffer[MAXLINE + 1] = {0};
	//size_t max = MAXLINE;
	ssize_t n = 0;
	
	//Información del sistema
	struct sysinfo si;
	
	//Constantes
 	const long minute = 60;
 	const long hour = minute * 60;
 	const long day = hour * 24;
 	const double megabyte = 1024 * 1024;
 	const long ms=1000000;

	while ((opt = getopt (argc, argv, "ht:")) != -1){
		switch(opt)
		{
			case 'h':
				print_help(argv[0]);
				return 0;
			case 't':
				hostname = argv[1];
				port = argv[2];
				timeS=atoi(optarg);
				break;	
			case '?':
				if(optopt == 't' && isprint(optopt)){
					printf("tiempo por defecto es 5.");
					break;
				}else if(isprint(optopt)){
					fprintf(stderr, "Opcion desconocida '-%c'.\n",optopt);
				}else{
					fprintf(stderr, "ERR>> Caracter desconocido '\\x%x'.\n",optopt);
				}
			default:
			fprintf(stderr, "ERROR default");
				fprintf(stderr, "uso: %s <hostname> <puerto>\n", argv[0]);
				fprintf(stderr,"     -t <tiempo de actualización en segundos>\n");
				fprintf(stderr, "     %s -h\n", argv[0]);
				return -1;
		}
	}

	if(argc !=5){
	
		fprintf(stderr, "uso: %s <hostname> <puerto>\n", argv[0]);
		fprintf(stderr,"-t <tiempo de actualización en segundos>\n");
		fprintf(stderr, "     %s -h\n", argv[0]);
		return -1;
	}else{
		
		//printf("TIEMPO %d",timeS);
	}

	//Valida el puerto
	int port_n = atoi(port);
	if(port_n <= 0 || port_n > USHRT_MAX){
		fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
		return -1;
	}

	//Se conecta al servidor retornando un socket conectado
	clientfd = open_clientfd(hostname, port);

	if(clientfd < 0)
		connection_error(clientfd);

	printf("Conectado exitosamente a %s en el puerto %s.\n", hostname, port);

	//linea_consola = (char *) calloc(1, MAXLINE);
	
	//l = getline(&linea_consola, &max, stdin); //lee desde consola*/
	while(1){
		sysinfo (&si);
		n = write(clientfd, &si, sizeof(si)); //Envia al servidor
		printf ("Información enviada al servidor....\n");
		
		
		usleep(timeS*ms);
 	}


	printf("Desconectando...\n");
	free(linea_consola);
	close(clientfd);
}
