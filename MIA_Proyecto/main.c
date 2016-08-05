/////////////////////////////////////////////////////INCULDES////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ESTRUCTURA ////////////////////////////////////////////////////////////////
typedef struct Lex{
    char texto[25];
    int token;
    int numeroID;
}Lex;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////FUNCION TOKEN //////////////////////////////////////////////////////////////

int DetToken(char l[]){

if(strcasecmp(l,"mkdisk")==0){
return 100;
}else if(strcasecmp(l,"rmdisk")==0){
return 1;
}else if(strcasecmp(l,"fdisk")==0){
return 2;
}else if(strcasecmp(l,"mount")==0){
return 3;
}else if(strcasecmp(l,"unmount")==0){
return 4;
}else if(strcasecmp(l,"exec")==0){
return 5;
}else if(strcasecmp(l,"-size::")==0){
return 6;
}else if(strcasecmp(l,"-path::")==0){
return 7;
}else if(strcasecmp(l,"-name::")==0){
return 8;
}else if(strcasecmp(l,"+unit::")==0){
return 9;
}else if(strcasecmp(l,"+type::")==0){
return 10;
}else if(strcasecmp(l,"+fit::")==0){
return 11;
}else if(strcasecmp(l,"+delete::")==0){
return 12;
}else if(strcasecmp(l,"+add::")==0){
return 13;
}else{
return 0;
}



}

/////////////////////////////////////////////////////ANALIZADORES///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Lex Lexemas[20];
char lexema[25];
int AT(){   // ANALIZADOR DE TEXTO
/*VARIABLES*/
char linea[100]; //almacena la linea
int tokens[20]; //almacena tokens obtenidos
//char lexemas[20]; // almacena los lexemas obtenidos
//Lex Lexemas[20];
//char lexema[25];
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
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter=linea[pos];
            estado = 2;
        }else if(caracter == '-'){ // es signo menos?
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter=linea[pos];
            estado = 3;
        }else if(caracter == '+'){ // es signo mas?
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter=linea[pos];
            estado = 6;
        }else if(caracter == '"'){ // son comillas?
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter=linea[pos];
            estado = 9;
        }else if(caracter == '\\'){
            char nextL[50];
            fgets(nextL,50,stdin);
            strcat(linea,nextL);
            printf("nueva cadena: %s", linea);
        }else if(caracter == '#'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
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
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=DetToken(lexema);
            posTL+=1;
            posLex=0;
            LimpiarChar();
            estado = 0;
        }
        break; // Fin reconocimiento palabras

        case 2: // Estado 2 para numeros
        if(isdigit(caracter)){
            //concatenar numer
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 2;
        }else{
            //almacenar token y lexema
            //aumentar contador TL
            printf("RECONOCIO NUMERO \n");
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=14;
            posTL+=1;
            posLex=0;
            LimpiarChar();
            estado = 0;
        }
        break; // Fin reconocimiento Numeros

        case 3: // Estado 3 para parametros Obligatoros
        if(isalpha(caracter)){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 3;
        }else if(caracter == ':'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 4;
        }else{
            estado_anterior = 3;
            estado = 15;
        }
        break; // Fin estado 3 parametro obligatorio

        case 33: // case especial para IDs con numero
        if(isdigit(caracter)){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 33;
        }else if(caracter == ':'){
            //agregar concatenacion de ID's
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 34;
        }else{
            estado_anterior=33;
            estado=15;
        }
        break;

        case 34: // case de cierre de reonocimiento de IDs
        if(caracter == ':'){
            lexema[posLex]=caracter;
           // Lexemas[posTL].texto=lexema;
            Lexemas[posTL].token=17;
            posTL+=1;
            pos+=1;
            posLex=0;
            caracter = linea[pos];
            estado=0;
            //
        }else{
            estado=15;
        }
        break;

        case 4: // Estado 4  parametros Obligatorios
        if(caracter == ':'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 5;
        }
        break; // Fin reconocimiento segundo :

        case 5: // Estado 5 cierre parametro obligatorio
            //almacenar token y lexema parametro obligatorio
            printf("RECONOCIO PARAMETRO OBLIGATORIO \n");
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=DetToken(lexema);
            posTL+=1;
            posLex=0;
            estado = 0;
            LimpiarChar();
        break; // Fin reconocimiento parametro Obligatorio;

        case 6: // Estado 6 para parametros Opcionales
        if(isalpha(caracter)){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 6;
        }else if(caracter == ':'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
            caracter = linea[pos];
            estado = 7;
        }else{
            estado_anterior = 6;
            estado = 15;
        }
        break; // fin reconocimieto letras parametro opcional

        case 7: // Estado 7 parametros Opcionales
        if(caracter == ':'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
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
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=DetToken(lexema);
            posTL+=1;
            posLex=0;
            estado = 0;
            LimpiarChar();
        break;

        case 9: // Estado 9 reconocimiento cadenas ""
        if(isalpha(caracter) || isdigit(caracter) || caracter == '/' || caracter == '.' || caracter == '_' || caracter == '-'){
            lexema[posLex]=caracter;
            posLex+=1;
            pos+=1;
            caracter = linea[pos];
            estado = 9;
        }else if(caracter == '"'){
            lexema[posLex]=caracter;
            pos+=1;
            posLex+=1;
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
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=15;
            posTL+=1;
            posLex=0;
            estado = 0;
            LimpiarChar();
        break; // Fin reconocimiento Cadenas (phath o nombre)

        case 11: // Estado aceptacion sigue en otra linea
            //agregar codigo de lectura de siguiente linea
        break; // Fin reconocimiento diagonal invertida

        case 12: //Estado 12 reconocimiento de Comentarios
        if(caracter != '\n'){
          lexema[posLex]=caracter;
          pos+=1;
          posLex+=1;
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
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=16;
            posTL+=1;
            posLex=0;
            LimpiarChar();
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

void AS(Lex Tabla[]){
if(Tabla[0].token==100){
 CrearDisco(Lexemas);
}else{
//manejar error
}
} //llave AS

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////METODO CREAR DISCO /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearDisco(Lex Parametros[]){
//char para alamcenar valores de los parametros del disco a crear
char SizeDisco[25];
char PathDisco[25];
char NameDisco[25];
char UnitDisco[25];
//ints para validad que parametros han venido 0=NO 1=SI
int SD=0;
int PD=0;
int ND=0;
int UD=0;
//int para posicion en la tabla de tokens/lexemas
int posP=1;

while(posP<20){
printf("esta en el while \n");
    if(Parametros[posP].token==0){
    break;
    }

if(Parametros[posP].token==6){ //valida instruccion size
    if(SD==0){
    posP+=1;
    strcpy(SizeDisco,Parametros[posP].texto);
    posP+=1;
    SD=1;
    printf("reconocio tk 6 \n");
    }else{
    printf("el tamanio ya fue definido anteriormente \n");
    }
}else if(Parametros[posP].token==7){ //calida intruccion path
    if(PD==0){
    posP+=1;
    strcpy(PathDisco,Parametros[posP].texto);
    posP+=1;
    PD=1;
    printf("reconocio tk 7 \n");
    }else{
    printf("ya habia ingresado el path \n");
    }
}else if(Parametros[posP].token==8){ //calida intruccion name
    if(ND==0){
    posP+=1;
    strcpy(NameDisco,Parametros[posP].texto);
    posP+=1;
    ND=1;
    printf("reconocio tk 8 \n");
    }else{
    printf("ya habia ingresado el nombre del disco \n");
    }
}else if(Parametros[posP].token==9){ //calida intruccion unidad
    if(UD==0){
    posP+=1;
    strcpy(UnitDisco,Parametros[posP].texto);
    posP+=1;
    UD=1;
    printf("reconocio tk 9 \n");
    }else{
    printf("ya habia ingresado la unidid para el disco \n");
    }
}else{
    printf("fin del analisis \n");
    break;
}

}
if(SD==1){
    if(PD==1){
        if(ND==1){
        printf("El tamanio del disco es: %s \n",SizeDisco);
        printf("El path del disco es: %s \n",PathDisco);
        printf("El nombre del disco es: %s \n",NameDisco);
        if(UD==1){
        printf("La unidad es: %s \n",UnitDisco);
        }else{
        printf("La unidad quedara por defecto \n");
            }
        }
    }
}

/*
validar falta de algun parametro obligatorio o parametro incorrecto en caso de path o extension
*/

}//fin creacion disco



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// METODO DE LIMPIEA /////////////////////////////////////////////////////////

void LimpiarChar(){
int i=0;
for(i;i<25;i+=1)
if(lexema[i]!='\0'){
 lexema[i]='\0';
}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////MAIN//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
int a=0;

AT();
AS(Lexemas);
int i = 0;
/*
for(i;i<4;i+=1){
printf("lexema: %s \n",Lexemas[i].texto);
printf("token: %i \n",Lexemas[i].token);
}
*/

return 0;

}
