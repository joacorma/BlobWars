#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "blobsBack.h"

#define SAVE 0
#define FILENAME 1

#define GENERAR_COORDENADA(x,y) (x*100 + y)
#define GENERAR_FILA(x) (floor(x/100))
#define GENERAR_COLUMNA(x) (x%100)

int ** CrearTablero(tipoMatriz *Tablero)
{
	int i, j, ** matrix;
	
	matrix = malloc(Tablero->filas*sizeof(int*));
	
	for (i=0; i<Tablero->filas; i++)
	{
		matrix[i] = malloc(Tablero->columnas*sizeof(int));
	}
	
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
			matrix[i][j] = 0;
	}

	matrix[0][0] = 1;
	matrix[0][(Tablero->columnas)-1] = 2;
	matrix[(Tablero->filas)-1][0] = 1;
	matrix[(Tablero->filas)-1][(Tablero->columnas)-1] = 2;

	return matrix;
}

void EjecutarJugada(tipoMatriz *Tablero, int *mov)
{		
		if (CalcularDistancia(mov[0], mov[1], mov[2], mov[3]) == 1)
		{
			CrearBlob(Tablero, mov[2], mov[3]);
		}
		else if (CalcularDistancia(mov[0], mov[1], mov[2], mov[3]) == 2)
		{
			Salto(Tablero, mov);
		}

		Adyacentes(Tablero, mov[2], mov[3]);
}

int ValidarParametros(tipoMatriz *Tablero, char **movimiento, int *mov, char **vecErrores)
{
	int cantLeidos;
	char corchete, finalString;

	if (ValidarSave (movimiento) == 0)
		return 1;
	else if (strcmp(*movimiento, "quit") == 0)
		return 2;
	
	cantLeidos = sscanf(*movimiento, "[%d,%d][%d,%d%c%c", mov, mov+1, mov+2, mov+3, &corchete, &finalString);
		
	if (cantLeidos != 5 || corchete != ']')
		return 3;
	else if ((mov[0] >= Tablero->filas) || (mov[0] < 0) || (mov[1] >= Tablero->columnas) || (mov[1] < 0) || (mov[2] >= Tablero->filas) || (mov[2] < 0) || (mov[3] >= Tablero->columnas) || (mov[3] < 0))
		return 4;
	else if (CalcularDistancia (mov[0], mov[1], mov[2], mov[3]) == 0)
		return 5;
	else if (Tablero->matriz[mov[0]][mov[1]] != Tablero->turno)
		return 6;
	else if (Tablero->matriz[mov[2]][mov[3]] != 0)
		return 7;
	
	return 0;
}

int CalcularDistancia(int u, int x, int v, int y)
{
	int distFila, distColumna;
	distFila = abs(v - u);
	distColumna = abs(y - x);
	
	if (distFila <= 2 || distColumna <= 2)
	{
		switch (distFila)
		{
			case 0:
			{
				if (distColumna != 0)
					return distColumna;
			}
			break;

			case 1:
			{
				if (distColumna != 2)
					return distFila;
				return distColumna;
			}
			break;

			case 2: return distFila;
		}
	}
	
	return 0;
}

void CrearBlob (tipoMatriz *Tablero ,int fila, int columna)
{
	Tablero->matriz[fila][columna] = Tablero->turno;
}

void Adyacentes (tipoMatriz *Tablero, int fila, int columna)
{
	int i, j;

	for (i=(fila-1); i<=(fila+1); i++)
	{
		for (j=(columna-1); j<=(columna+1); j++)
		{
			if (i>=0 && j>=0 && i<Tablero->filas && j<Tablero->columnas)
			{
				if ((Tablero->matriz[i][j] != 0) && (abs(Tablero->turno - Tablero->matriz[i][j])) == 1)
					CrearBlob(Tablero, i, j);
			}
		}
	}
}

void Salto (tipoMatriz *Tablero, int *mov)
{
	int aux;

	aux = Tablero->matriz[mov[0]][mov[1]];
	Tablero->matriz[mov[0]][mov[1]] = 0;
	Tablero->matriz[mov[2]][mov[3]] = aux;	
}

int Fin (tipoMatriz *Tablero)
{
	int i, j, k, l;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == Tablero->turno)
			{
				for (k=(i-2); k<=(i+2); k++)
				{
					if (k>=0 && k<Tablero->filas)
					{
						for (l=(j-2); l<=(j+2); l++)
						{
							if (l>=0 && l<Tablero->columnas)	
							{	
								if (Tablero->matriz[k][l] == 0)
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

void LLenarTablero (tipoMatriz *Tablero)
{
	int i, j;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 0)
				Tablero->matriz[i][j] = ((Tablero->turno == 1) ? 2 : 1);
		}
	}
}

int ContarBlobs (tipoMatriz *Tablero)
{
	int i, j, jugador1=0, jugador2;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 1)
				jugador1++;
		}
	}
	jugador2 = (Tablero->filas * Tablero->columnas) - jugador1;
	return ((jugador1 > jugador2) ? 1 : ((jugador1 < jugador2) ? 2 : 0));
}

int ContarFichas(tipoMatriz *Tablero, int jugador)
{
	int i, j, cantidad = 0;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == jugador)
				cantidad++;
		}
	}

	return cantidad;
}

void Save (tipoMatriz *Tablero, char  *movimiento, char *idJugador)
{
	int i, j, k = 0, fichasAuxJ1, fichasAuxJ2;
	char * posciciones = malloc(Tablero->filas * Tablero->columnas);
	FILE * archivo;

	archivo = fopen(movimiento, "w");
	fwrite(&(Tablero->opcion), sizeof(int), 1, archivo);
	fwrite(&(Tablero->turno), sizeof(int), 1, archivo);
	fwrite(&(Tablero->filas), sizeof(int), 1, archivo);
	fwrite(&(Tablero->columnas), sizeof(int), 1, archivo);
	fichasAuxJ1 = ContarFichas(Tablero, 1);
	fwrite(&fichasAuxJ1, sizeof(int), 1, archivo);
	fichasAuxJ2 = ContarFichas(Tablero, 2);
	fwrite(&fichasAuxJ2, sizeof(int), 1, archivo);

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			posciciones[k] = idJugador[Tablero->matriz[i][j]];
			k++;
		}
	}
	fwrite(posciciones, Tablero->filas * Tablero->columnas, 1, archivo);
	
	fclose(archivo);

	printf("Partida guardada. Pulse enter para continuar...\n");
	getchar();
}

int ValidarSave(char **movimiento)
{
	char * save = "save ";
	int estado = SAVE, i = 0, j = 0;
	
	while ((*movimiento)[i] != 0)
	{
		switch (estado)
		{
			case SAVE:
			{
				if (save[i] == 0)
					estado = FILENAME;
				else if ((*movimiento)[i] == save[i])
					i++;
				else
					return 1;
			}
			break;

			case FILENAME:
				(*movimiento)[j++] = (*movimiento)[i++];
			break;
		}
	}
	(*movimiento)[j++] = 0;
	*movimiento = realloc (*movimiento, j*sizeof(char));
	if (estado != SAVE)
		return 0;
	return 1;
}

int JugadaComputadora (tipoMatriz *Tablero, int *mov)
{
	int i, j, k, l, n, m, comidas, comidasMax = 0, posOrigen = -1, posDestino = -1, coin, distancia = 2, cambio;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 2)
			{
				for (k=(i-2); k<=(i+2); k++)
				{
					if (k>=0 && k<Tablero->filas)
					{
						for (l=(j-2); l<=(j+2); l++)
						{
							if (l>=0 && l<Tablero->columnas)	
							{	
								if (Tablero->matriz[k][l] == 0)
								{
									comidas = 0;
									for (n=(k-1); n<=(k+1); n++)
									{
										if (n>=0 && n<Tablero->filas)
										{
											for (m=(l-1); m<=(l+1); m++)
											{
												if (m>=0 && m<Tablero->columnas)
												{
													if (Tablero->matriz[n][m] == 1)
														comidas++;
												}
											}
										}
									}
									cambio = 0;
									if (comidas > comidasMax)
										cambio = 1;
									else if (comidas == comidasMax && posDestino != -1)
									{
										if (distancia > CalcularDistancia(i, j, k, l))
											cambio = 1;
										else if (distancia == CalcularDistancia(i, j, k, l))
										{	
											coin = rand()%2 + 1;
											if (coin == 1)
												cambio = 1;
										}
									}
									if (cambio == 1)
									{
										comidasMax = comidas;
										posOrigen = i*100 + j;
										posDestino = k*100 + l;
										distancia = CalcularDistancia(i, j, k, l);

									}
								}
							}
						}
					}
				}
			}
		}
	}

	printf("%d\n%d\n%d\n", posOrigen, posDestino, comidasMax);
	getchar();
	if (comidasMax != 0)
	{
		mov[0] = GENERAR_FILA(posOrigen);
		mov[1] = GENERAR_COLUMNA(posOrigen);
		mov[2] = GENERAR_FILA(posDestino);
		mov[3] = GENERAR_COLUMNA(posDestino);
		return 0;
	}

	coin = 1;
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 2)
			{
				for (k=(i-1); k<=(i+1); k++)
				{
					if (k>=0 && k<Tablero->filas)
					{
						for (l=(j-1); l<=(j+1); l++)
						{
							if (l>=0 && l<Tablero->columnas)
							{
								if (Tablero->matriz[k][l] == 0)
								{
									if (posDestino != -1)
										coin = rand()%2 + 1;
									if (coin == 1)
									{
										posOrigen = i*100 + j;
										posDestino = k*100 + l;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	if (posDestino == -1)
		return 3;

	printf("%d.\n%d.\n%d.\n", posOrigen, posDestino, comidasMax);
	getchar();
	
	mov[0] = GENERAR_FILA(posOrigen);
	mov[1] = GENERAR_COLUMNA(posOrigen);
	mov[2] = GENERAR_FILA(posDestino);
	mov[3] = GENERAR_COLUMNA(posDestino);

	
	return 0;
}

int RecuperarJuego(tipoMatriz *Tablero, char **vecErrores)
{
	int i=0, j=0, k=0;
	char *nombre = NULL;
	FILE * archivo;
	
	printf("Introduzca el nombre del archivo: ");
	nombre = leerCaracteres();
	archivo = fopen(nombre, "r");
	while (archivo == NULL)
	{
		ImprimirError(vecErrores, 8);
		printf("Introduzca el nombre del archivo: ");
		nombre = leerCaracteres();
		archivo = fopen(nombre, "r");
	}
	
	fread(&(Tablero->opcion), sizeof(int), 1, archivo);
	fread(&(Tablero->turno), sizeof(int), 1, archivo);
	fread(&(Tablero->filas), sizeof(int), 1, archivo);
	fread(&(Tablero->columnas), sizeof(int), 1, archivo);
	int fichasJ1Aux;
	fread(&fichasJ1Aux, sizeof(int), 1, archivo);
	int fichasJ2Aux;
	fread(&fichasJ2Aux, sizeof(int), 1, archivo);

	char *posciciones = malloc (Tablero->filas * Tablero->columnas);
	fread(posciciones, Tablero->filas * Tablero->columnas, 1, archivo);

	Tablero->matriz = malloc (Tablero->filas*sizeof(int*));
	
	for (i=0; i<Tablero->filas; i++)
	{
		Tablero->matriz[i] = malloc (Tablero->columnas*sizeof(int));
	}
	
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			Tablero->matriz[i][j] = ((posciciones[k] == '0') ? 0 : ((posciciones[k] == 'A') ? 1 : 2));
			k++;
		}
	}

	fclose(archivo);

	return 0;
}
