# So-Proyecto-Final
Se requiere implementar una versión en modo texto de una solución de monitoreo
en tiempo real de métricas de varios sistemas GNU/Linux.
- (1) Existirán N clientes que enviarán diversas métricas de sus sistemas al
servidor.
- (2) Cada mensaje contendrá diversas métricas de un sistema en un tiempo
determinado.
- (3) Definir mínimo 6 métricas a recolectar por sistema. Ejm: % de espacio
usado y disponible de la memoria principal, secundaria, % de uso de la CPU,
etc.
- (4) Considerar definir diversas alertas (mínimo 2) que al superar un threshold
pueda enviar un mensaje vía mensaje Whatsapp (Sugerencia API Twilio).
## Diagrama de Solución
![DiagramaSolución drawio](https://user-images.githubusercontent.com/50672069/132141731-b89281b7-b9d2-42c6-bd7c-cbd264c553ff.png)

## Capturas de la solución
#### **Implementación de paso de parámetros desde la terminal**
Del lado del cliente se debe pasar el puerto, la ip y el tiempo de actualización para enviar datos al servidor
![image](https://user-images.githubusercontent.com/50672069/132160040-5c386064-72cd-4baa-8460-7da1166fe192.png)

Del lado del servidor se debe especificar el puerto en el cual va a correr el servidor

![image](https://user-images.githubusercontent.com/50672069/132160298-d0cba981-c142-4ef5-a37f-60ddefcda89b.png)

#### **Implementación de la comunicación cliente-servidor mediante sockets** 
Código del lado del servidor para aceptar la conexión de los diferentes clientes. El sercidor correrá en local host (`127.0.0.1`)
![image](https://user-images.githubusercontent.com/50672069/132159352-16362950-45e8-4a5c-968c-aa461d44aed4.png)

#### **Implementación de hilos**
Función que crea un hilo para atender a cada uno de los clientes que se conecten al servidor
![image](https://user-images.githubusercontent.com/50672069/132159886-005793ad-a41e-4540-9ef5-26ee220653e5.png)


Código para conectar un cliente al servidor de acuerdo a la ip y el puerto especificado 
![image](https://user-images.githubusercontent.com/50672069/132159541-2b0fd4a7-15dc-403f-aa5a-186e61c07a83.png)

#### **Impresión de datos de acuerdo a un tiempo de actualización**
Del lado del cliente se envía información al servidor de acuerdo a los segundos especificados como parámetro por el usuario.

![image](https://user-images.githubusercontent.com/50672069/132160509-31c7c98e-af8d-4ba1-a4b4-a9a6de0c506b.png)


## Video de ejecución
## Bibiliografía
- http://www.chuidiang.org/clinux/sockets/mensajes.php
- https://www.informit.com/articles/article.aspx?p=23618&seqNum=15
- https://man7.org/linux/man-pages/man2/sysinfo.2.html
