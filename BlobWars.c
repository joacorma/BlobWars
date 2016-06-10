#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "blobsBack.h"


#define MAX_ERRORES 9
#define MIN_FILAS 5
#define MAX_FILAS 30
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 30
#define BLOQUE 5
#define SAVE 0
#define FILENAME 1

#define BORRAR_BUFFER while (getchar()!='\n')
#define GENERAR_COORDENADA(x,y) (x*100 + y)
#define GENERAR_FILA(x) (floor(x/100))
#define GENERAR_COLUMNA(x) (x%100)

void ProcesoJuego (tipoMatriz *Tablero, char *idJugador, char **vecErrores);
void CargarErrores (char **vecErrores);
void ImprimirError (char **vecErrores, int nroError);
void PantallaInicial ();
int get_filas ();
int get_columnas ();
void ImprimirTablero (tipoMatriz *Tablero, char *idJugador);
int CapturarJugada (tipoMatriz *Tablero, char **movimiento, int *coordenadas, char **vecErrores);
char * leerCaracteres ();
int get_int ();

int main ()
{
	srand(time(NULL));
	char *vecErrores[MAX_ERRORES], idJugador[3] = {'0', 'A', 'Z'};
	tipoMatriz Tablero;
	Tablero.filas = 0;
	Tablero.columnas = 0;
	Tablero.opcion = 0;
	Tablero.turno = 0;

	CargarErrores(vecErrores);
	
	while (Tablero.opcion != 4)
	{
		PantallaInicial ();
		printf("Elegir opcion: ");
		Tablero.opcion = get_int();
		while (Tablero.opcion < 1 || Tablero.opcion > 4)
		{
			ImprimirError(vecErrores, 0);
			printf("Elegir opcion: ");
			Tablero.opcion = get_int();
		}

		switch(Tablero.opcion)
		{
			case 3: 
			{	
				while (Tablero.opcion == 3)
					RecuperarJuego(&Tablero, vecErrores);
			}
			break;
			case 4: 
			{
				printf("Gracias por jugar al Blob Wars. Hasta pronto!\n\n");
				return 0;
			}
		}

		ProcesoJuego (&Tablero, idJugador, vecErrores);
	}
	return 0;
}

void ProcesoJuego (tipoMatriz *Tablero, char *idJugador, char **vecErrores)
{	
	char *movimiento = NULL;
	int fin = 0, accion, ganador, coordenadas[4];
	
	if(Tablero->filas == 0)
	{
		Tablero->filas = get_filas();
		while ((Tablero->filas < MIN_FILAS) || (Tablero->filas > MAX_FILAS))
		{
			ImprimirError(vecErrores, 1);
			Tablero->filas = get_filas();
		}

		Tablero->columnas = get_columnas();
		while ((Tablero->columnas < MIN_COLUMNAS) || (Tablero->columnas > MAX_COLUMNAS))
		{
			ImprimirError(vecErrores, 2);
			Tablero->columnas = get_columnas();
		}

		printf("\n");
		printf("Jugador 1: %c\n", idJugador[1]);
		printf("Jugador 2: %c\n", idJugador[2]);
		printf("\n");
		printf("Para introducir un movimiento debe hacerlo de esta forma: [F1,C1][F2,C2]\n");
		printf("Por ejemplo: [0,0][0,1]\n");
		printf("Pulse enter para comenzar el juego...");
		getchar();

		Tablero->matriz = CrearTablero (Tablero);
		Tablero->turno = (int) (rand() / ( (double) RAND_MAX + 1) * (2 - 1 + 1) + 1);
	}

	while (fin == 0)
	{
		ImprimirTablero(Tablero, idJugador);
		printf("\nTurno Jugador %d\n", Tablero->turno);

		if (Tablero->opcion == 2 && Tablero->turno == 2)
			accion = JugadaComputadora (Tablero, coordenadas);
		else
			accion = CapturarJugada (Tablero, &movimiento, coordenadas, vecErrores);
	
		switch (accion)
		{
			case 0: 
			{
				EjecutarJugada (Tablero, coordenadas);

				if (Tablero->turno == 1)
					Tablero->turno = 2;
				else
					Tablero->turno = 1;

				fin = Fin(Tablero);
			}
			break;
			
			case 1: Save (Tablero, movimiento, idJugador);
			break;
		
			case 2: 
			{
				fin = 1;
				free(Tablero->matriz);
				Tablero->filas = 0;
				Tablero->columnas = 0;
				printf("La partida ha sido terminada. Pulse enter para continuar...\n");
				getchar();
			}
			break;

			case 3:
			{
				fin = 1;
				accion = 0;
			}
		}
	}

	if (accion == 0)
	{	
		ImprimirTablero(Tablero, idJugador);
		LLenarTablero (Tablero);
		printf("\n");
		ImprimirTablero(Tablero, idJugador);
	
		ganador = ContarBlobs (Tablero);
		if (ganador == 0)
			printf("Hubo un empate\n");
		else
			printf("El ganador es el Jugador %d\n", ganador);
	
		printf("Pulse enter para volver al menu principal...\n");
		getchar();
	}

}


void CargarErrores (char **vecErrores)
{
	int i;
	for(i=0;i<MAX_ERRORES;i++)
		vecErrores[i]=malloc(70);
	vecErrores[0] = "La opcion debe ser entre 1 y 4";
	vecErrores[1] = "La cantidad de filas debe ser entre 5 y 30";
	vecErrores[2] = "La cantidad de columnas debe ser entre 5 y 30";
	vecErrores[3] = "El formato introducido es incorrecto";
	vecErrores[4] = "No existe esa poscicion en el tablero";
	vecErrores[5] = "Ese movimiento no esta dentro del rango de movimientos posibles";
	vecErrores[6] = "Ese casillero no corresponde a un Blob de tu color";
	vecErrores[7] = "Ese casillero ya esta ocupado";
	vecErrores[8] = "No existe ese archivo";
}

void ImprimirError (char **vecErrores, int nroError)
{
	printf("ERROR: %s\n\n", vecErrores[nroError]);

}

void PantallaInicial ()
{
	system("clear");

	printf("BLOB WARS\n\n\n");
	printf("1. Juego de dos jugadores\n");
	printf("2. Juego contra la computadora\n");
	printf("3. Recuperar juego grabado\n");
	printf("4. Terminar\n\n");
}

int get_filas ()
{
	int fila;
	printf("Ingrese la cantidad de filas del tablero (entre 5 y 30): ");
	fila = get_int();
	return fila;

}

int get_columnas ()
{
	int col;
	printf("Ingrese la cantidad de columnas del tablero (entre 5 y 30): ");
	col = get_int();
	return col;
}


void ImprimirTablero (tipoMatriz *Tablero, char *idJugador)
{
	int i, j;
	
	system("clear");
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
			printf("%c ", idJugador[Tablero->matriz[i][j]]);
		printf("\n");
	}
}

int CapturarJugada (tipoMatriz *Tablero, char **movimiento, int *coordenadas, char **vecErrores)
{
	int datosInvalidos = 3;
	
	while (datosInvalidos > 2)
	{
		printf("Introduzca un movimiento: ");
		*movimiento = leerCaracteres();
		datosInvalidos = ValidarParametros (Tablero, movimiento, coordenadas, vecErrores);
		if (datosInvalidos > 2)
			ImprimirError(vecErrores, datosInvalidos);
	}
	
	return datosInvalidos;
}

char * leerCaracteres()
{
	char *aux = NULL, *string = malloc(sizeof(char));
	int i = 0, a;
	while ((a = getchar()) != '\n')
	{
		if (i % BLOQUE == 0)
		{
			aux = realloc(string, (i + BLOQUE)*sizeof(char));
			if (aux != NULL)
				string = aux;
		}
		string[i++] = a;
	}
	string[i++] = 0;
	string = realloc(string, i*sizeof(char));
	return string;
}

int get_int()
{
	int a, entero = 0;

	while ((a = getchar()) != '\n')
	{
		if (isdigit(a))
			entero = 10*entero + (a - '0');
		else
		{	
			BORRAR_BUFFER;
			return -1;
		}
	}

	return entero;
}
