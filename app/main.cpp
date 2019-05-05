#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 /*incluimos la libreria para obtener el tiempo*/
/* cabecera de las llamadas a MPI */
#include <mpi.h>
#include <ctime>

/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por
 * el proceso 0, que los imprime
 */
int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    char mensaje[100]; /* mensaje  */
    MPI_Status estado; /* devuelve estado al recibir*/
    //tiempo en base al sistema
    time_t now = time(0);
    // conversiona  string
    char* dt = ctime(&now);


    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan
     * ejecutando nuestro porgrama
     */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    //esto seria para obtener el nombre del equipo.
    char nombre_procesador[MPI_MAX_PROCESSOR_NAME];
    int nombre_largo;
    MPI_Get_processor_name(nombre_procesador, &nombre_largo);

    if (mi_rango != 0) {
        /* Crea mensaje */
        sprintf(mensaje, "Saludos del procesador %d! en la maquina %s con fecha %s", mi_rango , nombre_procesador, dt);
        //sprintf(mensaje, "Saludos desde la maquina %s" , nombre_procesador);
        dest = 0;
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }

    /* Termina con MPI. Recordemos que después de
     * esta llamada no podemos llamar a funciones
     * MPI, ni siquiera de nuevo a MPI_Init
     */
    MPI_Finalize();
    return EXIT_SUCCESS;
}
