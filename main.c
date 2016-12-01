#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fichero {
	char nombre[10];
	int size;
	int pista;
} fichero;

int main(int argc, char const *argv[]) {
	FILE *archivo = NULL;
	FILE *bloques = NULL;
	fichero disco[100];
	char nombre_tmp[10];
	int bloques_aux[100] = {[0 ... 99] = -1};
	int size_tmp, pista_tmp = 0, pistas_dir_ext = 0, i = 1, j = 0;
	int cabeza_final, cabeza_inicial, cabeza, direccion, cabeza_pista;
	float t_busqueda = 0, t_total = 0;
	int ultima;
	archivo = fopen("discoC","r");
	bloques = fopen("bloques.txt", "w");

	while (feof(archivo) == 0) {
		bloques_aux[j] = i;
		fscanf(archivo, "%s %d", nombre_tmp, &size_tmp);
		printf("El archivo '%s' ocupa los bloques: ", nombre_tmp);
		while (--size_tmp >= 0) {
			if (i == 100) {
				printf("\nERROR: No hay espacio suficiente en el disco.\n");
				printf("La ejecución del programa termina en este punto.\n");
				exit(0);
			}
			printf("%d -> ", i);
			strcpy(disco[i++].nombre, nombre_tmp);
			if (i % 5 == 0)
				pista_tmp ++;
			disco[i].pista = pista_tmp;			
		}		
		printf("\u23DA\n");
		j++;
	}	
	cabeza_final = i;
	if (cabeza_final % 5 == 0)
		cabeza_pista = pista_tmp - 1;
	else
		cabeza_pista = pista_tmp;
	direccion = 1;

	for (j = 99; j >= 0; j--)
		if (bloques_aux[j] != -1)
			fprintf(bloques, "%d\n", bloques_aux[j]);
	
	fclose(archivo);
	fclose(bloques);

	bloques = fopen("bloques.txt", "r");
	printf("\nLeyendo del disco...\n\n");
	printf("La cabeza lectora está en la pista %d con dirección hacia ", cabeza_pista);
	printf( (direccion == 1) ? "la pista más externa.\n":"la pista más interna.\n");
	printf("Bloque\tPista\tArchivo\t\tTiempo de  Dirección\n");
	printf("      \t     \t       \t\tbúsqueda   de cabeza\n");
	while (cabeza_final != 1) {	
		fscanf(bloques, "%d", &cabeza);
		cabeza_inicial = cabeza;
		pista_tmp = disco[cabeza].pista;
		
		if (cabeza_pista < pista_tmp && direccion == -1){
			t_busqueda += 5;
			direccion = 1;
		} else if (cabeza_pista > pista_tmp && direccion == 1) {
			t_busqueda += 5;
			direccion = -1;
		}
		t_busqueda += abs(cabeza_pista - pista_tmp) * 1.5;	
		while (cabeza != cabeza_final) {
			printf("%d\t%d\t%10s\t", cabeza, disco[cabeza].pista, disco[cabeza].nombre);
			if (pista_tmp != disco[cabeza].pista) {				
				pistas_dir_ext ++;
				if (pistas_dir_ext == 1) {
					t_busqueda += 6.5;
					direccion = direccion * -1;
				} else
					t_busqueda += 1.5;
				pista_tmp = disco[cabeza].pista;
			}
			printf("%.1f\t", t_busqueda);
			printf( (direccion == 1) ? "   P. más externa\n":"   P. más interna\n");
			cabeza ++;
			cabeza_pista = pista_tmp;
		}
		t_total += t_busqueda;
		pistas_dir_ext = 0;
		t_busqueda = 0;
		ultima = cabeza_final;
		cabeza_final = cabeza_inicial;
	}
	fclose(bloques);
		printf("El tiempo de búsqueda total fue de %.1f ms.\n", t_total);
	
	return 0;	
}