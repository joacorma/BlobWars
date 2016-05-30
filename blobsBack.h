#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>

#define MAX_ERRORES 10
#define MAX_FILA 30
#define MAX_COL 30
#define MAX_MOVIMIENTOS 50

#define BORRAR_BUFFER while (getchar()!='\n')

typedef char TipoColumna[MAX_COL];
typedef TipoColumna TipoTablero[MAX_FILA];

void CapturarJugada (TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES], int F1, int C1, int F2, int C2);
int ValidarParametros (char movimiento[], TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES], int F1, int C1, int F2, int C2);
int CalcularDistancia (int F1, int C1, int F2, int C2);
int Disponible (TipoTablero Tablero, int fila, int columna);
void Save ();

void ImprimirTablero (TipoTablero Tablero, int filas, int columnas)
{
	int i, j;
	/*system("clear");*/
	
	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
			printf("%c ", Tablero[i][j]);
		printf("\n");
	}
}

void CrearTablero (TipoTablero Tablero, int filas, int columnas)
{
	int i, j;

	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
			Tablero[i][j] = '0';
	}

	Tablero[0][0] = 'A';
	Tablero[0][columnas-1] = 'Z';
	Tablero[filas-1][0] = 'A';
	Tablero[filas-1][columnas-1] = 'Z';
}

void ProcesoDosJugadores (TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES])
{
	int fin = 0, F1, C1, F2, C2;
	/*char movimiento[MAX_MOVIMIENTOS];*/ 

	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", turno);

		/*movimiento[] = */CapturarJugada (Tablero, filas, columnas, turno, vecErrores, F1, C1, F2, C2);
		printf("%d %d %d %d\n", F1, C1, F2, C2);

		if (turno == 1)
			turno = 2;
		else
			turno = 1;
	}
}

void CapturarJugada (TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES], int F1, int C1, int F2, int C2)
/* Si quiero retornar un vector de chars: char * CapturarJugada. */ 
/* DI ACA PODRIAMOS HACER EL TABERO COMO UN PUNTERO EN VEZ DE UNA MATRIZ //
// ASI NO TENEMOS QUE PASARLE LAS FILAS Y LAS COLUMNAS CADA VEZ QUE VA A //
// CAPTURAR LA JUGADA, NO?//
{
	int datosInvalidos = 1;
	char movimiento[MAX_MOVIMIENTOS];
	
	while (datosInvalidos == 1)
	{
		printf("Introduzca un movimiento: ");
		scanf("%s", movimiento);
		BORRAR_BUFFER;
		datosInvalidos = ValidarParametros (movimiento, Tablero, filas, columnas, turno, vecErrores, F1, C1, F2, C2);

	}
	/* Falta devolver los parametros que capture */
	/* return movimiento; */
}

int ValidarParametros (char movimiento[], TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES], int F1, int C1, int F2, int C2)
{
	int cantLeidos, distancia;

	if (strcmp(movimiento, "save") == 0)
		Save ();
	else if (strcmp(movimiento, "quit") == 0)
	{
		printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
		return 0;
	}
	else
	{
		cantLeidos = sscanf(movimiento, "[%d,%d][%d,%d]", &F1, &C1, &F2, &C2);
		
		if (cantLeidos != 4)
			ImprimirError (vecErrores, 3);
		else if ((F1 >= filas) || (F1 < 0) || (C1 >= columnas) || (C1 < 0) || (F2 >= filas) || (F2 < 0) || (C2 >= columnas) || (C2 < 0))
			ImprimirError (vecErrores, 4);
		else if (((distancia = CalcularDistancia (F1, C1, F2, C2)) != 1) && (distancia != 2))
			ImprimirError (vecErrores, 5);
		else if (Disponible (Tablero, F1, C1) != turno)
			ImprimirError (vecErrores, 6);
		else if (Disponible (Tablero, F2, C2) != 0)
			ImprimirError (vecErrores, 7);
		else
			return 0;
	}
	return 1;
}

int CalcularDistancia (int F1, int C1, int F2, int C2)
{
	int distFila, distColumna;
	distFila = abs(F2 - F1);
	distColumna = abs(C2 - C1);
	if (((distFila == 0) && (distColumna == 1)) || ((distFila == 1) && (distColumna == 0)) || ((distFila == 1) && (distColumna == 1)))
		return 1;
	else if (((distFila == 1) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 1)) || ((distFila == 2) && (distColumna == 2)) || ((distFila == 0) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 0)))
		return 2;
	return 0;
}

int Disponible (TipoTablero Tablero, int fila, int columna)
{
	if (Tablero[fila][columna] == 'A')
		return 1;
	else if (Tablero[fila][columna] == 'Z')
		return 2;
	return 0;

}

void Save ()
{
	printf("Save\n");
}
