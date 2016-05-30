#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>

#define MAX_ERRORES 50
#define MAX_FILA 30
#define MAX_COL 30

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

void CapturarJugada (char vecErrores[], int filas, int columnas)
{
	int F1, C1, F2, C2, cantLeidos;
	
	while (datosInvalidos == 1)
	{
		scanf("%s", &movimiento);
		
		if (strcmp(movimiento, "save") == 0)
			Save();
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

	}
}

void ProcesoDosJugadores (TipoTablero Tablero, int filas, int columnas, int turno)
{
	while (fin == 0)
	{
		ImprimirTablero(Tablero, filas, columnas);
		printf("Turno Jugador %d\n", turno);

		printf("Introduzca un movimiento: ");
		CapturarJugada (char vecErrores, filas, columnas);

		if (turno == 1)
			turno = 2;
		else
			turno = 1;
		getchar();
	}
}