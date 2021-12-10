#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct calc_token calc_token_t;
typedef bool (*operador_t)(calc_num *arr, calc_num *resultado);


/****************************************************
 *            FUNCIONES AUXILIARES
 * **************************************************/
//Vacia la pila.
void vaciar_pila(pilanum_t *pila_num){
    while (!pila_esta_vacia(pila_num)){
        calc_num resto;
        desapilar_num(pila_num, &resto);
    }
}

//Imprime mensaje de error.
void error(){
    printf("ERROR\n");
}

// Realiza la suma correspondiente.
bool suma(calc_num *arr, calc_num *resultado){
    *resultado = arr[1] + arr[0];
    return true;
}

// Realiza la resta correspondiente.
bool resta(calc_num *arr, calc_num *resultado){
    *resultado = arr[1] - arr[0];
    return true;
}

// Realiza la division correspondiente.
// En caso de que se intente dividir por cero devuelve false.
bool division(calc_num *arr, calc_num *resultado){
    if (arr[0] == 0) return false;
    *resultado = arr[1] / arr[0];
    return true;
}

// Realiza la multiplicaion correspondiente.
bool multiplicacion(calc_num *arr, calc_num *resultado){
    *resultado = arr[1] * arr[0];
    return true;
}

// Realiza la potenciacion correspondiente.
// En caso de que el exponente sea menor que 0 devuelve false.
bool potencia(calc_num *arr, calc_num *resultado){
    if (arr[0] < 0) return false;
    double aux = pow((double)arr[1], (double)arr[0]);
    *resultado = (calc_num) aux;
    return true;
}

// Realiza el logaritmo correspondiente.
// En caso de que la base del logaritmo sea menor que 2 devuelve false.
bool logaritmo(calc_num *arr, calc_num *resultado){
    if (arr[0] < 2) return false;
    *resultado = (calc_num)(log10((double)arr[1]) / log10((double)arr[0]));
    return true;
}

// Realiza la raiz correspondiente.
// En caso de que el numero sea menor que 0, devuelve false.
bool raiz(calc_num *arr, calc_num *resultado){
    if (arr[0] < 0) return false;
    double aux = sqrt((double)arr[0]);
    *resultado = (calc_num) aux;
    return true;
}

// Realiza el operador ternario correspondiente.
bool ternario(calc_num *arr, calc_num *resultado){
    *resultado = arr[2] ? arr[1] : arr[0];
    return true;
}

// Desapila los numeros que va a usar y en caso de que no se pueda desapilar devuelve false.
// Llama a la funcion correspondiente para aplicar el calculo.
bool calculo(pilanum_t *pila, calc_num operador, calc_num cant_operandos ,calc_num *resultado){
    calc_num nums[3];
    calc_num aux = 0;
    while (aux != cant_operandos){
        if(!desapilar_num(pila, &nums[aux])){
            return false;
        }
        aux++;
    }

    operador_t operadores[] = {suma, resta, multiplicacion, division, potencia, logaritmo, raiz, ternario};
    if (!operadores[operador](nums, resultado)){
        return false;
    }
    apilar_num(pila, *resultado);
    return true;
}

// Realiza las operaciones correspondientes en caso de que el operador sea un token.
// Devuelve true cuando se pudo realizar la operacion y false en caso contrario.
bool token_operador(pilanum_t *pila_num, char **vector, size_t i, calc_token_t *token){
    calc_num res;
    if (!calculo(pila_num, token->oper.op, token->oper.num_operandos, &res) || (!vector[i + 1] && pila_cantidad(pila_num) > 1)){
        vaciar_pila(pila_num);
        error();
        free(token);
        return false;
    }
    else if (!vector[i + 1]){
        vaciar_pila(pila_num);
        printf("%ld\n", res);
    }
    return true;
} 

// Procesa el vector recibido utilizando la estructura auxiliar pila.
void procesar_entrada(pilanum_t *pila_num, char **vector){
    for (int i = 0; vector[i]; i++){
        calc_token_t *token = malloc(sizeof(calc_token_t));
        if (!token){
            error();
            break;
        }
        
        if (!calc_parse(vector[i], token)){
            error();
            free(token);
            break;
        }

        if (token->type == TOK_NUM){
            apilar_num(pila_num, token->value);
        }
        else if (token->type == TOK_OPER){
            if (!token_operador(pila_num, vector, i, token)){
                break;
            }
        }
        if (!vector[i + 1] && pila_cantidad(pila_num) > 0){
            error();
        }
        free(token);
    }
}
/****************************************************
 *              PROGRAMA PRINCIPAL
 * **************************************************/
void calculadora_posfija(){
    char *linea = NULL;
    size_t capacidad = 0;

    while (getline(&linea, &capacidad, stdin) != -1){
        char **vector = dc_split(linea);
        if (!vector){
            error();
            continue;
        }

        pilanum_t *pila_num = pilanum_crear();
        if (!pila_num){
            error();
            free(vector);
            continue;
        }
        procesar_entrada(pila_num, vector);

        pilanum_destruir(pila_num);
        free_strv(vector);  
    }
    free(linea);
}

int main(){
    calculadora_posfija();
    return 0;
}

