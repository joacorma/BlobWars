#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>
#include "blobsBack.h"
#include <ctype.h>

#define MAX_ERRORES 9
#define MIN_FILAS 5
#define MAX_FILAS 30
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 30
#define BLOQUE 5
#define SAVE 0
#define FILENAME 1

#define BORRAR_BUFFER while (getchar()!='\n')

void DosJugadores (char **vecErrores, char ***Tablero, int *filas, int *columnas, int *turno, int *opcion);
void ContraLaCompu ();
int RecuperarJuego(char *nombre, char ***Tablero, int *filas, int *columnas, int *turno, int *opcion, char **vecErrores);
void CargarErrores (char **vecErrores);
void ImprimirError (char **vecErrores, int nroError);
void PantallaInicial ();
int Pantalla11 ();
int Pantalla12 ();
void ImprimirTablero (char ***Tablero, int *filas, int *columnas);
int CapturarJugada (char ***Tablero, int *filas, int *columnas, int *turno, char **vecErrores, int *mov, char **movimiento);
char ** leerCaracteres (char **string);
int get_int ();

int main ()
{
	int opcion = 0, filas=0, columnas=0, turno=0, error;
	char *vecErrores[MAX_ERRORES], **Tablero, *nombre;

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
			case 1: DosJugadores (vecErrores, &Tablero, &filas,&columnas,&turno, &opcion);
				break;
			case 2: ContraLaCompu ();
				break;
			case 3:
				printf("%s\n", "Escriba el nombre del archivo: ");
				leerCaracteres(&nombre);
				error = RecuperarJuego (nombre,&Tablero,&filas, &columnas, &turno, &opcion,vecErrores);
				if (error==8)
					ImprimirError(vecErrores, error);
				break;
			case 4: printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
		}
	}
	return 0;
}

void DosJugadores (char **vecErrores, char ***Tablero, int *filas, int *columnas, int *turno, int *opcion)
{	
	int fin = 0, mov[4], Ganador, accion;
	srand (time (NULL));

	if(*filas==0)
	{
		*filas = Pantalla11();
		while ((*filas < MIN_FILAS) || (*filas > MAX_FILAS))
		{
			ImprimirError(vecErrores, 1);
			*filas = Pantalla11();
		}
	}
	if(*columnas==0)
	{
		*columnas = Pantalla12();
		while ((*columnas < MIN_COLUMNAS) || (*columnas > MAX_COLUMNAS))
		{
			ImprimirError(vecErrores, 2);
			*columnas = Pantalla12();
		}
	}

	printf("\n");
	printf("Jugador 1: A\n");
	printf("Jugador 2: Z\n");

	if(*Tablero==NULL)
		CrearTablero (Tablero, filas, columnas);
	printf("\n");
	
	if(*turno==0)
		*turno = rand()%2 + 1; /*RANDINT*/

	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", *turno);
		char *movimiento = NULL;

		accion = CapturarJugada (Tablero, filas, columnas, turno, vecErrores, mov, &movimiento);
		switch (accion)
		{
			case 0: break;
			case 1: 
			{
				Save (Tablero, filas, columnas, turno, movimiento, opcion);
				if (*turno == 1)
					*turno = 2;
				else
					*turno = 1;

			}
					break;
			case 2: return;

		}
		
		Jugar (Tablero, filas, columnas, turno, mov);

		if (*turno == 1)
			*turno = 2;
		else
			*turno = 1;

		fin = Fin(Tablero, filas, columnas, turno);
	}

	ImprimirTablero(Tablero, filas, columnas);
	LLenarTablero (Tablero, filas, columnas, turno);
	printf("\n");
	ImprimirTablero(Tablero, filas, columnas);
	
	Ganador = ContarBlobs (Tablero, filas, columnas);
	if (Ganador == 0)
		printf("Hubo un empate\n");
	else
		printf("El ganador es el Jugador %d\n", Ganador);
}

void ContraLaCompu ()
{
	printf("Proceso 2\n");
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
	vecErrores[8] = "El archivo indicado no fue encontrado, por favor introduzca un nuevo nombre";
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


void ImprimirTablero (char ***Tablero, int *filas, int *columnas)
{
	int i, j;
	
	system("clear");
	for (i=0; i<*filas; i++)
	{
		for (j=0; j<*columnas; j++)
			printf("%c ", (*Tablero)[i][j]);
		printf("\n");
	}
}

int CapturarJugada (char ***Tablero, int *filas, int *columnas, int *turno, char **vecErrores, int *mov, char **movimiento)
{
	int datosInvalidos = 3;
	
	
	while (datosInvalidos > 2)
	{
		printf("Introduzca un movimiento: ");
		movimiento = leerCaracteres(movimiento);
		datosInvalidos = ValidarParametros (movimiento, Tablero, filas, columnas, turno, vecErrores, mov);
		printf("%s\n", *movimiento);
		if (datosInvalidos > 2)
			ImprimirError(vecErrores, datosInvalidos);
	}
	return datosInvalidos;
}

char ** leerCaracteres (char **string)
{
	char *aux = NULL;
	int i = 0, a;
	while ((a=getchar()) != '\n')
	{
		if (i%BLOQUE == 0)
		{
			aux = realloc (*string, (i+BLOQUE)*sizeof(char));
			if (aux != NULL)
				*string = aux;
		}
		(*string)[i++] = a;
	}
	(*string)[i++] = 0;
	*string = realloc (*string, i*sizeof(char));
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
