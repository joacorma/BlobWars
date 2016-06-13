#ifndef _blobsBack_h
#define _blobsBack_h

#define MAX_ERRORES 11
#define MIN_FILAS 5
#define MAX_FILAS 30
#define MIN_COLUMNAS 5
#define MAX_COLUMNAS 30
#define BLOQUE 5
#define SAVE 0
#define FILENAME 1

#define BORRAR_BUFFER while (getchar()!='\n')

typedef struct
	{	
		int filas;
		int columnas;
		int **matriz;
		int opcion;
		int turno;
	} tipoMatriz;

void TableroInicial (tipoMatriz *Tablero, char *contenidoTablero);
void EjecutarJugada (tipoMatriz *Tablero, int *mov);
void LLenarTablero (tipoMatriz *Tablero);
void CrearBlob (tipoMatriz *Tablero ,int fila, int columna);
void Salto (tipoMatriz *Tablero, int *mov);
void Adyacentes (tipoMatriz *Tablero, int fila, int columna);
int ValidarParametros (tipoMatriz *Tablero, char **movimiento, int *mov, char **vecErrores, int *error);
int ValidarSave (char **movimiento);
int ValidarLimites (tipoMatriz *Tablero, int *mov);
int CalcularDistancia (int u, int x, int v, int y);
int Fin (tipoMatriz *Tablero);
int ContarFichas (tipoMatriz *Tablero, int jugador);
int JugadaComputadora (tipoMatriz *Tablero, int *mov);
int ** AlocarTablero (tipoMatriz *Tablero);

#endif
