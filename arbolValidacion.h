#ifndef ARBOL_VALIDACION_H
#define ARBOL_VALIDACION_H

typedef struct ArbolValidacion {
    long long *heap;
    int capacidad;
} ArbolValidacion;

ArbolValidacion *crear_arbol_validacion(int num_hojas);
void actualizar_hoja_y_propagar(ArbolValidacion *arbol, int indice_hoja, long long nuevo_valor);
void destruir_arbol_validacion(ArbolValidacion *arbol);

#endif
