#include "strutil.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/****************************************************
 *            FUNCIONES AUXILIARES
 * **************************************************/
size_t cantidad_separadores(const char *cadena, char separador){
    size_t contador = 0;
    for (int i = 0; i < strlen(cadena); i++){
        if (cadena[i] == separador){
            contador++;
        }
    }
    return contador;
}

size_t largo_cadena(char **strv){
    size_t largo_total = 0;
    for (int i = 0; strv[i]; i++){
        // + 1 por el conector que va a concatenar la funcion.
        largo_total += strlen(strv[i]) + 1;
    }
    return largo_total;
}
/****************************************************
 *            FUNCIONES PRINCIPALES
 * **************************************************/
char *substr(const char *str, size_t n){
    char *resultado = calloc(n + 1, sizeof(char));
    if (!resultado){
        return NULL;
    }

    strncpy(resultado, str, n);

    return resultado;
}

char **split(const char *str, char sep){
    size_t cant_separadores = cantidad_separadores(str, sep);

    char **resultado = malloc(sizeof(char**) * (cant_separadores + 2));
    if (!resultado){
        return NULL;
    }

    size_t pos_arreglo = 0;
    size_t pos_str = 0;
    size_t cant_letra = 0;
    size_t largo = strlen(str);
    for (int i = 0; i < largo + 1; i++){
        if (str[i] == sep || i == largo){
            char *str_aux = substr(str + pos_str, cant_letra); 

            strncpy(str_aux, str + pos_str, cant_letra);
            str_aux[cant_letra] = '\0';

            resultado[pos_arreglo] = str_aux;
            pos_arreglo++;
            pos_str += cant_letra + 1;
            
            //caso estoy en la ultima pos del arreglo que lleva una cadena.
            cant_letra = (pos_arreglo == cant_separadores) ? 1 : 0;
            continue;
        }
        cant_letra++;
    }
    resultado[pos_arreglo] = NULL;
    return resultado;
}

char *join(char **strv, char sep){
    if (!strv){
        char *resultado = calloc(2, sizeof(char));
        resultado = '\0';
        return resultado;
    }
    
    size_t largo_total = largo_cadena(strv);

    char *resultado = calloc(largo_total + 1, sizeof(char));
    if (!resultado){
        return NULL;
    }

    char *puntero_a_final = resultado;
    for (int j = 0; strv[j]; j++){
        size_t hay_sep = j == 0 || sep == '\0' ? 0 : 1;
        
        puntero_a_final = stpcpy(puntero_a_final + hay_sep, strv[j]);
        puntero_a_final[0] = sep;
    }

    puntero_a_final[0] = '\0';
    return resultado;
}

void free_strv(char *strv[]){
    for (int i = 0; strv[i]; i++){
        free(strv[i]);
    }
    free(strv);
}

