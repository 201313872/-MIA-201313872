/////////////////////////////////////////////////////INCULDES////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ESTRUCTURAS ////////////////////////////////////////////////////////////////
typedef struct Lex{
    char texto[50];
    int token;
    int numeroID;
}Lex;

typedef struct Particion{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}Particion;

typedef struct MBR{
    int mbr_tamano;
    char mbr_fecha_creacion[32];
    int mbr_disk_signature;
    Particion mbr_partition_1;
    Particion mbr_partition_2;
    Particion mbr_partition_3;
    Particion mbr_partition_4;
}MBR;

typedef struct EBR{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

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
}else if(strcasecmp(l,"m")==0){
return 19;
}else if(strcasecmp(l,"k")==0){
return 18;
}else if(strcasecmp(l,"b")==0){
return 20;
}else{
return 872;
}



}

/////////////////////////////////////////////////////ANALIZADORES///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Lex Lexemas[20];
char lexema[50];
int AT(){   // ANALIZADOR DE TEXTO
/*VARIABLES*/
char linea[128]; //almacena la linea
int tokens[20]; //almacena tokens obtenidos
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
            //lexema[posLex]=caracter;
            pos+=1;
            //posLex+=1;
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
        }else if(isdigit(caracter)){
            Lexemas[posTL].token=21; //signo menos
            strcpy(Lexemas[posTL].texto,lexema);
            posTL+=1;
            posLex=0;
            estado=0;
            LimpiarChar();
            printf("reconocio signo menos \n");
        }else if(caracter=' '){
            Lexemas[posTL].token=21; //signo menos
            strcpy(Lexemas[posTL].texto,lexema);
            posTL+=1;
            posLex=0;
            estado=0;
            LimpiarChar();
            printf("reconocio signo menos \n");
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
            //lexema[posLex]=caracter;
            pos+=1;
            //posLex+=1;
            caracter = linea[pos];
            estado = 10;
        }else{
            estado_anterior = 9;
            estado = 15;
        }
        break; // fin reconocimiento cadena

        case 10: // Estado 10 fin cadena
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////METODO SELECTOR DE ACCION ///////////////////////////////////////////////////////

void AS(Lex Tabla[]){
if(Tabla[0].token==100){
 CrearDisco(Lexemas);
}else if(Tabla[0].token==1){
 RemoverDisco(Lexemas);
}else if(Tabla[0].token==2){
 ParticionarDisco(Lexemas);
}else if(Tabla[0].token==1){
 MontarDisco(Lexemas);
}else if(Tabla[0].token==1){
 DesmontarDisco(Lexemas);
}else{
//manejar error
}
} //llave AS

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////METODO CREAR DISCO /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CrearDisco(Lex Parametros[]){
//char para alamcenar valores de los parametros del disco a crear
char SizeDisco[50];
char PathDisco[50];
char NameDisco[50];
char UnitDisco[50];
///////////////////////
int TamDisco=0;
//ints para validad que parametros han venido 0=NO 1=SI
int SD=0;
int PD=0;
int ND=0;
int UD=0;
//int para posicion en la tabla de tokens/lexemas
int posP=1;
//int para constantes de K y M
int K =1024;
int M =1048576;
//int dimension
int Dimension=0;
/////////////////////////////////////////////////
while(posP<20){
//printf("esta en el while \n");
    if(Parametros[posP].token==0){
    break;
    }

if(Parametros[posP].token==6){ //valida instruccion size
    if(SD==0){
    posP+=1;
    if(Parametros[posP].token==14){
    strcpy(SizeDisco,Parametros[posP].texto);
    Dimension=atoi(SizeDisco);
    printf("El numero es %i\n",atoi(SizeDisco));
    printf("El numero2 es  %i\n",Dimension);
    posP+=1;
    SD=1;
    printf("reconocio tk 6 \n");
    }else{
    printf("El valor ingresado no es correcto \n");
    }
    }else{
    printf("el tamanio ya fue definido anteriormente \n");
    }
}else if(Parametros[posP].token==7){ //valida intruccion path
    if(PD==0){
    posP+=1;
    if(Parametros[posP].token==15){
    strcpy(PathDisco,Parametros[posP].texto);
    printf("path: %s \n",PathDisco);
    posP+=1;
    PD=1;
    printf("reconocio tk 7 \n");
    }else{
    printf("El path es incorrecoto \n");
    }
    }else{
    printf("ya habia ingresado el path \n");
    }
}else if(Parametros[posP].token==8){ //valida intruccion name
    if(ND==0){
    posP+=1;
    if(Parametros[posP].token==15){
    strcpy(NameDisco,Parametros[posP].texto);
        if(strstr(NameDisco,".dsk")!=0){
        printf("Si contiene la extension .dsk\n");
        posP+=1;
        ND=1;
        printf("reconocio tk 8 \n");
        }else{
        printf("No contiene la extension .dsk\n");
        }
    }else{
    printf("El nombre del disco es incorrecto \n");
    }
    }else{
    printf("ya habia ingresado el nombre del disco \n");
    }
}else if(Parametros[posP].token==9){ //valida intruccion unidad
    if(UD==0){
    posP+=1;
    if(Parametros[posP].token==19){
    strcpy(UnitDisco,Parametros[posP].texto);
    posP+=1;
    UD=1;
    printf("reconocio tk 9 \n");
    }else if(Parametros[posP].token==18){
    strcpy(UnitDisco,Parametros[posP].texto);
    posP+=1;
    UD=1;
    printf("reconocio tk 9 \n");
    }else{
    printf("La unidad no es valida \n");
    break;
    }
    }else{
    printf("ya habia ingresado la unidid para el disco \n");
    }
}else{
    printf("Parametros invalidos para instruccion mkdisk \n");
    break;
}

}
if(SD==1 && PD==1 && ND==1){

printf("parametros obligatorios correctos \n");
//existeC = system("ls /home/david/Escritorio/Discos");
//printf("el retorno es: %i \n", existeC);
//system("mkdir /home/david/Escritorio/Discos ");
//existeC = system("ls /home/david/Escritorio/Discos");
//printf("el retorno es: %i \n", existeC);

char ValPath[100];
char instruccion[10]="mkdir ";
strcat(instruccion,PathDisco);
printf("concat: %s \n",instruccion);

//strcpy(ValPath,instruccion);
/////validacion path
struct stat buffer;
int ExisteCarpeta = 0;
ExisteCarpeta = stat(PathDisco, &buffer);
        if(ExisteCarpeta!=0){
        printf("Se debera crear \n");
        system(instruccion);
        }else{
        printf("Ya existe carpeta \n");
        }
/////////////////////
        char PathCompleto[127];
        strcpy(PathCompleto,PathDisco);
        strcat(PathCompleto,NameDisco);
        printf("ruta disco: %s \n",PathCompleto);
FILE* arch = fopen(PathCompleto,"w+b");

    if(arch){
   printf("si se creo \n");
    if(UD!=0){
      if(strcasecmp(UnitDisco,"k")==0){
        TamDisco = Dimension*K;
      }else if(strcasecmp(UnitDisco,"m")==0){
        TamDisco = Dimension*M;
      }
    }else{
        TamDisco = Dimension*M;
            }
    printf("el tamonio del disco es: %i",TamDisco);
    char A = '\0';
    fseek(arch,TamDisco,SEEK_SET);
    fwrite(&A,sizeof(char),1,arch);
    fclose(arch);
    }else{
    printf("no se creo \n");
    }

}else{
    if(SD==0){
    printf("falta parametro size \n");
    }
    if(PD==0){
    printf("falta parametro path \n");
    }
    if(ND==0){
    printf("falta parametro name \n");
    }
}


}//fin creacion disco

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// REMOVER DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RemoverDisco(Lex Parametros[]){
char PathRMDisco[25];
int PRMD=0;
int posP=1;

while(posP<20){

    if(Parametros[posP].token==0){
    break;
    }

    if(Parametros[posP].token==7){
        posP+=1;
        if(Parametros[posP].token==15){
        strcpy(PathRMDisco,Parametros[posP].texto);
        PRMD=1;
        posP+=1;
        printf("reconocio instruccion path y direccion correctamente \n");
        }else{
        printf("Parametro de path incorrecto \n");
        break;
        }

    }else{
    printf("Parametros incorrectos \n");
    break;
    }

    if(PRMD!=0){
        printf("path del disco:  %s \n",PathRMDisco);
        struct stat buffer2;
        int ExisteDSK = 0;
        ExisteDSK = stat(PathRMDisco, &buffer2);
        if(ExisteDSK!=0){
        printf("no esxiste el disco \n");
        }else{
        printf("si existe el disco \n");
        char Instruccion2[10]="rm -i ";
        strcat(Instruccion2,PathRMDisco);
        printf("inst: %s \n",Instruccion2);
        system(Instruccion2);
        }
    }else{
    printf("no se puede ejecutar rmdisk \n");
    }

}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// PARTICIONAR DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParticionarDisco(Lex Parametros[]){
//almacenamiento datos de particion;
int SizeParticion; //>0
char UnitParticion='k'; // B,K,M <K>
char PathParticion[100];
char TypeParticion='p'; // P,E,L <P>
char NameParticion[100];
char FitParticion[100]="wf"; //BF,FF,WF <WF>
char DeleteParticion[100]; //Fast,Full
int AddParticion;
//banderas de instruccion
int SP=0;
int UP=0;
int PP=0;
int TP=0;
int NP=0;
int FP=0;
int DP=0;
int AP=0;
int AMP=0; //valida si es numero negativo
/////////////////////////
int posP=1;

while(posP<20){

    if(Parametros[posP].token==0){
    break;
    }

    if(Parametros[posP].token==6){ //size
        posP+=1;
        if(SP==0){
            if(Parametros[posP].token==14){
            strcpy(SizeParticion,Parametros[posP].texto);
            SP=1;
            posP+=1;
            }else{
            printf("tamanio incorrecto \n");
            }
        }else{
            printf("instruccion path definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==7){ //path
        posP+=1;
        if(PP==0){
            if(Parametros[posP].token==15){
            strcpy(PathParticion,Parametros[posP].texto);
            PP=1;
            posP+=1;
            }else{
            printf("path incorrecto \n");
            }
        }else{
            printf("instruccion path definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==8){ //name
        posP+=1;
        if(NP==0){
            if(Parametros[posP].token==15){
            strcpy(NameParticion,Parametros[posP].texto);
            NP=1;
            posP+=1;
            }else{
            printf("nombre incorrecto \n");
            }
        }else{
            printf("instruccion definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==9){ //unit
        posP+=1;
        if(UP==0){
            if(Parametros[posP].token==20){
            strcpy(UnitParticion,Parametros[posP].texto);
            UP=1;
            posP+=1;
            }else if(Parametros[posP].token==19){
            strcpy(UnitParticion,Parametros[posP].texto);
            UP=1;
            posP+=1;
            }else if(Parametros[posP].token==18){
            strcpy(UnitParticion,Parametros[posP].texto);
            UP=1;
            posP+=1;
            }else{
            printf("unidad incorrecta \n");
            }
        }else{
            printf("instruccion definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==10){ //type
        posP+=1;
        if(TP==0){
            if(Parametros[posP].token==872){
            if(strcasecmp(Parametros[posP].texto,"p")==0){
            strcpy(SizeParticion,Parametros[posP].texto);
            TP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"e")==0){
            strcpy(SizeParticion,Parametros[posP].texto);
            TP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"l")==0){
            strcpy(SizeParticion,Parametros[posP].texto);
            TP=1;
            posP+=1;
            }else{
            printf("tipo de particion incorrecta \n");
            }
            }else{
            printf("tipo de particion incorrecta \n");
            }
        }else{
            printf("instruccion tipo definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==11){ //fit
        posP+=1;
        if(FP==0){
            if(Parametros[posP].token==872){
            if(strcasecmp(Parametros[posP].texto,"bf")==0){
            strcpy(FitParticion,Parametros[posP].texto);
            FP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"ff")==0){
            strcpy(FitParticion,Parametros[posP].texto);
            FP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"wf")==0){
            strcpy(FitParticion,Parametros[posP].texto);
            FP=1;
            posP+=1;
            }else{
            printf("ajuste de particion incorrecta \n");
            }
            }else{
            printf("ajuste de particion incorrecta \n");
            }
        }else{
            printf("instruccion ajuste definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==12){ //delete
        posP+=1;
        if(DP==0){
            if(Parametros[posP].token==15){
            strcpy(NameParticion,Parametros[posP].texto);
            DP=1;
            posP+=1;
            }else{
            printf("nombre incorrecto \n");
            }
        }else{
            printf("instruccion definida previamente \n");
            break;
        }
    }else if(Parametros[posP].token==13){ //add
     posP+=1;
     if(AP==0){
     if(Parametros[posP].token==21){
        posP+=1;
        if(Parametros[posP].token==14){
        AddParticion =atoi(Parametros[posP].texto);
        AP=1;
        AMP=1;
        posP+=1;
        }else{
        printf("valor add incorrecto \n");
        }
    }else if(Parametros[posP].token==14){
        AddParticion =atoi(Parametros[posP].texto);
        AP=1;
        posP+=1;
     }else{
        printf("parametro de add incorrecto \n");
     }
     }else{
     printf("instruccion definida previamente \n");
     }
    }else{
     printf("parametro invalido@@@@ \n");
     break;
    }

}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MONTAR DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MontarDisco(Lex Parametros[]){}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// DESMONTAR DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DesmontarDisco(Lex Parametros[]){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// METODO DE LIMPIEZA /////////////////////////////////////////////////////////

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

return 0;

}
