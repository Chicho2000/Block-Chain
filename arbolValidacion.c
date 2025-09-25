#include <stdlib.h>
#include <math.h>
#include "arbolValidacion.h"

static int get_parent(int i){
     return (i - 1) / 2; 
}

static int get_left_child(int i) { 
    return 2 * i + 1; 
}

static int get_right_child(int i) { 
    return 2 * i + 2; 
}

ArbolValidacion *crear_arbol_validacion(int num_hojas) {
    ArbolValidacion *arbol = (ArbolValidacion *)malloc(sizeof(ArbolValidacion));
    if (arbol == NULL) return NULL;
    
    int hojas_completas = pow(2, ceil(log2(num_hojas)));
    arbol->capacidad = 2 * hojas_completas - 1;
    arbol->heap = (long long *)malloc(sizeof(long long) * arbol->capacidad);
    if (arbol->heap == NULL) {
        free(arbol);
        return NULL;
    }
    for(int i = 0; i < arbol->capacidad; i++) {
        arbol->heap[i] = 1;
    }
    return arbol;
}

void actualizar_hoja_y_propagar(ArbolValidacion *arbol, int indice_hoja, long long nuevo_valor) {
    if (arbol == NULL || arbol->heap == NULL) return;
    
    int hojas_completas = (arbol->capacidad + 1) / 2;
    int indice_en_heap = hojas_completas - 1 + indice_hoja;

    if (indice_en_heap >= arbol->capacidad) return;

    arbol->heap[indice_en_heap] = nuevo_valor;

    int i = indice_en_heap;
    while (i > 0) {
        int parent = get_parent(i);
        long long left_child_val = (get_left_child(parent) < arbol->capacidad) ? arbol->heap[get_left_child(parent)] : 1;
        long long right_child_val = (get_right_child(parent) < arbol->capacidad) ? arbol->heap[get_right_child(parent)] : 1;
        
        arbol->heap[parent] = left_child_val * right_child_val;
        i = parent;
    }
}

void destruir_arbol_validacion(ArbolValidacion *arbol) {
    if (arbol == NULL) return;
    free(arbol->heap);
    free(arbol);
}
