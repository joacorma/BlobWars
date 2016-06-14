#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "blobsBack.h"

int ** AlocarTablero(tipoMatriz *Tablero) 												/* Crea espacio en memoria para el tablero */
{
	int i, **matrix;
	
	matrix = malloc(Tablero->filas*sizeof(int*));
	
	for (i=0; i<Tablero->filas; i++)
	{
		matrix[i] = malloc(Tablero->columnas*sizeof(int));
	}

	return matrix;
}

void TableroInicial (tipoMatriz *Tablero, const char *contenidoTablero) 							/* Crea el tablero inicial */
{
	int i, j, k = 0;

	if (contenidoTablero == NULL) 								/* Si contenidoTablero es null es porque no es juego recuperado */
	{
		for (i=0; i<Tablero->filas; i++)														/* Llena la matriz con ceros */
		{
			for (j=0; j<Tablero->columnas; j++)
				Tablero->matriz[i][j] = 0;
		}
		Tablero->matriz[0][0] = 1; 											/* Pone las letras correspondientes en las esquinas */
		Tablero->matriz[0][(Tablero->columnas)-1] = 2;
		Tablero->matriz[(Tablero->filas)-1][0] = 1;
		Tablero->matriz[(Tablero->filas)-1][(Tablero->columnas)-1] = 2;
	}
	else 													/* Si entra en este caso, llena el tablero con el juego que recupero */
	{
		for (i=0; i<Tablero->filas; i++)
		{
			for (j=0; j<Tablero->columnas; j++)
			{
				Tablero->matriz[i][j] = ((contenidoTablero[k] == '0') ? 0 : ((contenidoTablero[k] == 'A') ? 1 : 2));
				k++;
			}
		}
	}
}

void EjecutarJugada(tipoMatriz *Tablero, int *mov)								/* Ejecutas las jugada y se fija de comer adyacentes */
{		
		if (CalcularDistancia(mov[0], mov[1], mov[2], mov[3]) == 1)
			CrearBlob(Tablero, mov[2], mov[3]);
		else if (CalcularDistancia(mov[0], mov[1], mov[2], mov[3]) == 2)
			Salto(Tablero, mov);

		Adyacentes(Tablero, mov[2], mov[3]);
}

int ValidarParametros(tipoMatriz *Tablero, char **movimiento, int *mov, int *error)
																									/* Valida la entrada del usuario */
{
	int cantLeidos;
	char corchete, finalString;

	if (ValidarSave (movimiento) == 0) 																/* Valida si se ingresa un save */
		return 2;
	if (strcmp(*movimiento, "quit") == 0) 															/* Valida si se ingresa un quit */
		return 3;
	
	cantLeidos = sscanf(*movimiento, "[%d,%d][%d,%d%c%c", mov, mov+1, mov+2, mov+3, &corchete, &finalString);
		
	if (cantLeidos != 5 || corchete != ']') 											/* Valida que se ingrese un formato valido */
	{
		*error = 3;
		return 0;
	}
	if (ValidarLimites(Tablero, mov) != 0)												/* Validan que el movimiento este permitido */
	{	
		*error = 4;
		return 0;
	}
	if (CalcularDistancia (mov[0], mov[1], mov[2], mov[3]) == 0)
	{	
		*error = 5;
		return 0;
	}
	if (Tablero->matriz[mov[0]][mov[1]] != Tablero->turno)
	{	
		*error = 6;
		return 0;
	}
	if (Tablero->matriz[mov[2]][mov[3]] != 0)
	{	
		*error = 7;
		return 0;
	}
	
	return 1; 																				/* Retorna 1 si el movimiento es valido */
}

int ValidarSave(char **movimiento) 												/* Valida que se haya entrado un save y un filename */
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
				else if ((*movimiento)[i] == save[i])						/* Compara el vector con otro que tiene la palbra save */
					i++;
				else
					return 1;													/* Si no es igual sale diciendo que no es un save */
			}
			break;

			case FILENAME:													/* Si es un save todo lo que le sigue es un filename */
				(*movimiento)[j++] = (*movimiento)[i++];							/* Deja en movimiento unicamente el filename */
			break;
		}
	}
	(*movimiento)[j++] = 0;
	*movimiento = realloc (*movimiento, j*sizeof(char));
	if (estado != SAVE)
		return 0;																/* Devuelve 0 si el formato del save es valido */
	return 1;
}

int ValidarLimites (tipoMatriz *Tablero, const int *mov)	/* Comprueba que el movimiento no se pase de los limites del tablero */
{
	int i; 

	for (i=0; i<4; i++)
	{
		if (mov[i] < 0)
			return 1;
		if (i%2 == 0)
		{
			if (mov[i] >= Tablero->filas)
				return 1;
		}
		else
		{
			if (mov[i] >= Tablero->columnas)
				return 1;
		}
	}
	return 0;
}

int CalcularDistancia(int u, int x, int v, int y) 	/* Calcula la distancia entre la poscicion origen y la poscicion destino */
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

void Salto (tipoMatriz *Tablero, int *mov)
{
	int aux;

	aux = Tablero->matriz[mov[0]][mov[1]];
	Tablero->matriz[mov[0]][mov[1]] = 0;
	Tablero->matriz[mov[2]][mov[3]] = aux;	
}

void Adyacentes (tipoMatriz *Tablero, int fila, int columna)			/* Come las fichas adyacentes a la poscicion destino */
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

int Fin (tipoMatriz *Tablero)						/* Comprueba que haya al menos un movimiento para el jugador actual */
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

void LLenarTablero (tipoMatriz *Tablero)									/* Llena el tablero al final del juego */
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

int ContarFichas(tipoMatriz *Tablero, int jugador)					/* Cuenta las fichas actuales de un jugador */
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

int JugadaComputadora (tipoMatriz *Tablero, int *mov) 							/* Algoritmo de la computadora */
{
	int i, j, k, l, n, m, comidas, comidasMax = 0, posOrigen = -1, posDestino = -1, coin, distancia = 2, cambio;

	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			if (Tablero->matriz[i][j] == 2)						/* comprueba que sea ficha de la computadora */
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
									comidas = 0;/* Resetea el contador cada que arranca con una nueva poscicion */
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
										/* Si encuentra una que come mas la cambia */
										cambio = 1;
									else if (comidas == comidasMax && posDestino != -1)
										/* Si comen lo mismo busca la que tenga menor distancia */
									{
										if (distancia > CalcularDistancia(i, j, k, l))
											cambio = 1;
										else if (distancia == CalcularDistancia(i, j, k, l))
										{	
											coin = (int) (rand() / ( (double) RAND_MAX + 1) * (2 - 1 + 1) + 1);
											/* Hace que sea al azar el movimento */
											if (coin == 1)
												cambio = 1;
										}
									}
									if (cambio == 1)			/* Si encontro algun mov mejor hace el cambio */
									{
										comidasMax = comidas;
										posOrigen = i*100 + j;/* Guarda la poscicion como un numero de 4 digitos */
 										posDestino = (k*100 + l);
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
	if (comidasMax != 0) 							/* Si encontro para comer las pone en el vector termina */
	{
 			mov[0] = GENERAR_FILA(posOrigen);
 			mov[1] = GENERAR_COLUMNA(posOrigen);
 			mov[2] = GENERAR_FILA(posDestino);
 			mov[3] = GENERAR_COLUMNA(posDestino);
 			return 1;
 	}

	coin = 1;												/* Si no encontro para comer busca adyantes */
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
								if (Tablero->matriz[k][l] == 0)	/* Comprueba que el destino este vacio */
								{
									if (posDestino != -1)
									/* Si es la primera que encuentra la pone en el vector */
										coin = (int) (rand() / ( (double) RAND_MAX + 1) * (2 - 1 + 1) + 1);
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
	if (posDestino == -1) 		/* Si tampoco encontro adyacentes devuelve que no tiene movimientos */
 		return 4;
 	
 	mov[0] = GENERAR_FILA(posOrigen);
 	mov[1] = GENERAR_COLUMNA(posOrigen);
 	mov[2] = GENERAR_FILA(posDestino);
 	mov[3] = GENERAR_COLUMNA(posDestino);
 
 	
 	return 1;
}
