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

void CapturarJugada (TipoTablero Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov);
int ValidarParametros (char movimiento[], TipoTablero Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov);
int CalcularDistancia (int *mov);
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

void ProcesoDosJugadores (TipoTablero Tablero, int filas, int columnas, int turno, char **vecErrores)
{
	int fin = 0, mov[4];
	/*char movimiento[MAX_MOVIMIENTOS];*/ 

	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", turno);

		/*movimiento[] = */CapturarJugada (Tablero, filas, columnas, turno, vecErrores,*mov);
		printf("%d %d %d %d\n", mov[0], mov[1], mov[2], mov[3]);

		if (turno == 1)
			turno = 2;
		else
			turno = 1;
	}
}

void CapturarJugada (TipoTablero Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov)
/* Si quiero retornar un vector de chars: char * CapturarJugada. */
{
	int datosInvalidos = 1;
	char movimiento[MAX_MOVIMIENTOS];
	
	while (datosInvalidos == 1)
	{
		printf("Introduzca un movimiento: ");
		scanf("%s", movimiento);
		BORRAR_BUFFER;
		datosInvalidos = ValidarParametros (movimiento, Tablero, filas, columnas, turno, vecErrores, mov);

	}
	/* Falta devolver los parametros que capture */
	/* return movimiento; */
}

int ValidarParametros (char movimiento[], TipoTablero Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov)
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
		cantLeidos = sscanf(movimiento, "[%d,%d][%d,%d]", mov, (mov+1), (mov+2), (mov+3));
		
		if (cantLeidos != 4)
			ImprimirError (vecErrores, 3);
		else if ((mov[0] >= filas) || (mov[0] < 0) || (mov[1] >= columnas) || (mov[1] < 0) || (mov[2] >= filas) || (mov[2] < 0) || (mov[3] >= columnas) || (mov[3] < 0))
			ImprimirError (vecErrores, 4);
		else if (((distancia = CalcularDistancia (mov[0], mov[1], mov[2], mov[3])) != 1) && (distancia != 2))
			ImprimirError (vecErrores, 5);
		else if (Disponible (Tablero, mov[0], mov[1]) != turno)
			ImprimirError (vecErrores, 6);
		else if (Disponible (Tablero, mov[2], mov[3]) != 0)
			ImprimirError (vecErrores, 7);
		else
			return 0;
	}
	return 1;
}

int CalcularDistancia (int *mov)
{
	int distFila, distColumna;
	distFila = abs(mov[1] - mov[0]);
	distColumna = abs(mov[3] - mov[2]);
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
