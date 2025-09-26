#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockChain.h"      
#include "arbolValidacion.h"

#define CANT_BLOCKCHAINS 3
#define MAX_MENSAJE_LEN 100

void imprimir_blockchain(NodoBlockchain* head, int index) {
    printf("Blockchain %d: ", index);
    if (head == NULL) {
        printf("Vacia\n");
        return;
    }
    
    NodoBlockchain* temp = head;
    while (temp->anterior != NULL) {
        temp = temp->anterior;
    }
    
    while (temp != NULL) {
        printf("%d", temp->id);
        if (temp->siguiente != NULL) {
            printf(" -> ");
        }
        temp = temp->siguiente;
    }
    printf(" -> (NULL)\n");
}

void imprimir_arbol(ArbolValidacion* arbol) {
    if (arbol != NULL && arbol->heap != NULL) {
        printf("Raiz del arbol de validacion: %lld\n", arbol->heap[0]);
    } else {
        printf("Arbol de validacion no inicializado.\n");
    }
}

void menu_alta(NodoBlockchain **blockchains, ArbolValidacion *arbol, int *lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    int index;
    char mensaje[MAX_MENSAJE_LEN];

    printf("\n---- ALTA DE NODO ----\n");
    printf("Ingrese el indice de la blockchain (0 a %d): ", CANT_BLOCKCHAINS - 1);
    if (scanf("%d", &index) != 1 || index < 0 || index >= CANT_BLOCKCHAINS) {
        printf("Indice invalido.\n"); while (getchar() != '\n'); return;
    }
    
    printf("Ingrese el mensaje del nodo (max %d chars): ", MAX_MENSAJE_LEN);
    while (getchar() != '\n');
    if (fgets(mensaje, MAX_MENSAJE_LEN, stdin) == NULL) {
        printf("Error al leer el mensaje.\n"); return;
    }
    mensaje[strcspn(mensaje, "\n")] = 0;
    
    alta(blockchains, arbol, index, mensaje, &lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
    printf("Nodo agregado exitosamente. Nuevo estado:\n");
    imprimir_blockchain(blockchains[index], index);
    imprimir_arbol(arbol);
}

void menu_actualizar(NodoBlockchain **blockchains, ArbolValidacion *arbol, int *lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    int index;
    int id_nodo;
    char mensaje[MAX_MENSAJE_LEN];

    printf("\n---- ACTUALIZAR NODO ----\n");
    printf("Ingrese el indice de la blockchain (0 a %d): ", CANT_BLOCKCHAINS - 1);
    if (scanf("%d", &index) != 1 || index < 0 || index >= CANT_BLOCKCHAINS) {
        printf("Indice invalido o error de lectura.\n"); while (getchar() != '\n'); return;
    }
    
    printf("Ingrese el ID del nodo a modificar (debe ser un ID existente): ");
    if (scanf("%d", &id_nodo) != 1) {
        printf("Error: Ingrese un ID valido.\n"); while (getchar() != '\n'); return;
    }
    
    printf("Ingrese el nuevo mensaje: ");
    while (getchar() != '\n');
    if (fgets(mensaje, MAX_MENSAJE_LEN, stdin) == NULL) {
        printf("Error al leer el mensaje.\n"); return;
    }
    mensaje[strcspn(mensaje, "\n")] = 0; 

    actualizar(blockchains, arbol, index, id_nodo, mensaje, &lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
    printf("Intentando actualizar el nodo ID %d. Nuevo estado:\n", id_nodo);
    imprimir_blockchain(blockchains[index], index);
    imprimir_arbol(arbol);
}

void menu_validar_subconjunto(NodoBlockchain **blockchains) {
    int min_index, max_index;
    long long valor_esperado;

    printf("\n---- VALIDAR SUBCONJUNTO ----\n");
    printf("Ingrese el indice minimo (0 a %d): ", CANT_BLOCKCHAINS - 1);
    if (scanf("%d", &min_index) != 1 || min_index < 0 || min_index >= CANT_BLOCKCHAINS) {
        printf("Indice minimo invalido.\n"); while (getchar() != '\n'); return;
    }
    
    printf("Ingrese el indice maximo (0 a %d): ", CANT_BLOCKCHAINS - 1);
    if (scanf("%d", &max_index) != 1 || max_index < 0 || max_index >= CANT_BLOCKCHAINS || max_index < min_index) {
        printf("Indice maximo invalido.\n"); while (getchar() != '\n'); return;
    }

    printf("Ingrese el valor esperado para el subconjunto: ");
    if (scanf("%lld", &valor_esperado) != 1) {
        printf("Error: Ingrese un número válido.\n"); while (getchar() != '\n'); return;
    }

    if (validar_subconjunto(blockchains, CANT_BLOCKCHAINS, valor_esperado, min_index, max_index)) {
        printf("\nRESULTADO: El subconjunto {%d a %d} es VALIDO.\n", min_index, max_index);
    } else {
        printf("\nRESULTADO: El subconjunto {%d a %d} es INVALIDO.\n", min_index, max_index);
    }
}

int main() {
    int opcion;
    int cant_primos_totales = 0;
    int cant_primos_usados = 0;
    
    int *lista_primos = crear_primos(&cant_primos_totales); 

    NodoBlockchain **blockchains = crear_blockchains(CANT_BLOCKCHAINS);
    ArbolValidacion *arbol = crear_arbol_validacion(CANT_BLOCKCHAINS);

    if (blockchains == NULL || arbol == NULL) {
        printf("Error al inicializar las estructuras. Saliendo.\n");
        return 1;
    }

    do {
        printf("\n\n=========================== MENU ===========================");
        printf("\n1. Probar Alta de Nodo");
        printf("\n2. Probar Actualizar Nodo");
        printf("\n3. Probar Validacion Total");
        printf("\n4. Probar Validar Subconjunto");
        printf("\n5. Mostrar Estado Actual de Blockchains y Arbol");
        printf("\n0. Salir y Liberar Memoria");
        printf("\nSeleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            opcion = -1;
            while (getchar() != '\n');
        }

        switch (opcion) {
            case 1:
                menu_alta(blockchains, arbol, lista_primos, &cant_primos_usados, &cant_primos_totales);
                break;
            case 2:
                menu_actualizar(blockchains, arbol, lista_primos, &cant_primos_usados, &cant_primos_totales);
                break;
            case 3:
                printf("\n---- VALIDACION TOTAL ----\n");
                if (validar(blockchains, arbol, CANT_BLOCKCHAINS)) {
                    printf("RESULTADO: La red federada es VALIDA.\n");
                } else {
                    printf("RESULTADO: La red federada es INVALIDA.\n");
                }
                break;
            case 4:
                menu_validar_subconjunto(blockchains);
                break;
            case 5:
                printf("\n---- ESTADO ACTUAL .---\n");
                for (int i = 0; i < CANT_BLOCKCHAINS; i++) {
                    imprimir_blockchain(blockchains[i], i);
                }
                imprimir_arbol(arbol);
                break;
            case 0:
                printf("\nSaliendo del programa. :(\n");
                break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
                break;
        }

    } while (opcion != 0);

    destruir_blockchain_y_primos(blockchains, CANT_BLOCKCHAINS, lista_primos);
    destruir_arbol_validacion(arbol);
    printf("Memoria liberada exitosamente.\n");

    return 0;
}