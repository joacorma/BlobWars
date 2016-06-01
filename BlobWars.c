#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>
#include "blobsBack.h"

#define MAX_ERRORES 10
#define MAX_MOVIMIENTOS 50
#define MIN_FILAS 5
#define MAX_FILAS 30
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 30

#define BORRAR_BUFFER while (getchar()!='\n')

void DosJugadores (char **vecErrores, char ***Tablero);
void ContraLaCompu ();
void RecuperarJuego ();
void CargarErrores (char **vecErrores);
void ImprimirError (char **vecErrores, int nroError);
void PantallaInicial ();
int Pantalla11 ();
int Pantalla12 ();
void ImprimirTablero (char ***Tablero, int filas, int columnas);
void CapturarJugada (char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov);

int main ()
{
	int opcion = 0;
	char *vecErrores[MAX_ERRORES], **Tablero;

	CargarErrores(vecErrores);
	
	while (opcion != '4')
	{
		PantallaInicial ();
		printf("Elegir opcion: ");
		opcion = getchar();
		

		switch(opcion)
		{
			case '1': DosJugadores (vecErrores, &Tablero);
				break;
			case '2': ContraLaCompu ();
				break;
			case '3': RecuperarJuego ();
				break;
			case '4': printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
				break;
			default: ImprimirError(vecErrores, 0);
		}
	}
	return 0;
}

void DosJugadores (char **vecErrores, char ***Tablero)
{	
	int filas = Pantalla11(), columnas, turno, fin = 0, mov[4];
	srand (time (NULL));
	
	while ((filas < MIN_FILAS) || (filas > MAX_FILAS))
	{
		ImprimirError(vecErrores, 1);
		filas = Pantalla11();
	}

	columnas = Pantalla12();
	while ((columnas < MIN_COLUMNAS) || (columnas > MAX_COLUMNAS))
	{
		ImprimirError(vecErrores, 2);
		columnas = Pantalla11();
	}

	printf("\n");
	printf("Jugador 1: A\n");
	printf("Jugador 2: Z\n");

	CrearTablero (Tablero, filas, columnas);
	printf("\n");
	
	turno = rand()%2 + 1; /*RANDINT*/

	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", turno);

		CapturarJugada (Tablero, filas, columnas, turno, vecErrores, mov);
		
		Jugar (Tablero, filas, columnas, turno, mov);

		if (turno == 1)
			turno = 2;
		else
			turno = 1;

		fin = Fin(Tablero, filas, columnas, turno);
	}
}

void ContraLaCompu ()
{
	printf("Proceso 2\n");
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
	printf("ERROR: %s\n", vecErrores[nroError]);

}

void PantallaInicial ()
{
	/*system("clear");*/

	printf("BLOB WARS\n\n\n");
	printf("1. Juego de dos jugadores\n");
	printf("2. Juego contra la computadora\n");
	printf("3. Recuperar juego grabado\n");
	printf("4. Terminar\n\n");
}

int Pantalla11 ()
{
	int fila;
	fila = getint("Ingrese la cantidad de filas del tablero (entre 5 y 30): ");
	return fila;

}

int Pantalla12 ()
{
	int col;
	col = getint("Ingrese la cantidad de columnas del tablero (entre 5 y 30): ");
	return col;
}


void ImprimirTablero (char ***Tablero, int filas, int columnas)
{
	int i, j;
	
	/*system("clear");*/
	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
			printf("%c ", (*Tablero)[i][j]);
		printf("\n");
	}
}

void CapturarJugada (char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov)
{
	int datosInvalidos = 1;
	char movimiento[MAX_MOVIMIENTOS];
	
	while (datosInvalidos != 0)
	{
		printf("Introduzca un movimiento: ");
		scanf("%s", movimiento);
		BORRAR_BUFFER;
		datosInvalidos = ValidarParametros (movimiento, Tablero, filas, columnas, turno, vecErrores, mov);
		if (datosInvalidos != 0)
			ImprimirError(vecErrores, datosInvalidos);
	}
}
