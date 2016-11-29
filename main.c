#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define p printf

typedef struct fichero
{
	char nombre[10];
	int size;
	int pista;
} fichero;

int main(int argc, char const *argv[])
{
	FILE *archivo = NULL;
	FILE *bloques = NULL;
	fichero disco[100];
	char nombre_tmp[10];
	int size_tmp, pista_tmp = 0, i = 1;
	archivo = fopen("discoC","r");
	bloques = fopen("bloques.txt", "w");

	while (feof(archivo) == 0 && i < 99)
	{
		fprintf(bloques, "%d\n", i);
		fscanf(archivo, "%s %d", nombre_tmp, &size_tmp);
		printf("El archivo '%s' ocupa los bloques: ", nombre_tmp);
		while (--size_tmp >= 0)
		{
			printf("%d  ", i);
			strcpy(disco[i++].nombre, nombre_tmp);
			if (i % 5 == 0)
				pista_tmp ++;
			disco[i].pista = pista_tmp;
		}
		printf("\n");
	}	
	int cabeza = pista_tmp;
	
	fclose(archivo);
	fclose(bloques);

	int pistas[200] = {0}, bloque_tmp;
	i = 0;

	bloques = fopen("bloques.txt", "r");
	while (feof(bloques) == 0 && i < 200)
	{
		fscanf(bloques, "%d", &bloque_tmp);
		pistas[i] = bloque_tmp / 5;
		i ++;
	}
	for (i = 0; i < 20; i++)
		printf("%d\n",pistas[i]);

	return 0;	
}