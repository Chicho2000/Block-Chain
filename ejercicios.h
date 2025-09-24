#include<stdio.h>
#include<stdlib.h>
#include<blockChain.h>
#include<arbolValidacion.h>

void actualizar_hoja_y_propagar(ArbolValidacion *arbol, int indice_hoja, long long nuevo_valor);

void alta(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, const char *mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr);

void actualizar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, int id_nodo, const char *nuevo_mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr);

int validar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains);

int validar_subconjunto(NodoBlockchain **blockchains, int num_blockchains, long long valor_esperado, int min_index, int max_index);

void destruir_red(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains, int *lista_primos);




