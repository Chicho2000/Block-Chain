#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"
#include "arbolValidacion.h"

#define CANT_BLOCKCHAINS 3

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

int main() {
    int cant_primos_totales = 0;
    int cant_primos_usados = 0;
    int *lista_primos = crear_primos(&cant_primos_totales);

    NodoBlockchain **blockchains = crear_blockchains(CANT_BLOCKCHAINS);
    ArbolValidacion *arbol = crear_arbol_validacion(CANT_BLOCKCHAINS);

    if (blockchains == NULL || arbol == NULL) {
        printf("Error al inicializar las estructuras.\n");
        return 1;
    }

    printf("Inicializacion y Alta:\n");
    printf("Agrego nodos a las blockchains\n");
    
    alta(blockchains, arbol, 0, "Primer mensaje en cadena 0", &lista_primos, &cant_primos_usados, &cant_primos_totales);
    int id_a_modificar = blockchains[0]->id;
    

    alta(blockchains, arbol, 1, "Mensaje 1 en cadena 1", &lista_primos, &cant_primos_usados, &cant_primos_totales);
    alta(blockchains, arbol, 2, "Mensaje 1 en cadena 2", &lista_primos, &cant_primos_usados, &cant_primos_totales);
    alta(blockchains, arbol, 0, "Segundo mensaje en cadena 0", &lista_primos, &cant_primos_usados, &cant_primos_totales);


    imprimir_blockchain(blockchains[0], 0);
    imprimir_blockchain(blockchains[1], 1);
    imprimir_blockchain(blockchains[2], 2);
    imprimir_arbol(arbol);

    printf("\nValidacion Inicial\n");
    if (validar(blockchains, arbol, CANT_BLOCKCHAINS)) {
        printf("Validacion: Correcta\n");
    } else {
        printf("Validacion: Incorrecta\n");
    }



    printf("\nOperacion de Actualizacion\n");
    printf("Modificando nodo con ID %d en la blockchain 0\n", id_a_modificar);
    actualizar(blockchains, arbol, 0, id_a_modificar, "Nuevo Mensaje", &lista_primos, &cant_primos_usados, &cant_primos_totales);
  
    imprimir_blockchain(blockchains[0], 0);
    imprimir_blockchain(blockchains[1], 1);
    imprimir_blockchain(blockchains[2], 2);
    imprimir_arbol(arbol);

    printf("\nValidacion despues de la post-actualizacion\n");
    if (validar(blockchains, arbol, CANT_BLOCKCHAINS)) {
        printf("Validacion: Correcta\n");
    } else {
        printf("Validacion: Incorrecta\n");
    }



    printf("\nValidacion de Subconjunto\n");

    long long valor_esperado = blockchains[0]->id * blockchains[1]->id;
    printf("Validando subconjunto {0, 1}. Valor esperado: %lld\n", valor_esperado);
    if (validar_subconjunto(blockchains, CANT_BLOCKCHAINS, valor_esperado, 0, 1)) {
        printf("Validacion de subconjunto: Correcta\n");
    } else {
        printf("Validacion de subconjunto: Incorrecta\n");
    }
    
    destruir_blockchain_y_primos(blockchains, CANT_BLOCKCHAINS, lista_primos);
    destruir_arbol_validacion(arbol);
    
    return 0;
}