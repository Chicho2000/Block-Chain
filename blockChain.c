#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "blockchain.h"
#include "generador_primos.h"
#include "arbolValidacion.h"

NodoBlockchain **crear_blockchains(int num_blockchains) {
    NodoBlockchain **blockchains = (NodoBlockchain **)malloc(sizeof(NodoBlockchain *) * num_blockchains);
    if (blockchains == NULL) return NULL;
    for (int i = 0; i < num_blockchains; i++) {
        blockchains[i] = NULL;
    }
    return blockchains;
}

int *crear_primos(int *cant_totales) {
    *cant_totales = 200;
    return primos(*cant_totales);
}

static int obtener_nuevo_primo(int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    if (*cant_primos_usados_ptr >= *cant_primos_totales_ptr) {
        free(*lista_primos_ptr);
        *cant_primos_totales_ptr += 200;
        *lista_primos_ptr = primos(*cant_primos_totales_ptr);
    }
    return (*lista_primos_ptr)[(*cant_primos_usados_ptr)++];
}

void alta(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, const char *mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    if (blockchains == NULL || blockchain_index < 0 || arbol_validacion == NULL) return;

    NodoBlockchain *nuevo_nodo = (NodoBlockchain *)malloc(sizeof(NodoBlockchain));
    if (nuevo_nodo == NULL) return;
    
    nuevo_nodo->mensaje = strdup(mensaje);
    nuevo_nodo->id = obtener_nuevo_primo(lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
    nuevo_nodo->siguiente = NULL; 

    if (blockchains[blockchain_index] == NULL) {
        nuevo_nodo->anterior = NULL;
        blockchains[blockchain_index] = nuevo_nodo;
    } else {
        nuevo_nodo->anterior = blockchains[blockchain_index];
        blockchains[blockchain_index]->siguiente = nuevo_nodo;
        blockchains[blockchain_index] = nuevo_nodo;
    }

    actualizar_hoja_y_propagar(arbol_validacion, blockchain_index, nuevo_nodo->id);
}

void actualizar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, int id_nodo, const char *nuevo_mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    if (blockchains == NULL || blockchain_index < 0 || arbol_validacion == NULL) return;

    NodoBlockchain *actual = blockchains[blockchain_index];
    NodoBlockchain *nodo_a_modificar = NULL;

    while (actual != NULL) {
        if (actual->id == id_nodo) {
            nodo_a_modificar = actual;
            break; 
        }
        actual = actual->anterior;
    }

    if (nodo_a_modificar == NULL) {
        printf("Nodo con ID %d no encontrado.\n", id_nodo);
        return;
    }

    free(nodo_a_modificar->mensaje);
    nodo_a_modificar->mensaje = strdup(nuevo_mensaje);
    if (nodo_a_modificar->mensaje == NULL) {
        return;
    }
    
    nodo_a_modificar->id = obtener_nuevo_primo(lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
    actual = nodo_a_modificar->siguiente;
    while(actual != NULL) {
        actual->id = obtener_nuevo_primo(lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
        actual = actual->siguiente;
    }

    actualizar_hoja_y_propagar(arbol_validacion, blockchain_index, blockchains[blockchain_index]->id);
}

int validar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains) {
    if (blockchains == NULL || arbol_validacion == NULL || arbol_validacion->heap == NULL) return 0;
    
    // Verifico IDs crecientes en cada blockchain
    for (int i = 0; i < num_blockchains; i++) {
        NodoBlockchain *actual = blockchains[i];
        while (actual != NULL && actual->anterior != NULL) {
            if (actual->id <= actual->anterior->id) {
                return 0;
            }
            actual = actual->anterior;
        }
    }
    
    // Verifico la raiz del arbol de validacion
    long long valor_esperado_raiz = 1;
    for (int i = 0; i < num_blockchains; i++) {
        NodoBlockchain *ultimo_nodo = blockchains[i]; 
        if (ultimo_nodo != NULL) {
            valor_esperado_raiz *= ultimo_nodo->id;
        }
    }
    
    return (arbol_validacion->heap[0] == valor_esperado_raiz);
}

int validar_subconjunto(NodoBlockchain **blockchains, int num_blockchains, long long valor_esperado, int min_index, int max_index) {
    if (blockchains == NULL || min_index < 0 || max_index >= num_blockchains || min_index > max_index) return 0;
    
    long long valor_calculado = 1;
    for (int i = min_index; i <= max_index; i++) {
        NodoBlockchain *ultimo_nodo = blockchains[i];
        if (ultimo_nodo != NULL) {
            valor_calculado *= ultimo_nodo->id;
        } 
    }
    
    return (valor_calculado == valor_esperado);
}

void destruir_blockchain_y_primos(NodoBlockchain **blockchains, int num_blockchains, int *lista_primos) {
    if (blockchains == NULL) return;
    
    for (int i = 0; i < num_blockchains; i++) {
        NodoBlockchain *actual = blockchains[i];
        while (actual != NULL) {
            NodoBlockchain *temp = actual;
            actual = actual->anterior;
            free(temp->mensaje);
            free(temp);
        }
    }
    
    free(blockchains);
    free(lista_primos);
}
