#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>
#include <math.h>

#define SAVE 0
#define FILENAME 1

#define GENERAR_COORDENADA(x,y) (x*100 + y)
#define GENERAR_FILA(x) (floor(x/100))
#define GENERAR_COLUMNA(x) (x%100)

typedef struct
	{	
		int filas;
		int columnas;
		int **matriz;
	} tipoMatriz;


int ** CrearTablero (tipoMatriz *Tablero);
void EjecutarJugada (tipoMatriz *Tablero, int *turno, int *mov);
int ValidarParametros (char **movimiento, tipoMatriz *Tablero, int *turno, char **vecErrores, int *mov);
int CalcularDistancia (int u, int v, int x, int y);
void CrearBlob (tipoMatriz *Tablero ,int fila, int columna, int *turno);
void Adyacentes (tipoMatriz *Tablero, int *turno, int fila, int columna);
void Salto (tipoMatriz *Tablero, int *mov);
int Fin (tipoMatriz *Tablero, int *turno);
void LLenarTablero (tipoMatriz *Tablero, int *turno);
int ContarBlobs (tipoMatriz *Tablero);
int ValidarSave(char **movimiento);
void Save (tipoMatriz *Tablero, int *turno, char  *movimiento, int *opcion);
int RecuperarJuego(tipoMatriz *Tablero, int *turno, int *opcion, char **vecErrores);
int JugadaComputadora (tipoMatriz *Tablero, int *mov);
char * leerCaracteres ();/*hay que sacarlo de aca*/

int ** CrearTablero (tipoMatriz *Tablero)
{
	int i, j;
	int**matrix;
	matrix = malloc((*Tablero).filas*sizeof(int*));
	
	for (i=0; i<(*Tablero).filas; i++)
	{
		matrix[i]=malloc((*Tablero).columnas*sizeof(int));
	}
	
	for (i=0; i<(*Tablero).filas; i++)
	{
		for (j=0; j<(*Tablero).columnas; j++)
			matrix[i][j] = 0;
	}

	matrix[0][0] = 1;
	matrix[0][((*Tablero).columnas)-1] = 2;
	matrix[((*Tablero).filas)-1][0] = 1;
	matrix[((*Tablero).filas)-1][((*Tablero).columnas)-1] = 2;

	return matrix;
}

void EjecutarJugada (tipoMatriz *Tablero, int *turno, int *mov)
{		
		if (CalcularDistancia(mov[0], mov[2], mov[1], mov[3]) == 1)
		{
			CrearBlob(Tablero, mov[2], mov[3], turno);
		}
		else if (CalcularDistancia(mov[0], mov[2], mov[1], mov[3]) == 2)
		{
			Salto(Tablero, mov);
		}

		Adyacentes(Tablero, turno, mov[2], mov[3]);
}

int ValidarParametros (char **movimiento, tipoMatriz *Tablero, int *turno, char **vecErrores, int *mov)
{
	int cantLeidos, distancia;
	char corchete;

	if (ValidarSave (movimiento) == 0)
		return 1;
	else if (strcmp(*movimiento, "quit") == 0)
		return 2;
	
	cantLeidos = sscanf(*movimiento, "[%d,%d][%d,%d%c", mov, mov+1, mov+2, mov+3, &corchete);
		
	if (cantLeidos != 5 && corchete != ']')
		return 3;
	else if ((mov[0] >= Tablero->filas) || (mov[0] < 0) || (mov[1] >= Tablero->columnas) || (mov[1] < 0) || (mov[2] >= Tablero->filas) || (mov[2] < 0) || (mov[3] >= Tablero->columnas) || (mov[3] < 0))
		return 4;
	else if (CalcularDistancia (mov[0], mov[2], mov[1], mov[3]) == 0)
		return 5;
	else if (Tablero->matriz[mov[0]][mov[1]] != *turno)
		return 6;
	else if (Tablero->matriz[mov[2]][mov[3]] != 0)
		return 7;
	
	return 0;
}

int CalcularDistancia (int u, int v, int x, int y)
{
	int distFila, distColumna;
	distFila = abs(v - u);
	distColumna = abs(y - x);
	if (((distFila == 0) && (distColumna == 1)) || ((distFila == 1) && (distColumna == 0)) || ((distFila == 1) && (distColumna == 1)))
		return 1;
	else if (((distFila == 1) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 1)) || ((distFila == 2) && (distColumna == 2)) || ((distFila == 0) && (distColumna == 2)) || ((distFila == 2) && (distColumna == 0)))
		return 2;
	return 0;
}

void CrearBlob (tipoMatriz *Tablero ,int fila, int columna, int *turno)
{
	Tablero->matriz[fila][columna] = *turno;
}

void Adyacentes (tipoMatriz *Tablero, int *turno, int fila, int columna)
{
	int i, j;

	for (i=(fila-1); i<=(fila+1); i++)
	{
		for (j=(columna-1); j<=(columna+1); j++)
		{
			if (i>=0 && j>=0 && i<Tablero->filas && j<Tablero->columnas)
			{
				if ((Tablero->matriz[i][j] != 0) && (abs(*turno - Tablero->matriz[i][j])) == 1)
					CrearBlob(Tablero, i, j, turno);
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

int Fin (tipoMatriz *Tablero, int *turno)
{
	int i, j, k, l;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == *turno)
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

void LLenarTablero (tipoMatriz *Tablero, int *turno)
{
	int i, j;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 0)
				Tablero->matriz[i][j] = ((*turno == 1) ? 2 : 1);
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

void Save (tipoMatriz *Tablero, int *turno, char  *movimiento, int *opcion)
{
	int i, j;
	printf("Save\n");
	FILE *fPointer;
	fPointer = fopen(movimiento,"w");
	fprintf(fPointer, "%d,%d,%d,%d\n", (*Tablero).filas, (*Tablero).columnas, *turno, *opcion);
	for(i=0;i<(*Tablero).filas;i++)
	{
		for(j=0;j<(*Tablero).columnas;j++)
		{
			fprintf(fPointer, "%c ", (*Tablero).matriz[i][j]);
		}
		fprintf(fPointer, "\n");
	}
	fclose(fPointer);
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
	if(estado!=SAVE)
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
										if (distancia > CalcularDistancia(i, k, j, l))
											cambio = 1;
										else if (distancia == CalcularDistancia(i, k, j, l))
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
										distancia = CalcularDistancia(i, k, j, l);

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

int RecuperarJuego(tipoMatriz *Tablero, int *turno, int *opcion, char **vecErrores)
{
	int i=0, j=0, caracter;
	char c, *nombre=NULL;
	printf("Introduzca el nombre del archivo: ");
	nombre = leerCaracteres();
	printf("Este es el nombre:%s\n", nombre);
	FILE *fPointer;
	fPointer = fopen(nombre, "r");
	if(fPointer==NULL)
	{
		return 8;
	}
	while((c=fgetc(fPointer))!='\n')
	{
		switch(caracter)
		{
			case 0:
			{
				(*Tablero).filas=(int)(c-'0');
				caracter++;
				break;
			}
			case 2:
			{
				(*Tablero).columnas=(int)(c-'0');
				caracter++;
				break;
			}
			case 4:
			{
				*turno=(int)(c-'0');
				caracter++;
				break;
			}
			case 6:
			{
				*opcion=(int)(c-'0');
				caracter++;
				break;
			}
			default:
			{
				caracter++;
				break;
			}
		}
	}

	printf("PRUEBA\n");
	
	(*Tablero).matriz=malloc(((*Tablero).filas)*sizeof(char*));

	for(i=0;i<(*Tablero).filas;i++)
	{
		(*Tablero).matriz[i]=malloc((*Tablero).columnas);
	}
	i=0;
	j=0;
	while((c=fgetc(fPointer))!=EOF)
	{
		if(c=='\n')
		{
			i++;
			j=0;
		}	
		else if(c!=' ')
		{
			(*Tablero).matriz[i][j] = c;
			j++;
		}
	}
	fclose(fPointer);
	return 0;
}
