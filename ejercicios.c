#include<stdio.h>
#include<stdlib.h>

void actualizar_hoja_y_propagar(ArbolValidacion *arbol, int indice_hoja, long long nuevo_valor) {
    if (arbol == NULL || indice_hoja >= arbol->capacidad) return;
    
    // El indice de la hoja en el heap es (capacidad/2) + indice_blockchain
    int indice_en_heap = (arbol->capacidad / 2) + indice_hoja;
    arbol->heap[indice_en_heap] = nuevo_valor;

    // Propagar el cambio hacia la raiz
    int i = indice_en_heap;
    while (i > 0) {
        int parent = get_parent(i);
        long long left_child_val = (get_left_child(parent) < arbol->capacidad) ? arbol->heap[get_left_child(parent)] : 1;
        long long right_child_val = (get_right_child(parent) < arbol->capacidad) ? arbol->heap[get_right_child(parent)] : 1;
        
        arbol->heap[parent] = left_child_val * right_child_val;
        i = parent;
    }
}

void alta(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int blockchain_index, const char *mensaje, int **lista_primos_ptr, int *cant_primos_usados_ptr, int *cant_primos_totales_ptr) {
    if (blockchains == NULL || blockchain_index < 0 || arbol_validacion == NULL) return;

    NodoBlockchain *nuevo_nodo = (NodoBlockchain *)malloc(sizeof(NodoBlockchain));
    if (nuevo_nodo == NULL) return;
    
    nuevo_nodo->mensaje = strdup(mensaje);
    nuevo_nodo->id = obtener_nuevo_primo(lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
    nuevo_nodo->anterior = NULL;

    if (blockchains[blockchain_index] == NULL) {
        blockchains[blockchain_index] = nuevo_nodo;
    } else {
        NodoBlockchain *actual = blockchains[blockchain_index];
        while (actual->anterior != NULL) {
            actual = actual->anterior;
        }
        actual->anterior = nuevo_nodo;
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
    
    actual = nodo_a_modificar;
    while(actual != NULL) {
        actual->id = obtener_nuevo_primo(lista_primos_ptr, cant_primos_usados_ptr, cant_primos_totales_ptr);
        actual = actual->anterior;
    }
    
    NodoBlockchain *ultimo_nodo = blockchains[blockchain_index];
    while(ultimo_nodo->anterior != NULL) {
        ultimo_nodo = ultimo_nodo->anterior;
    }
    actualizar_hoja_y_propagar(arbol_validacion, blockchain_index, ultimo_nodo->id);
}

int validar(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains) {
    if (blockchains == NULL || arbol_validacion == NULL || arbol_validacion->heap == NULL) return 0;
    
    // 1. Verificar IDs crecientes en cada blockchain
    for (int i = 0; i < num_blockchains; i++) {
        NodoBlockchain *actual = blockchains[i];
        while (actual != NULL && actual->anterior != NULL) {
            if (actual->id <= actual->anterior->id) {
                return 0;
            }
            actual = actual->anterior;
        }
    }
    
    // 2. Verificar la raiz del arbol de validacion
    long long valor_esperado_raiz = 1;
    for (int i = 0; i < num_blockchains; i++) {
        NodoBlockchain *actual = blockchains[i];
        while (actual != NULL && actual->anterior != NULL) {
            actual = actual->anterior;
        }
        if (actual != NULL) {
            valor_esperado_raiz *= actual->id;
        }
    }
    
    return (arbol_validacion->heap[0] == valor_esperado_raiz);
}

int validar_subconjunto(NodoBlockchain **blockchains, int num_blockchains, long long valor_esperado, int min_index, int max_index) {
    if (blockchains == NULL || min_index < 0 || max_index >= num_blockchains || min_index > max_index) return 0;
    
    long long valor_calculado = 1;
    for (int i = min_index; i <= max_index; i++) {
        NodoBlockchain *actual = blockchains[i];
        while (actual != NULL && actual->anterior != NULL) {
            actual = actual->anterior;
        }
        if (actual != NULL) {
            valor_calculado *= actual->id;
        } else {
            valor_calculado *= 1;
        }
    }
    return (valor_calculado == valor_esperado);
}

void destruir_red(NodoBlockchain **blockchains, ArbolValidacion *arbol_validacion, int num_blockchains, int *lista_primos) {
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
    free(arbol_validacion->heap);
    free(arbol_validacion);
    free(lista_primos);
}