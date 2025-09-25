#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "generador_primos.h"
#include "arbolValidacion.h"

typedef struct NodoBlockchain {
    int id;
    char *mensaje;
    struct NodoBlockchain *anterior;
    struct NodoBlockchain *siguiente;
} NodoBlockchain;

NodoBlockchain **crear_blockchains(int num_blockchains);
int *crear_primos(int *cant_totales)
int obtener_nuevo_primo(int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr);
void alta(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, const char *mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr);
void actualizar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, int id_nodo, const char *nuevo_mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr);
int validar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains);
int validar_subconjunto(NodoBlockchain **blockchains, int num_blockchains, long long valor_esperado, int min_index, int max_index);
void destruir_blockchain_y_primos(NodoBlockchain **blockchains, int num_blockchains, int *lista_primos);

#endif

