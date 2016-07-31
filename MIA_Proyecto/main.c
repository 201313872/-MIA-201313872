/////////////////////////////////////////////////////INCULDES////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////CONCATENADOR////////////////////////////////////////////////////////////////
typedef struct Lex{
    char texto[25];
    int token;
}Lexemas, Lex;

/////////////////////////////////////////////////////ANALIZADOR///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int AT(){   // ANALIZADOR DE TEXTO
/*VARIABLES*/
char linea[100]; //almacena la linea
int tokens[20]; //almacena tokens obtenidos
//char lexemas[20]; // almacena los lexemas obtenidos
Lex Lexemas[20];
char lexema[25];
char lexemaCL[25];

char caracter; //almacena caracter temporal de la linea
int estado=0; // selector de estados
int estado_anterior = 0; //registro de estado de error
int pos = 0; //posicion acctual de la linea
int posLex = 0; // pisicion actual en el arreglo del lexema
int bandera = 0; // bandera de salida de while
int posTL = 0; //posicion en los arreglos de Tokens y Lexemas
fgets(linea,100,stdin);
caracter = linea[pos];
/*FIN VARIABLES*/

while(bandera == 0/*pos <=99*/){
    switch(estado){

        case 0: // Estado Inicial 0

        if(isalpha(caracter)){ // es letra?
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter=linea[pos];
            estado = 1;
        }else if(isdigit(caracter)){ // es numero?
            //concatenar numero
            pos+=1;
            caracter=linea[pos];
            estado = 2;
        }else if(caracter == '-'){ // es signo menos?
            //concatenar simbolo
            pos+=1;
            caracter=linea[pos];
            estado = 3;
        }else if(caracter == '+'){ // es signo mas?
            //concatenar simbolo +
            pos+=1;
            caracter=linea[pos];
            estado = 6;
        }else if(caracter == '"'){ // son comillas?
            //concatenar simbolo "
            pos+=1;
            caracter=linea[pos];
            estado = 9;
        }else if(caracter == '\\'){
            //concatenar siguiente liena a la linea
            //resetear contador
            //
        }else if(caracter == '#'){
            //concatenar simbolo $
            pos+=1;
            caracter=linea[pos];
            estado = 12;
        }else if(caracter == ' '){
            pos+=1;
            caracter = linea[pos];
            estado = 14;
        }else if(caracter == '\n'){
            bandera = 1;
        }else{
            estado_anterior = 0;
            estado = 15;
        }
        break; // fin del estado 0

        case 1: // Estado 1 para palabras
        if(isalpha(caracter)){
            //concatenar caracter
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 1;
        }else{
            printf("RECONOCIO PALABRA \n");
            printf("lexema: %s \n",lexema);
            posLex=0;
            if(strcasecmp(lexema,"holas")==0){
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=1;
            printf("lexema: %s \n",Lexemas[posTL].texto);
            printf("token: %i \n",Lexemas[posTL].token);
            posTL+=1;
            strcpy(lexema,"");
           // printf("es igual \n");
            }else if(strcasecmp(lexema,"como")==0){
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=1;
            printf("lexema: %s \n",Lexemas[posTL].texto);
            printf("token: %i \n",Lexemas[posTL].token);
            posTL+=1;
            strcpy(lexema,"");
           // printf("es igual \n");
            }
            estado = 0;
        }
        break; // Fin reconocimiento palabras

        case 2: // Estado 2 para numeros
        if(isdigit(caracter)){
            //concatenar numer
            pos+=1;
            caracter = linea[pos];
            estado = 2;
        }else{
            //almacenar token y lexema
            //aumentar contador TL
            printf("RECONOCIO NUMERO \n");
            estado = 0;
        }
        break; // Fin reconocimiento Numeros

        case 3: // Estado 3 para parametros Obligatoros
        if(isalpha(caracter)){
            //concatenar caracter
            pos+=1;
            caracter = linea[pos];
            estado = 3;
        }else if(caracter == ':'){
            pos+=1;
            caracter = linea[pos];
            estado = 4;
        }else{
            estado_anterior = 3;
            estado = 15;
        }
        break; // Fin estado 3 parametro obligatorio

        case 4: // Estado 4  parametros Obligatorios
        if(caracter == ':'){
            pos+=1;
            caracter = linea[pos];
            estado = 5;
        }
        break; // Fin reconocimiento segundo :

        case 5: // Estado 5 cierre parametro obligatorio
            //almacenar token y lexema parametro obligatorio
            printf("RECONOCIO PARAMETRO OBLIGATORIO \n");
            estado = 0;
        break; // Fin reconocimiento parametro Obligatorio;

        case 6: // Estado 6 para parametros Opcionales
        if(isalpha(caracter)){
            //concatenar letra
            pos+=1;
            caracter = linea[pos];
            estado = 6;
        }else if(caracter == ':'){
            pos+=1;
            caracter = linea[pos];
            estado = 7;
        }else{
            estado_anterior = 6;
            estado = 15;
        }
        break; // fin reconocimieto letras parametro opcional

        case 7: // Estado 7 parametros Opcionales
        if(caracter == ':'){
            pos+=1;
            caracter = linea[pos];
            estado = 8;
        }else{
            estado_anterior = 7;
            estado = 15;
        }
        break; // fin de reconocimiento : parametro Opcional

        case 8: // Estado 8 parametros Opcionales
            //almacenar token y lexema parametro opcional
            printf("RECONOCIO PARAMETRO OPCIONAL \n");
            estado = 0;
        break;

        case 9: // Estado 9 reconocimiento cadenas ""
        if(isalpha(caracter) || isdigit(caracter) || caracter == '/' || caracter == '.' || caracter == '_' || caracter == '-'){
            //concatenar caracter
            pos+=1;
            caracter = linea[pos];
            estado = 9;
        }else if(caracter == '"'){
            pos+=1;
            caracter = linea[pos];
            estado = 10;
        }else{
            estado_anterior = 9;
            estado = 15;
        }
        break; // fin reconocimiento cadena

        case 10: // Estado 10 fin cadena
            //guardar token y lexema
            //aumentar contador TL
            printf("RECONOCIO CADENA \n");
            estado = 0;
        break; // Fin reconocimiento Cadenas (phath o nombre)

        case 11: // Estado aceptacion sigue en otra linea
            //agregar codigo de lectura de siguiente linea
        break; // Fin reconocimiento diagonal invertida

        case 12: //Estado 12 reconocimiento de Comentarios
        if(caracter != '\n'){
          //concatenar comentario
          pos+=1;
          caracter = linea[pos];
          estado = 12;
        }else{
            estado = 13;
        }
        break; // Fin reconicimiento Comentario

        case 13: // Estado 13 para Comentarios
            //almacenar Token y Lexema
            //aumnetar variable TL
            /* Se puede agregar metodo de ejecucion de parametros*/
            bandera = 1;
            printf("reconocio comentario \n");
        break; // Fin de reconocimiento Comentarios

        case 14:  //estado de eliminacion de espacios y posibles tabs
            if(caracter == ' '){
            pos+=1;
            caracter = linea[pos];
            estado = 14;
            }else if(caracter == '\n'){
             // fin de linea
             bandera = 1;
            }else{
            printf("regreso a estado 0 \n");
            estado = 0;
            }
            break;

        case 15: //Para detectar posibles errores
        printf("Ocurrio un error en el estado %i \n",estado_anterior);
        bandera = 1;
        system("pause");
        break;
        break;

}//fin switch
} // fin while
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////MAIN//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
int a=0;
AT();

return 0;

}
