#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getnum.h"
#include <string.h>


#define MAX_ERRORES 10
#define MAX_MOVIMIENTOS 50

#define BORRAR_BUFFER while (getchar()!='\n')


void ImprimirError (char **vecErrores, int nroError);
void DosJugadores (char **vecErrores, char ***Tablero);
void ContraLaCompu ();
void RecuperarJuego ();
void CargarErrores (char **vecErrores);
void PantallaInicial ();
int Pantalla11 ();
int Pantalla12 ();
#include "blobsBack.h"

int main ()
{
	int opcion = 0;
	char *vecErrores[10];
	char **Tablero;

	CargarErrores(vecErrores);
	
	while (opcion != 4)
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
	ImprimirTablero (Tablero, filas, columnas);
	printf("\n");
	
	turno = rand()%2 + 1; /*RANDINT*/
	
	ProcesoDosJugadores (Tablero, filas, columnas, turno, vecErrores);
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
