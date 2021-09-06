#include <getopt.h>
#include <limits.h>
#include <sys/wait.h>
#include <pthread.h>
#include "common.h"
#define MAX_FD 2
#include <stdbool.h>

bool dflag=false;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Constantes
 	const long minute = 60;
 	const long hour = minute * 60;
 	const long day = hour * 24;
 	const double megabyte = 1024 * 1024;

void atender_cliente(int connfd);


void print_help(char *command)
{
	printf("Servidor simple de ejecución remota de comandos.\n");
	printf("uso:\n %s [-d] <puerto>\n", command);
	printf(" %s -h\n", command);
	printf("Opciones:\n");
	printf(" -h\t\t\tAyuda, muestra este mensaje\n");
	printf(" -d\t\t\tFunciona en modo daemon\n");
	
}

/* Thread routine */
void *thread(void *vargp){
	int connfd = *((int *)vargp);
	pthread_detach(pthread_self());
	free(vargp);
	atender_cliente(connfd);
	close(connfd);
	return NULL;
}


/**
 * Recoge hijos zombies...
 */
void recoger_hijos(int signal){
	while(waitpid(-1, 0, WNOHANG) >0)
		;

	return;
}


int main(int argc, char **argv)
{
	int opt;

	//Sockets
	int listenfd, *connfd;
	unsigned int clientlen;
	//Direcciones y puertos
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp, *port;
	pthread_t tid;

	while ((opt = getopt (argc, argv, "h")) != -1){
		switch(opt)
		{
			
			case 'h':
				print_help(argv[0]);
				return 0;
			default:
				fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
				fprintf(stderr, "     %s -h\n", argv[0]);
				return -1;
		}
	}

	if(!(argc == 2)){
		fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
		fprintf(stderr, "     %s -h\n", argv[0]);
		return -1;
	}else{
		// Obtiene el puerto
		int index;
		for (index = optind; index < argc; index++)
			port = argv[index];
	}
	//Valida el puerto
	int port_n = atoi(port);
	if(port_n <= 0 || port_n > USHRT_MAX){
		fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
		return -1;
	}

	//Registra funcion para recoger hijos zombies
	signal(SIGCHLD, recoger_hijos);

	//Abre un socket de escucha en port
	listenfd = open_listenfd(port);

	if(listenfd < 0)
		connection_error(listenfd);

	 if(dflag){
                //Transformando a daemon
                int i, fd0, fd1, fd2;
                pid_t pid;

                //Liberando el pront
	        if ((pid = fork()) < 0)
		        //err_quit("%s: can't fork", cmd);
		        printf("error\n");
	        else if (pid != 0) /* parent */
		        exit(0);
	        setsid();
	        for (i = 0; i < MAX_FD; i++)
		        close(i);
                //STDIN STDOUT STDERRApuntando a null
	        fd0 = open("/dev/null");
	        fd1 = dup(0);
	        fd2 = dup(0);
        }

        printf("server escuchando en puerto %s...\n", port);
	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = malloc(sizeof(int));
		*connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		/* Determine the domain name and IP address of the client */
		hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
						sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);

		printf("server conectado a %s (%s)\n", hp->h_name, haddrp);
		//atender_cliente(connfd);
		pthread_create(&tid, NULL, thread, connfd);
		//printf("server desconectando a %s (%s)\n", hp->h_name, haddrp);
		//close(connfd);
		//exit(0);
		

		//close(connfd);
	}
}

void atender_cliente(int connfd)
{
	int n;
	char buf[MAXLINE] = {0};
	char **argv;
	
	//Información del sistema
	struct sysinfo si;

	//Comunicación con cliente es delimitada con '\0'
	while(1){
		//pthread_mutex_lock(&mutex); //Inicio sección crítica
		n = read(connfd, &si, sizeof(si));
		//pthread_mutex_unlock(&mutex); //Fin de la sección critica
		if(n <= 0)
			return;

		/* Summarize interesting values. */
		pthread_mutex_lock(&mutex);
		printf("Información recibida del cliente: %d\n", connfd);
		
		printf ("system uptime : %ld days, %ld:%02ld:%02ld\n", 
	 	si.uptime / day, (si.uptime % day) / hour, 
	 	(si.uptime % hour) / minute, si.uptime % minute);
		printf ("total RAM   : %5.1f MB\n", si.totalram / megabyte);
		printf ("free RAM   : %5.1f MB\n", si.freeram / megabyte);
		printf ("shared RAM   : %5.1f MB\n", si.sharedram / megabyte);
		printf ("buffer RAM   : %5.1f MB\n", si.bufferram / megabyte);
		printf ("process count : %d\n", si.procs);
		printf ("Total SWAP  : %5.1f MB\n", si.totalswap / megabyte);
		printf ("free SWAP  : %5.1f MB\n\n", si.freeswap / megabyte);
		pthread_mutex_unlock(&mutex);

		memset(buf, 0, MAXLINE); //Encera el buffer
		
	}
}
