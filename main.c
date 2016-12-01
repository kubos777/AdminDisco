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
	int size_tmp, pista_tmp = 0, pistas_dir_ext = 0, bloque = 1, j = 0;
	int bloque_final, bloque_inicial, cabeza, direccion, cabeza_pista;
	float t_busqueda = 0, t_total = 0;
//EMPIEZA LECTURA DE ARCHIVO.
	archivo = fopen("discoC","r"); 		//Apertura del archivo que contiene los "archivos" a leer
	bloques = fopen("bloques.txt", "w");//Apertura del archivo donde se almacenará el bloque donde empieza cada "archivo".
	//Se lee el archivo con archivos y tamaños
	while (feof(archivo) == 0) {
		bloques_aux[j] = bloque; 	//Se almacena en un arreglo cada bloque donde se haya empezado la escritura de un "archivo".
		fscanf(archivo, "%s %d", nombre_tmp, &size_tmp); //Se lee del archivo el nombre y tamaño de cada "archivo".
		printf("El archivo '%s' ocupa los bloques: ", nombre_tmp);
		while (--size_tmp >= 0) { 	//Conforme se va leyendo y pasando al disco, disminuye el tamaño del "archivo".
			if (bloque == 100) {	//Se verifica que se ocupe más memoria de la que hay.
				printf("\nERROR: No hay espacio suficiente en el disco.\n");
				printf("La ejecución del programa termina en este punto.\n");
				exit(0);
			}
			printf("%d -> ", bloque);	//Se imprime cada bloque que ocupa el "archivo".
			strcpy(disco[bloque++].nombre, nombre_tmp);	//Se copia el nombre del "archivo" al "disco" en el bloque correspondiente.
			if (bloque % 5 == 0)	//Si el bloque actual es múltiplo de 5, hay un cambio de pista.
				pista_tmp ++;
			disco[bloque].pista = pista_tmp;	//Se almacena en que pista está cada bloque del "archivo".		
		}		
		printf("\u23DA\n");
		j++;	//Aumenta el índice del arreglo con bloques donde inician los archivos. 
	}	
	bloque_final = bloque; 	//Se posiciona la "cabeza" en el último bloque donde se trabajó.
	if (bloque_final % 5 == 0)
		cabeza_pista = pista_tmp - 1;
	else
		cabeza_pista = pista_tmp;
	direccion = 1;

	//Se pasa, en orden inverso, el arreglo de bloques de inicio de "archivo" a un archivo.
	for (j = 99; j >= 0; j--)
		if (bloques_aux[j] != -1)
			fprintf(bloques, "%d\n", bloques_aux[j]);
	
	fclose(archivo);
	fclose(bloques);
//EMPIEZA PLANIFICACIÓN DE DISCO.
	bloques = fopen("bloques.txt", "r");
	printf("\nLeyendo del disco...\n\n");
	printf("La cabeza lectora está en la pista %d con dirección hacia ", cabeza_pista);
	printf( (direccion == 1) ? "la pista más externa.\n":"la pista más interna.\n");
	printf("Bloque\tPista\tArchivo\t\tTiempo de  Dirección\n");
	printf("      \t     \t       \t\tbúsqueda   de cabeza\n");
	while (bloque_final != 1) {	//Mientras el final de un "archivo" no esté en el primer bloque...
		fscanf(bloques, "%d", &cabeza);	//Se lee del archivo con bloques de inicio de "archivo" la primera posición de la "cabeza".
		bloque_inicial = cabeza;
		pista_tmp = disco[cabeza].pista;//Se asigna pone la "cabeza" en la "pista" correspondiente.
		
		//Todo este "if" verifica si habrá cambio de dirección de la cabeza para leer el "archivo" y acumula el tiempo correspondiente.
		if (cabeza_pista < pista_tmp && direccion == -1){
			t_busqueda += 5;
			direccion = 1;
		} else if (cabeza_pista > pista_tmp && direccion == 1) {
			t_busqueda += 5;
			direccion = -1;
		}
		t_busqueda += abs(cabeza_pista - pista_tmp) * 1.5; //Se acumula el tiempo de búsqueda.

		while (cabeza != bloque_final) { //Mientras la "cabeza" no llegue al final de un "archivo".
			printf("%d\t%d\t%10s\t", cabeza, disco[cabeza].pista, disco[cabeza].nombre);
			//Este "if" verifica si hubo cambio de pista y de dirección y se acumula el tiempo correspondiente.	
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
			cabeza ++; //La cabeza pasa al siguiente bloque
			cabeza_pista = pista_tmp;	//y se le asigna la pista correspondiente.
		}
		t_total += t_busqueda;	//Se acumulan los tiempos de búsqueda.
		pistas_dir_ext = 0;		//Reinicio de bandera de cambio de pista y dirección.
		t_busqueda = 0;			//Se reinicia el tiempo de búsqueda de "archivo".
		bloque_final = bloque_inicial; //Ahora el bloque final del "archivo" siguiente será el bloque inicial del anterior.
	}
	fclose(bloques);
	printf("El tiempo de búsqueda total fue de %.1f ms.\n", t_total);
	
	return 0;	
}