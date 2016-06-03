#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "getnum.h"
#include <string.h>

#define SAVE 0
#define FILENAME 1

void CrearTablero (char ***Tablero, int filas, int columnas);
void Jugar (char ***Tablero, int filas, int columnas, int turno, int *mov);
int ValidarParametros (char **movimiento, char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov);
int CalcularDistancia (int *mov);
int Disponible (char ***Tablero, int fila, int columna);
void CrearBlob (char ***Tablero ,int fila, int columna, int turno);
void Adyacentes (char ***Tablero, int filas, int columnas, int turno, int fila, int columna);
void Salto (char ***Tablero, int *mov);
int Fin (char ***Tablero, int filas, int columnas, int turno);
void LLenarTablero (char ***Tablero, int filas, int columnas, int turno);
int ContarBlobs (char ***Tablero, int filas, int columnas);
int ValidarSave(char **movimiento);
void Save (char ***Tablero,int filas,int columnas, int turno, char  *movimiento);
void RecuperarJuego(char *nombre, char ***Tablero, int *filas, int *columnas, int *turno, int *opcion);

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

int ValidarParametros (char **movimiento, char ***Tablero, int filas, int columnas, int turno, char **vecErrores, int *mov)
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
	else if ((mov[0] >= filas) || (mov[0] < 0) || (mov[1] >= columnas) || (mov[1] < 0) || (mov[2] >= filas) || (mov[2] < 0) || (mov[3] >= columnas) || (mov[3] < 0))
		return 4;
	else if (CalcularDistancia (mov) == 0)
		return 5;
	else if (Disponible (Tablero, mov[0], mov[1]) != turno)
		return 6;
	else if (Disponible (Tablero, mov[2], mov[3]) != 0)
		return 7;
	
	return 0;
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

void LLenarTablero (char ***Tablero, int filas, int columnas, int turno)
{
	int i, j;

	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
		{
			if ((*Tablero)[i][j] == '0')
				(*Tablero)[i][j] = ((turno == 1) ? 'Z' : 'A');
		}
	}
}

int ContarBlobs (char ***Tablero, int filas, int columnas)
{
	int i, j, jugador1=0, jugador2;

	for (i=0; i<filas; i++)
	{
		for (j=0; j<columnas; j++)
		{
			if ((*Tablero)[i][j] == 'A')
				jugador1++;
		}
	}
	jugador2 = (filas * columnas) - jugador1;
	return ((jugador1 > jugador2) ? 1 : ((jugador1 < jugador2) ? 2 : 0));
}

void Save (char ***Tablero,int filas,int columnas, int turno, char  *movimiento, int opcion)
{
	int i, j;
	printf("Save\n");
	FILE *fPointer;
	fPointer = fopen(movimiento,"w");
	fprintf(fPointer, "%d,%d,%d,%d\n", filas, columnas, turno, opcion);
	for(i=0;i<filas;i++)
	{
		for(j=0;j<columnas;j++)
		{
			fprintf(fPointer, "%c ", (*Tablero)[i][j]);
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
	return 0;
}

void RecuperarJuego(char *nombre, char ***Tablero, int *filas, int *columnas, int *turno, int *opcion, char **vectorErrores)
{
	int i=0, j=0, caracter;
	char c;
	printf("Este es el nombre:%s\n", nombre);
	FILE *fPointer;
	fPointer = fopen(nombre, "r");
	if(fPointer==NULL)
	{
		nroError=8;
		ImprimirError (char **vecErrores, int nroError)
	}
	while((c=fgetc(fPointer))!='\n')
	{
		switch(caracter)
		{
			case 0:
			{
				*filas=(int)(c-'0');
				caracter++;
				break;
			}
			case 2:
			{
				*columnas=(int)(c-'0');
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

	*Tablero=malloc((*filas)*sizeof(char*));

	for(i=0;i<*filas;i++)
	{
		(*Tablero)[i]=malloc(*columnas);
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
			(*Tablero)[i][j] = c;
			j++;
		}
	}
	fclose(fPointer);	
}
