#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define VOLUMEN_INICIAL 10
#define CUADRUPLE 4
#define REDIMENSION 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL){
        return NULL;
    }
    
    pila->datos = malloc(VOLUMEN_INICIAL * sizeof(void*));
    if (pila->datos == NULL){
        free(pila);
        return NULL;
    }

    pila->capacidad = VOLUMEN_INICIAL;
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_redimensionar(pila_t *pila, size_t modificacion){
    void *datos_nuevos = realloc(pila->datos, modificacion * sizeof(void*));
    if (datos_nuevos == NULL){
        return false;
    }

    pila->datos = datos_nuevos;
    pila->capacidad = modificacion;
    return true;
}

int pila_cantidad(const pila_t *pila){
    return (int)pila->cantidad;
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        if (!pila_redimensionar(pila, pila->capacidad * REDIMENSION)){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad += 1;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    return pila_esta_vacia(pila) ? NULL : pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    if (pila->cantidad * CUADRUPLE <= pila->capacidad){
        if (!pila_redimensionar(pila, pila->capacidad / REDIMENSION)){
            return NULL;
        }
    }

    pila->cantidad -= 1;
    return pila->datos[pila->cantidad];
}
