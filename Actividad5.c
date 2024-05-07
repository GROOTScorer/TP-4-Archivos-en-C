#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS 100

typedef struct {
    char nombre[30];
    char apellido[30];
    int dni;
} Persona;

Persona personas[MAX_REGISTROS];
int num_registros = 0;
int num_registros_cambios = 0;

void guardar_datos() {
    FILE *archivo = fopen("cambios.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    for (int i = 0; i < num_registros_cambios; i++) {
        fprintf(archivo, "%s,%s,%d\n", personas[i].nombre, personas[i].apellido, personas[i].dni);
    }

    fclose(archivo);
    printf("Datos guardados en el archivo cambios.txt\n");
}

void cargar_datos() {
    FILE *archivo = fopen("datos.txt", "r");
    if (archivo == NULL) {
        printf("No se encontró el archivo datos.txt\n");
        return;
    }

    num_registros = 0;
    char linea[100];
    while (fgets(linea, sizeof(linea), archivo)) {
        char *token = strtok(linea, ",");
        strcpy(personas[num_registros].nombre, token);
        token = strtok(NULL, ",");
        strcpy(personas[num_registros].apellido, token);
        personas[num_registros].dni = atoi(strtok(NULL, ","));
        num_registros++;
    }

    fclose(archivo);
    printf("Datos cargados desde el archivo datos.txt\n");
}

void cargar_cambios() {
    num_registros_cambios = 0;
    char linea_cambio[100];
    FILE *archivo = fopen("cambios.txt", "r");
    if (archivo == NULL) {
        printf("No se encontró el archivo cambios.txt\n");
        return;
    }

    while (fgets(linea_cambio, sizeof(linea_cambio), archivo)) {
        char *token = strtok(linea_cambio, ",");
        strcpy(personas[num_registros_cambios].nombre, token);
        token = strtok(NULL, ",");
        strcpy(personas[num_registros_cambios].apellido, token);
        personas[num_registros_cambios].dni = atoi(strtok(NULL, ","));
        num_registros_cambios++;
    }

    fclose(archivo);

    printf("Datos cargados desde el archivo cambios.txt\n");
}

void agregar_persona() {
    if (num_registros_cambios >= MAX_REGISTROS) {
        printf("No se pueden agregar más registros.\n");
        return;
    }

    printf("Ingrese el nombre: ");
    scanf("%s", personas[num_registros_cambios].nombre);
    printf("Ingrese el apellido: ");
    scanf("%s", personas[num_registros_cambios].apellido);
    printf("Ingrese el DNI: ");
    scanf("%d", &personas[num_registros_cambios].dni);

    int existe = 0;
    for (int i = 0; i < num_registros_cambios; i++) {
        if (personas[i].dni == personas[num_registros_cambios].dni) {
            printf("Ya existe un registro con ese DNI.\n");
            existe = 1;
            break;
        }
    }

    if (!existe) {
        num_registros_cambios++;
        printf("Persona agregada correctamente.\n");
    }
}

void buscar_por_dni() {
    int dni;
    printf("Ingrese el DNI a buscar: ");
    scanf("%d", &dni);

    int encontrado = 0;
    for (int i = 0; i < num_registros; i++) {
        if (personas[i].dni == dni) {
            printf("Nombre: %s\nApellido: %s\n", personas[i].nombre, personas[i].apellido);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontró ninguna persona con ese DNI.\n");
    }
}

void buscar_por_nombre_apellido() {
    char nombre[50], apellido[50];
    printf("Ingrese el nombre a buscar: ");
    scanf("%s", nombre);
    printf("Ingrese el apellido a buscar: ");
    scanf("%s", apellido);

    int encontrado = 0;
    for (int i = 0; i < num_registros; i++) {
        if (strcmp(personas[i].nombre, nombre) == 0 && strcmp(personas[i].apellido, apellido) == 0) {
            printf("DNI: %d\n", personas[i].dni);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontró ninguna persona con ese nombre y apellido.\n");
    }
}

void ordenar_por_nombre_apellido() {
    for (int i = 0; i < num_registros - 1; i++) {
        for (int j = 0; j < num_registros - i - 1; j++) {
            if (strcmp(personas[j].apellido, personas[j + 1].apellido) > 0 ||
                (strcmp(personas[j].apellido, personas[j + 1].apellido) == 0 &&
                 strcmp(personas[j].nombre, personas[j + 1].nombre) > 0)) {
                Persona temp = personas[j];
                personas[j] = personas[j + 1];
                personas[j + 1] = temp;
            }
        }
    }

    printf("Personas ordenadas por apellido y nombre:\n");
    for (int i = 0; i < num_registros; i++) {
        printf("%s, %s - DNI: %d\n", personas[i].apellido, personas[i].nombre, personas[i].dni);
    }
}

void ordenar_por_dni() {
    for (int i = 0; i < num_registros - 1; i++) {
        for (int j = 0; j < num_registros - i - 1; j++) {
            if (personas[j].dni > personas[j + 1].dni) {
                Persona temp = personas[j];
                personas[j] = personas[j + 1];
                personas[j + 1] = temp;
            }
        }
    }

    printf("Personas ordenadas por DNI:\n");
    for (int i = 0; i < num_registros; i++) {
        printf("%d - %s, %s\n", personas[i].dni, personas[i].apellido, personas[i].nombre);
    }
}

void guardar_datos_ordenados() {
    FILE *archivo = fopen("datos_ordenados.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("¿Cómo desea ordenar los datos?\n");
    printf("1. Por nombre y apellido\n");
    printf("2. Por DNI\n");
    int opcion;
    scanf("%d", &opcion);

    if (opcion == 1) {
        ordenar_por_nombre_apellido();
    } else {
        ordenar_por_dni();
    }

    for (int i = 0; i < num_registros; i++) {
        fprintf(archivo, "%s,%s,%d\n", personas[i].nombre, personas[i].apellido, personas[i].dni);
    }

    fclose(archivo);
    printf("Datos guardados en el archivo datos_ordenados.txt\n");
}

int main() {
    cargar_datos();
    cargar_cambios();

    int opcion;
    do {
        printf("\nMenú:\n");
        printf("1. Agregar persona\n");
        printf("2. Buscar por DNI\n");
        printf("3. Buscar por nombre y apellido\n");
        printf("4. Ordenar y guardar datos\n");
        printf("5. Salir\n");
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregar_persona();
                break;
            case 2:
                buscar_por_dni();
                break;
            case 3:
                buscar_por_nombre_apellido();
                break;
            case 4:
                guardar_datos_ordenados();
                break;
            case 5:
                guardar_datos();
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 5);

    return 0;
}
