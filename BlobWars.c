#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>
#include "blobsBack.h"

#define MAX_ERRORES 10
#define MAX_FILA 30
#define MAX_COL 30

typedef char TipoColumna[MAX_COL];
typedef TipoColumna TipoTablero[MAX_FILA];

void DosJugadores (char *vecErrores[MAX_ERRORES], TipoTablero Tablero);
void ContraLaCompu ();
void RecuperarJuego ();
void CargarErrores (char *vecErrores[MAX_ERRORES]);
void ImprimirError (char *vecErrores[MAX_ERRORES], int nroError);
void PantallaInicial ();
int Pantalla11 ();
int Pantalla12 ();


int main ()
{
	int opcion = 0;
	char *vecErrores[10];

	CargarErrores(vecErrores);
	TipoTablero Tablero;
	
	while (opcion != 4)
	{
		PantallaInicial ();
		opcion = getint("Elegir opcion: ");

		if (opcion == 1)
			DosJugadores (vecErrores, Tablero);
		else if (opcion == 2)
			ContraLaCompu ();
		else if (opcion == 3)
			RecuperarJuego ();
		else
			ImprimirError(vecErrores, 0);
	}
	
	printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
	return 0;
}

void DosJugadores (char *vecErrores[MAX_ERRORES], TipoTablero Tablero)
{	
	int filas = Pantalla11(), columnas, turno;
	srand (time (NULL));
	while ((filas < 5) || (filas > 30))
	{
		ImprimirError(vecErrores, 1);
		filas = Pantalla11();
	}

	columnas = Pantalla12();
	while ((columnas < 5) || (columnas > 30))
	{
		ImprimirError(vecErrores, 2);
		columnas = Pantalla11();
	}

	printf("Jugador 1: A\n");
	printf("Jugador 2: Z\n");
	printf("Presione cualquier tecla para comenzar el juego...\n");
	getchar();

	CrearTablero (Tablero, filas, columnas);

	turno = rand()%2 + 1;

	ImprimirTablero (Tablero, filas, columnas);
	
	/*ProcesoDosJugadores (Tablero, filas, columnas, turno);*/
}

void ContraLaCompu ()
{
	printf("Proceso 2\n");
}

void RecuperarJuego ()
{
	printf("Proceso 3\n");
}

void CargarErrores (char *vecErrores[MAX_ERRORES])
{
	vecErrores[0] = "La opcion debe ser entre 1 y 4";
	vecErrores[1] = "La cantidad de filas debe ser entre 5 y 30";
	vecErrores[2] = "La cantidad de columnas debe ser entre 5 y 30";
	vecErrores[3] = "El formato introducido es incorrecto";
}

void ImprimirError (char *vecErrores[MAX_ERRORES], int nroError)
{
	printf("ERROR: %s\n", vecErrores[nroError]);

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
	fila = getint("Ingrese la cantidad de filas del tablero (entre 5 y 30): ");
	return fila;

}

int Pantalla12 ()
{
	int col;
	col = getint("Ingrese la cantidad de columnas del tablero (entre 5 y 30): ");
	return col;
}

