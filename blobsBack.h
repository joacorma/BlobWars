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

void ImprimirTablero (TipoTablero Tablero, int filas, int columnas)
{
	int i, j;
	system("clear");
	
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
			Tablero[i][j] = 'v';
	}

	Tablero[0][0] = 'A';
	Tablero[0][columnas-1] = 'Z';
	Tablero[filas-1][0] = 'A';
	Tablero[filas-1][columnas-1] = 'Z';
}

void ProcesoDosJugadores (TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES])
{
	int fin = 0;
	/*char movimiento[MAX_MOVIMIENTOS];*/ 

	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", turno);

		printf("Introduzca un movimiento: ");
		/*movimiento[] = */CapturarJugada (Tablero, filas, columnas, turno, vecErrores);

		if (turno == 1)
			turno = 2;
		else
			turno = 1;
		getchar();
	}
}

void CapturarJugada (TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES])
/* Si quiero retornar un vector de chars: char * CapturarJugada. */
{
	int datosInvalidos = 1;
	char movimiento[MAX_MOVIMIENTOS];
	
	while (datosInvalidos == 1)
	{
		scanf("%s", &movimiento);
		BORRAR_BUFFER;
		datosInvalidos = ValidarParametros (movimiento, Tablero, filas, columnas, turno, vecErrores);

	}
	/* Falta devolver los parametros que capture */
	/* return movimiento; */
}

int ValidarParametros (char movimiento[], TipoTablero Tablero, int filas, int columnas, int turno, char *vecErrores[MAX_ERRORES])
{
	int cantLeidos, F1, C1, F2, C2, diatncia;

	if (strcmp(movimiento, "save") == 0)
		Save ();
	else if (strcmp(movimiento, "quit") == 0)
	{
		printf("Gracias por jugar al Blob Wars. Hasta pronto!\n");
		return 0;
	}
	else
	{
		cantLeidos = sscanf(movimiento, "[%d,%d][%d,%d]", F1, C1, F2, C2);
		
		if (cantLeidos != 4)
			ImprimirError (vecErrores, 3);
		else if ((F1 >= filas) || (F1 < 0) || (C1 >= columnas) || (C1 < 0) || (F2 >= filas) || (F2 < 0) || (C2 >= columnas) || (C2 < 0))
			ImprimirError (vecErrores, 4);
		else if ((diatncia = CalcularDistancia (F1, C1, F2, C2)) != 1) && distancia != 2))
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
	distFila = F2 - F1;
	distColumna = C2 - C1;
	if ((distFila == 1) && (distColumna == 1))
		return 1;
	else if ((distFila == 2) && (distColumna == 2))
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