#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "blobsBack.h"

void ImprimirError (char **vecErrores, int nroError);
void ProcesoJuego (tipoMatriz *Tablero, const char *idJugador, char **vecErrores);
void CargarErrores (char **vecErrores);
void PantallaInicial ();
void CondicionesInicilaes (tipoMatriz *Tablero, char **vecErrores);
void CrearTablero (tipoMatriz *Tablero, const char *contenidoTablero, char **vecErrores);
void ImprimirTablero (tipoMatriz *Tablero, const char *idJugador);
void GuardarJuego (tipoMatriz *Tablero, const char *movimiento, const char *idJugador);
char ValidarSiNO (char **vecErrores);
char * leerCaracteres ();
int get_int ();
int get_filas ();
int get_columnas ();
int CapturarJugada (tipoMatriz *Tablero, char **movimiento, int *coordenadas, char **vecErrores);
int RecuperarJuego(tipoMatriz *Tablero, char **contenidoTablero, char **vecErrores);

int main ()
{
	srand(time(NULL)); 
	char *vecErrores[MAX_ERRORES], idJugador[3] = {'0', 'A', 'Z'}, *contenidoTablero;
	tipoMatriz Tablero;
	int noLoad;
	Tablero.opcion = 0;												/* Se inicializa en 0 para que entre la primera vez al while */

	CargarErrores(vecErrores);
	
	while (Tablero.opcion != 4)
	{
		contenidoTablero = NULL;
		noLoad=1;
		PantallaInicial ();
		printf("Elegir opcion: ");
		Tablero.opcion = get_int();
		while (Tablero.opcion < 1 || Tablero.opcion > 4) 								/* Controla que la opcion este entre 1 y 4 */
		{
			ImprimirError(vecErrores, 0);
			printf("Elegir opcion: ");
			Tablero.opcion = get_int();
		}
		switch(Tablero.opcion)
		{
			case 3: 																	/* Si la opcion es 3 va a recuperar juego */
			{	
				while (Tablero.opcion == 3)
					noLoad=RecuperarJuego(&Tablero, &contenidoTablero, vecErrores);/* contenidoTablero guarda los el tablero recuperado */
			}
			break;
		
			case 4: 																	/* Si la opcion es 4 termina el programa */
			{
				printf("Gracias por jugar al Blob Wars. Hasta pronto!\n\n");
				return 0;
			}
		}
		if(noLoad!=0)
		{																				/* Si la opcion es 1 0 2 no ejecuto el juego */
			CrearTablero(&Tablero, contenidoTablero, vecErrores);
			ProcesoJuego(&Tablero, idJugador, vecErrores);
		}
	}
	return 0;
}

void ProcesoJuego (tipoMatriz *Tablero, const char *idJugador, char **vecErrores) 
{	
	char *movimiento = NULL, *filename;
	int fin = 0, accion, ganador, coordenadas[4], fichasJugador1, fichasJugador2;		

	printf("\n");
	printf("Jugador 1: %c\n", idJugador[1]);
	printf("Jugador 2: %c\n", idJugador[2]);
	printf("\n");
	printf("Para introducir un movimiento debe hacerlo de esta forma: [F1,C1][F2,C2]\n");
	printf("Por ejemplo: [0,0][0,1]\n");

	while (fin == 0) 						/* Se juega hasta que fin sea distinto 0. Fin es cero cuando uno se queda sin movimientos */
	{
		ImprimirTablero(Tablero, idJugador);
		printf("\nTurno Jugador %d\n", Tablero->turno);

		if (Tablero->opcion == 2 && Tablero->turno == 2)
			accion = JugadaComputadora (Tablero, coordenadas);
		else
			accion = CapturarJugada (Tablero, &movimiento, coordenadas, vecErrores);
	
		switch (accion)
		{
			case 1: 									/* Accion es 1 cuando se pasa un movimento valido del usuario o la maquina */
			{
				EjecutarJugada (Tablero, coordenadas);

				if (Tablero->turno == 1) 																		/* Cambia el turno */
					Tablero->turno = 2;
				else
					Tablero->turno = 1;

				fin = Fin(Tablero);
			}
			break;
			
			case 2: GuardarJuego(Tablero, movimiento, idJugador); 									/* Accion es 2 si introduce save */
			break;
		
			case 3: 																			/* Accion es 3 si se introduce quit */
			{
				fin = 1;
				printf("\nHa salido de la partida\n");
				int rta=ValidarSiNO(vecErrores);

				if (rta == 's')
				{
					printf("\nIntroduzca el nombre del archivo: ");
					filename = leerCaracteres();
					while (filename[0] == 0)
					{
						ImprimirError(vecErrores, 10);
						printf("\nIntroduzca el nombre del archivo: ");
						filename = leerCaracteres();
					}
					GuardarJuego(Tablero, filename, idJugador);
				}
			}
			break;

			case 4:																/* Accion es 4 si la maquina se queda sin movimientos */
			{
				fin = 1;
				accion = 1;													/* Se pone accion en 1 para que imprima el tablero final */
			}
		}
	}

	if (accion == 1) 																	/* Si accion es 1 es porque termino el juego */
	{	
		ImprimirTablero(Tablero, idJugador);								/* Se imprime el tablero con el ultimo movimiento valido */
		printf("\nEl Jugador %d no tiene mas mvimentos disponibles\n", Tablero->turno);

		LLenarTablero (Tablero);
		ImprimirTablero(Tablero, idJugador);		/* Se imprime el tablero llenado con las fichas del que todavia tenia movimientos */
		
		fichasJugador1 = ContarFichas(Tablero, 1);															/* Determina el ganador */
		fichasJugador2 = (Tablero->filas * Tablero->columnas) - fichasJugador1; 
		ganador = fichasJugador1 - fichasJugador2;
		if (ganador == 0)
			printf("\nHubo un empate\n");
		else
			printf("\nEl ganador es el Jugador %d\n", ((ganador > 0) ? 1 : 2));
	}

	free(Tablero->matriz); 												/* Se libera la el espacio donde se habia creado el tablero */
}


void CargarErrores (char **vecErrores) 															/* Inicializa el vector de errores */
{
	vecErrores[0] = "La opcion debe ser entre 1 y 4";
	vecErrores[1] = "La cantidad de filas debe ser entre 5 y 30";
	vecErrores[2] = "La cantidad de columnas debe ser entre 5 y 30";
	vecErrores[3] = "El formato introducido es incorrecto";
	vecErrores[4] = "No existe esa poscicion en el tablero";
	vecErrores[5] = "Ese movimiento no esta dentro del rango de movimientos posibles";
	vecErrores[6] = "Ese casillero no corresponde a una ficha tuya";
	vecErrores[7] = "Ese casillero ya esta ocupado";
	vecErrores[8] = "El archivo esta corrupto o no existe";
	vecErrores[9] = "Introduzca s o n";
	vecErrores[10] = "No es un nombre valido para el archivo";
}

void ImprimirError (char **vecErrores, int nroError)											/* Imprime el error correspondiente */
{
	printf("ERROR: %s\n\n", vecErrores[nroError]);

}

void PantallaInicial () 																						/* Menu prinicipal */
{
	printf("BLOB WARS\n\n\n");
	printf("1. Juego de dos jugadores\n");
	printf("2. Juego contra la computadora\n");
	printf("3. Recuperar juego grabado\n");
	printf("4. Terminar\n\n");
}

void CrearTablero (tipoMatriz *Tablero, const char *contenidoTablero, char **vecErrores) 						/* Crea el tablero de juego */
{
	if (contenidoTablero == NULL)
		CondicionesInicilaes(Tablero, vecErrores);
	Tablero->matriz = AlocarTablero(Tablero);
	TableroInicial(Tablero, contenidoTablero);
}

void CondicionesInicilaes (tipoMatriz *Tablero, char **vecErrores) 							/* Obtiene filas, columnas y primer turno */
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

	Tablero->turno = (int) (rand() / ( (double) RAND_MAX + 1) * (2 - 1 + 1) + 1);
}

int get_filas () 																						/* Lee la cantidad de filas */
{
	int fila;
	printf("Ingrese la cantidad de filas del tablero (entre 5 y 30): ");
	fila = get_int();
	return fila;

}

int get_columnas () 																				/* Lee la cantidad de columnas */
{
	int col;
	printf("Ingrese la cantidad de columnas del tablero (entre 5 y 30): ");
	col = get_int();
	return col;
}


void ImprimirTablero (tipoMatriz *Tablero, const char *idJugador)  					/* Recibe un tablero de ints y lo imprime con chars */
{
	int i, j;
	
	printf("\n");
	for (i=0; i<Tablero->filas; i++)
	{
		for (j=0; j<Tablero->columnas; j++)
			printf("%c ", idJugador[Tablero->matriz[i][j]]);
		printf("\n");
	}
	printf("\nJugador 1: %d fichas\t", ContarFichas(Tablero, 1)); 								/* Imprime las fichas de cada jugador */
	printf("Jugador 2: %d fichas\n", ContarFichas(Tablero, 2));
}

int CapturarJugada (tipoMatriz *Tablero, char **movimiento, int *coordenadas, char **vecErrores)
{
	int datosValidos = 0, error; 														/* Se inicializa en 0 para que entre al while */
	
	while (datosValidos == 0) 							
	{
		printf("Introduzca un movimiento: ");
		*movimiento = leerCaracteres();                                                     /* En movimiento se gurda lo que se leyo */
		datosValidos = ValidarParametros (Tablero, movimiento, coordenadas, &error);
		if (datosValidos == 0)
			ImprimirError(vecErrores, error);
	}
	
	return datosValidos; 																/* Retorna un movimiento valido, save o quit */
}

char * leerCaracteres() 																		/* lee caracteres de entrada estandar */
{
	char *aux = NULL, *string = malloc(sizeof(char)); 								/* El malloc inicial es por si no introduce nada */
	int i = 0, a;
	while ((a = getchar()) != '\n')
	{
		if (i % BLOQUE == 0)
		{
			aux = realloc(string, (i + BLOQUE)*sizeof(char));							/* Realoca cada vez que se llena el bloque */
			if (aux != NULL)
				string = aux;
		}
		string[i++] = a;
	}
	string[i++] = 0;
	string = realloc(string, i*sizeof(char)); 					/* Realoca para no desperdiciar si sobra espacio del ultimo bloque */
	return string;
}

int get_int() 																							/* Captura un numero entero */
{
	int a, entero = 0;

	while ((a = getchar()) != '\n')
	{
		if (isdigit(a))
			entero = 10*entero + (a - '0'); 															/* Convierte de char a entero */
		else
		{	
			BORRAR_BUFFER; 													/* Limpia el buffer por si quedan caracteres sin agarrar */
			return -1;
		}
	}

	return entero;
}

char ValidarSiNO (char **vecErrores) 																/* Valida si se ingresa si o no */
{
	int s_n;

	printf("Desea guardar el juego? (1 por no/2 por si) ");
	s_n = get_int();
	while ((s_n != 1) && (s_n != 2))
	{
		ImprimirError(vecErrores, 9);
		printf("Desea guardar el juego? (1 por no/2 por si) ");
		s_n = get_int();		
	}

	return ((s_n==2)?'s':'n');
}

void GuardarJuego (tipoMatriz *Tablero, const char *movimiento, const char *idJugador) 		/* Guarda el juego en el estado actual */
{
	int i, j, k = 0, fichasAuxJ1, fichasAuxJ2;
	char * posciciones = malloc(Tablero->filas * Tablero->columnas);
	FILE * archivo;

	archivo = fopen(movimiento, "wb");																/* Lo guarda en formato binario */
	fwrite(&(Tablero->opcion), sizeof(int), 1, archivo);
	fwrite(&(Tablero->turno), sizeof(int), 1, archivo);
	fwrite(&(Tablero->filas), sizeof(int), 1, archivo);
	fwrite(&(Tablero->columnas), sizeof(int), 1, archivo);
	fichasAuxJ1 = ContarFichas(Tablero, 1);
	fwrite(&fichasAuxJ1, sizeof(int), 1, archivo);
	fichasAuxJ2 = ContarFichas(Tablero, 2);
	fwrite(&fichasAuxJ2, sizeof(int), 1, archivo);

	for (i=0; i<Tablero->filas; i++)												/* Pasa de ints a chars para guardar como chars */
	{
		for (j=0; j<Tablero->columnas; j++)
		{
			posciciones[k] = idJugador[Tablero->matriz[i][j]];
			k++;
		}
	}
	fwrite(posciciones, Tablero->filas * Tablero->columnas, 1, archivo);
	
	fclose(archivo);
}

int RecuperarJuego(tipoMatriz *Tablero, char **contenidoTablero, char **vecErrores)					/* Recupera un juego gurdado */
{
	int fichasJ1Aux, fichasJ2Aux;
	char *nombre = NULL;
	FILE * archivo;
	
	printf("Introduzca el nombre del archivo o \"cancelar\" para volver al menu: ");
	nombre = leerCaracteres();
	if(strcmp(nombre, "cancelar")==0) 								/*Si lo que se introduce es cancelar sale de recuperar juego*/
	{
		(Tablero->opcion=0);
		return 0;
	}
	archivo = fopen(nombre, "rb");																	/* Lee en formato binario */
	while (archivo == NULL)											/* Pide nombre de archivo hasta que encuentre uno valido */
	{
		ImprimirError(vecErrores, 8);
		printf("Introduzca el nombre del archivo o \"cancelar\" para volver al menu: ");
		nombre = leerCaracteres();
		if(strcmp(nombre, "cancelar")==0)
		{
			(Tablero->opcion=0);
			return 0;
		}
		archivo = fopen(nombre, "rb");
	}
	
	fread(&(Tablero->opcion), sizeof(int), 1, archivo);
	fread(&(Tablero->turno), sizeof(int), 1, archivo);
	fread(&(Tablero->filas), sizeof(int), 1, archivo);
	fread(&(Tablero->columnas), sizeof(int), 1, archivo);
	fread(&fichasJ1Aux, sizeof(int), 1, archivo);
	fread(&fichasJ2Aux, sizeof(int), 1, archivo);

	*contenidoTablero = malloc (Tablero->filas * Tablero->columnas);	/* Asigna memoria al vector donde se recupera el tablero */
	fread(*contenidoTablero, Tablero->filas * Tablero->columnas, 1, archivo);

	fclose(archivo);

	return 1;
}
