#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>
#include <math.h>
typedef struct
	{	
		int filas;
		int columnas;
		int **matriz;
	} tipoMatriz;
#include "blobsBackV3.h"

#define MAX_ERRORES 8
#define MIN_FILAS 5
#define MAX_FILAS 30
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 30
#define BLOQUE 5
#define SAVE 0
#define FILENAME 1

#define BORRAR_BUFFER while (getchar()!='\n')
#define GENERAR_COORDENADA(x,y) (x*100 + y)
#define GENERAR_FILA(x) (floor(x/100))
#define GENERAR_COLUMNA(x) (x%100)

void ProcesoJuego (char **vecErrores, int opcion, char *idJugador);
void RecuperarJuego ();
void CargarErrores (char **vecErrores);
void ImprimirError (char **vecErrores, int nroError);
void PantallaInicial ();
int Pantalla11 ();
int Pantalla12 ();
void ImprimirTablero (tipoMatriz *Tablero, char *idJugador);
int CapturarJugada (tipoMatriz *Tablero, int turno, char **vecErrores, int *mov);
char * leerCaracteres ();
int get_int ();

int main ()
{
	int opcion = 0;
	char *vecErrores[MAX_ERRORES], idJugador[] = {'0', 'A', 'Z'};

	CargarErrores(vecErrores);
	
	while (opcion != 4)
	{
		PantallaInicial ();
		printf("Elegir opcion: ");
		opcion = get_int();
		while (opcion < 1 || opcion > 4)
		{
			ImprimirError(vecErrores, 0);
			printf("Elegir opcion: ");
			opcion = get_int();
		}

		switch(opcion)
		{
			case 1: ProcesoJuego (vecErrores, opcion, idJugador);
				break;
			case 2: ProcesoJuego (vecErrores, opcion, idJugador);
				break;
			case 3: RecuperarJuego ();
				break;
			case 4: printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
		}
	}
	return 0;
}

void ProcesoJuego (char **vecErrores, int opcion, char *idJugador)
{	
	tipoMatriz Tablero;
	int turno, fin = 0, mov[4], Ganador, accion;
	srand (time (NULL));
	
	Tablero.filas = Pantalla11();
	while ((Tablero.filas < MIN_FILAS) || (Tablero.filas > MAX_FILAS))
	{
		ImprimirError(vecErrores, 1);
		Tablero.filas = Pantalla11();
	}

	Tablero.columnas = Pantalla12();
	while ((Tablero.columnas < MIN_COLUMNAS) || (Tablero.columnas > MAX_COLUMNAS))
	{
		ImprimirError(vecErrores, 2);
		Tablero.columnas = Pantalla12();
	}

	printf("\n");
	printf("Jugador 1: %c\n", idJugador[1]);
	printf("Jugador 2: %c\n", idJugador[2]);

	Tablero.matriz = CrearTablero (Tablero.filas, Tablero.columnas);
	printf("\n");
	
	turno = rand()%2 + 1; /*RANDINT*/

	while (fin == 0)
	{
		ImprimirTablero(&Tablero, idJugador);
		printf("Turno Jugador %d\n", turno);

		if (opcion == 2 && turno == 2)
			accion = JugadaComputadora (&Tablero, mov);
		else
			accion = CapturarJugada (&Tablero, turno, vecErrores, mov);
		
		switch (accion)
		{
			case 0: 
			{
				EjecutarJugada (&Tablero, turno, mov);

				if (turno == 1)
					turno = 2;
				else
					turno = 1;

				fin = Fin(&Tablero, turno);
			}
			break;
			
			case 1: Save ();
			break;
			
			case 2: 
			{
				fin = 1;
				printf("La partida ha sido terminada. Pulse enter para continuar...\n");
				getchar();
			}
			break;

			case 3:
			{
				fin = 1;
				accion = 0;
			}
		}
	}

	if (accion == 0)
	{
		ImprimirTablero(&Tablero, idJugador);
		LLenarTablero (&Tablero, turno);
		printf("\n");
		ImprimirTablero(&Tablero, idJugador);
		
		Ganador = ContarBlobs (&Tablero);
		if (Ganador == 0)
			printf("Hubo un empate\n");
		else
			printf("El ganador es el Jugador %d\n", Ganador);
		
		printf("Pulse enter para volver al menu principal...\n");
		getchar();
	}
}

void RecuperarJuego ()
{
	printf("Proceso 3\n");
}

void CargarErrores (char **vecErrores)
{
	vecErrores[0] = "La opcion debe ser entre 1 y 4";
	vecErrores[1] = "La cantidad de filas debe ser entre 5 y 30";
	vecErrores[2] = "La cantidad de columnas debe ser entre 5 y 30";
	vecErrores[3] = "El formato introducido es incorrecto";
	vecErrores[4] = "No existe esa poscicion en el tablero";
	vecErrores[5] = "Ese movimiento no esta dentro del rango de movimientos posibles";
	vecErrores[6] = "Ese casillero no corresponde a un Blob de tu color";
	vecErrores[7] = "Ese casillero ya esta ocupado";
}

void ImprimirError (char **vecErrores, int nroError)
{
	printf("ERROR: %s\n\n", vecErrores[nroError]);

}

void PantallaInicial ()
{
	system("clear");

	printf("BLOB WARS\n\n\n");
	printf("1. Juego de dos jugadores\n");
	printf("2. Juego contra la computadora\n");
	printf("3. Recuperar juego grabado\n");
	printf("4. Terminar\n\n");
}

int Pantalla11 ()
{
	int fila;
	printf("Ingrese la cantidad de filas del tablero (entre 5 y 30): ");
	fila = get_int();
	return fila;

}

int Pantalla12 ()
{
	int col;
	printf("Ingrese la cantidad de columnas del tablero (entre 5 y 30): ");
	col = get_int();
	return col;
}


void ImprimirTablero (tipoMatriz *Tablero, char *idJugador)
{
	int i, j;
	
	system("clear");
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
			printf("%c ", idJugador[Tablero->matriz[i][j]]);
		printf("\n");
	}
}

int CapturarJugada (tipoMatriz *Tablero, int turno, char **vecErrores, int *mov)
{
	int datosInvalidos = 3;
	char *movimiento = NULL;
	
	while (datosInvalidos > 2)
	{
		printf("Introduzca un movimiento: ");
		movimiento = leerCaracteres(movimiento);
		datosInvalidos = ValidarParametros (movimiento, Tablero, turno, vecErrores, mov);
		if (datosInvalidos > 2)
			ImprimirError(vecErrores, datosInvalidos);
	}
	return datosInvalidos;
}

char * leerCaracteres (char *string)
{
	char *aux = NULL;
	int i = 0, a;
	while ((a=getchar()) != '\n')
	{
		if (i%BLOQUE == 0)
		{
			aux = realloc (string, (i+BLOQUE)*sizeof(char));
			if (aux != NULL)
				string = aux;
		}
		string[i++] = a;
	}
	string[i++] = 0;
	string = realloc (string, i*sizeof(char));
	return string;
}

int get_int ()
{
	int a, entero = 0;

	while ((a=getchar()) != '\n')
	{
		if (isdigit(a))
			entero = 10*entero + (a - '0');
		else
		{	
			BORRAR_BUFFER;
			return -1;
		}
	}

	return entero;

}