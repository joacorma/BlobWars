#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>

void CrearTablero (char ***Tablero, int filas, int columnas);
void Jugar (char ***Tablero, int filas, int columnas, int turno, int *mov);
int ValidarParametros (char movimiento[], char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov);
int CalcularDistancia (int *mov);
int Disponible (char ***Tablero, int fila, int columna);
void CrearBlob (char ***Tablero ,int fila, int columna, int turno);
void Adyacentes (char ***Tablero, int filas, int columnas, int turno, int fila, int columna);
void Salto (char ***Tablero, int *mov);
int Fin (char ***Tablero, int filas, int columnas, int turno);
void Save (char ***Tablero,int filas,int columnas,int turno,char *opcion);

void CrearTablero (char ***Tablero, int filas, int columnas)
{
	int i, j;
	*Tablero=malloc(filas*sizeof(char*));
	
	for(i=0;i<filas;i++)
	{
		(*Tablero)[i]=malloc(columnas);
	}
	
	for(i=0;i<filas;i++)
	{
		for (j=0; j<columnas; j++)
			(*Tablero)[i][j] = '0';
	}

	(*Tablero)[0][0] = 'A';
	(*Tablero)[0][columnas-1] = 'Z';
	(*Tablero)[filas-1][0] = 'A';
	(*Tablero)[filas-1][columnas-1] = 'Z';

}

void Jugar (char ***Tablero, int filas, int columnas, int turno, int *mov)
{		
		if (CalcularDistancia(mov) == 1)
			CrearBlob(Tablero, mov[2], mov[3], turno);
		else if (CalcularDistancia(mov) == 2)
			Salto(Tablero, mov);

		Adyacentes(Tablero, filas, columnas, turno, mov[2], mov[3]);
}

int ValidarParametros (char movimiento[], char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov)
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
		cantLeidos = sscanf(movimiento, "[%d,%d][%d,%d]", mov, mov+1, mov+2, mov+3);
		
		if (cantLeidos != 4)
			return 3;
		else if ((mov[0] >= filas) || (mov[0] < 0) || (mov[1] >= columnas) || (mov[1] < 0) || (mov[2] >= filas) || (mov[2] < 0) || (mov[3] >= columnas) || (mov[3] < 0))
			return 4;
		else if (((distancia = CalcularDistancia (mov)) != 1) && (distancia != 2))
			return 5;
		else if (Disponible (Tablero, mov[0], mov[1]) != turno)
			return 6;
		else if (Disponible (Tablero, mov[2], mov[3]) != 0)
			return 7;
		else
			return 0;
	}
}

int CalcularDistancia (int *mov)
{
	int distFila, distColumna;
	distFila = abs(mov[2] - mov[0]);
	distColumna = abs(mov[3] - mov[1]);
	if (((distFila == 0) && (distColumna == 1)) || ((distFila == 1) && (distColumna == 0)) || ((distFila == 1) && (distColumna == 1)))
		return 1;
	else if (((distFila == 1) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 1)) || ((distFila == 2) && (distColumna == 2)) || ((distFila == 0) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 0)))
		return 2;
	return 0;
}

int Disponible (char ***Tablero, int fila, int columna)
{
	if ((*Tablero)[fila][columna] == 'A')
		return 1;
	else if ((*Tablero)[fila][columna] == 'Z')
		return 2;
	return 0;

}

void CrearBlob (char ***Tablero ,int fila, int columna, int turno)
{
	(*Tablero)[fila][columna] = ((turno == 1) ? 'A' : 'Z');
}

void Adyacentes (char ***Tablero, int filas, int columnas, int turno, int fila, int columna)
{
	int i, j, blob;

	for (i=(fila-1); i<=(fila+1); i++)
	{
		for (j=(columna-1); j<=(columna+1); j++)
		{
			if (i>=0 && j>=0 && i<filas && j<columnas)
			{
				blob = Disponible(Tablero, i, j);
				if ((blob != 0) && (abs(turno-blob) == 1))
					CrearBlob(Tablero, i, j, turno);
			}
		}
	}
}

void Salto (char ***Tablero, int *mov)
{
	char aux;

	aux = (*Tablero)[mov[0]][mov[1]];
	(*Tablero)[mov[0]][mov[1]] = '0';
	(*Tablero)[mov[2]][mov[3]] = aux;
}

int Fin (char ***Tablero, int filas, int columnas, int turno)
{
	int i, j, k, l;

	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
		{
			if ((*Tablero)[i][j] == ((turno == 1) ? 'A' : 'Z'))
			{
				for (k=(i-2); k<=(i+2); k++)
				{
					if (k>=0 && k<filas)
					{
						for (l=(j-2); l<=(j+2); l++)
						{
							if (l>=0 && l<columnas)	
							{	
								if (Disponible(Tablero, k, l) == 0)
									return 0;
							}
						}
					}
				}
			}
		}
	}
	return 1;

}

void Save (char ***Tablero,int filas,int columnas, int turno, char *opcion)
{
	int i, j;
	printf("Save\n");
	FILE = *fPointer;
	fPointer = fopen("PartidaSalvada.txt","w");
	for(i=0;i<filas;i++)
	{
		for(j=0;j<columnas;j++)
		{
			fprintf(fPointer, "%c ", (*Tablero)[i][j]);
		}
		fprintf(fPointer, "\n");
	}
	fprintf(fPointer, "%d,%d,%d", filas, columnas, turno);
	fclose(fPointer);
	*opcion='4';
}
