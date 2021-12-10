#include "calc_helper.h"
#include "pila.h"
#include "cola.h"
#include "strutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct calc_token calc_token_t;
/* ******************************************************************
 *                        AUXILIARES
 * *****************************************************************/
// Desapila y en cola todos los operadores que hay entre el LPAREN y el RPAREN.
// En caso de que algun parseo no se pueda realizar devuelve false.
bool token_rparen(pila_t *pila, cola_t *cola){
    calc_token_t token_tope;
    char *tope = pila_ver_tope(pila);
    if (!calc_parse(tope, &token_tope)) return false;

    while (token_tope.type != TOK_LPAREN){
        char *viejo = pila_desapilar(pila);
        cola_encolar(cola, viejo);
        tope = pila_ver_tope(pila);
        if (!calc_parse(tope, &token_tope)) return false;
    }
    pila_desapilar(pila);
    return true;
}

// Mientras haya en el tope de la pila algun operador con mayor o igual precedencia, lo desapila
// y lo encola en la cola.
// Termina apilando el caracter actual.
// En caso de que algun parseo no se pueda realizar devuelve false.
bool token_oper(pila_t *pila, cola_t *cola, calc_token_t *token, char *caracter){
    if (!pila_esta_vacia(pila)){
        calc_token_t token_tope;
        char *tope = pila_ver_tope(pila);
        if (!calc_parse(tope, &token_tope)) return false;

        while (token_tope.oper.precedencia >= token->oper.precedencia && token->oper.precedencia != 3){
            char *viejo = pila_desapilar(pila);
            cola_encolar(cola, viejo);
            if (!pila_esta_vacia(pila)){
                tope = pila_ver_tope(pila);
                if (!calc_parse(tope, &token_tope)) return false;
            }
            else{
                break;
            }
        }
    }
    pila_apilar(pila, caracter);
    return true;
}

// Procesa los elementos que hay en las estrucutras auxiliares 
// y los imprime por salida estandar.
void proceso_final(pila_t *pila, cola_t *cola, char **vector){
    while (!pila_esta_vacia(pila)){
        char *restante = pila_desapilar(pila);
        cola_encolar(cola, restante);
    }

    while (!cola_esta_vacia(cola)){
        char *cadena = cola_desencolar(cola);
        printf("%s ", cadena);
    }
    printf("\n");

    free_strv(vector);
}

/* ******************************************************************
 *                          PROGRAMA
 * *****************************************************************/
void inf_to_post(){
    char *linea = NULL;
    size_t capacidad = 0;

    cola_t *cola = cola_crear();
    if (!cola) return;
    
    pila_t *pila = pila_crear();
    if (!pila){
        cola_destruir(cola, NULL);
        return;
    }

    while (getline(&linea, &capacidad, stdin) != EOF){
        char **vector = infix_split(linea);
        if (!vector) continue;
        
        for (size_t i = 0; vector[i]; i++){
            calc_token_t token;

            if (!calc_parse(vector[i], &token)) break;
            
            if (token.type == TOK_NUM){
                cola_encolar(cola, vector[i]);
            }
            else if (token.type == TOK_LPAREN){
                pila_apilar(pila, vector[i]);
            }
            else if (token.type == TOK_RPAREN){
                if (!token_rparen(pila, cola)) break;
            }
            else if (token.type == TOK_OPER){
                if (!token_oper(pila, cola, &token, vector[i])) break;
            }
        }

        proceso_final(pila, cola, vector);
    }
    pila_destruir(pila);
    cola_destruir(cola, NULL);
    free(linea);
}

int main(){
    inf_to_post();
    return 0;
}

