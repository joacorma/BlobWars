#ifndef _blobsBack_h
#define _blobsBack_h

typedef struct
	{	
		int filas;
		int columnas;
		int **matriz;
		int opcion;
		int turno;
	} tipoMatriz;

int ** CrearTablero(tipoMatriz *Tablero);
void EjecutarJugada(tipoMatriz *Tablero, int *mov);
int ValidarParametros(tipoMatriz *Tablero, char **movimiento, int *mov, char **vecErrores);
int CalcularDistancia(int u, int x, int v, int y);
void CrearBlob(tipoMatriz *Tablero ,int fila, int columna);
void Adyacentes(tipoMatriz *Tablero, int fila, int columna);
void Salto(tipoMatriz *Tablero, int *mov);
int Fin(tipoMatriz *Tablero);
void LLenarTablero(tipoMatriz *Tablero);
int ContarBlobs(tipoMatriz *Tablero);
int ValidarSave(char **movimiento);
int RecuperarJuego(tipoMatriz *Tablero, char **vecErrores);
int JugadaComputadora (tipoMatriz *Tablero, int *mov);
char * leerCaracteres ();/*hay que sacarlo de aca*/
void ImprimirError (char **vecErrores, int nroError);/*hay que sacarla de aca, y ponerla en front*/
void Save(tipoMatriz *Tablero, char  *movimiento, char *idJugador);
int JugadaComputadora(tipoMatriz *Tablero, int *mov);
int ContarFichas(tipoMatriz *Tablero, int jugador);

#endif
