/////////////////////////////////////////////////////INCULDES////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// NUMERO RANDOM //////////////////////////////////////////////////////////

int NumeroRandom(){
	return 1 + rand()% ((1000+1)-1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////  BUSCAR ESPACIO ///////////////////////////////////////////////////////////

int BuscarEspacio(int i1,int f1){


}



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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// METODO DE LIMPIEZA /////////////////////////////////////////////////////////

void LimpiarChar(){
int i=0;
for(i;i<25;i+=1){
if(lexema[i]!='\0'){
 lexema[i]='\0';
}
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
            //printf("nueva cadena: %s", linea);
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
            //printf("RECONOCIO PALABRA \n");
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
            //printf("RECONOCIO NUMERO \n");
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
            //printf("reconocio signo menos \n");
        }else if(caracter=' '){
            Lexemas[posTL].token=21; //signo menos
            strcpy(Lexemas[posTL].texto,lexema);
            posTL+=1;
            posLex=0;
            estado=0;
            LimpiarChar();
            //printf("reconocio signo menos \n");
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
            //printf("RECONOCIO PARAMETRO OBLIGATORIO \n");
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
            //printf("RECONOCIO PARAMETRO OPCIONAL \n");
            strcpy(Lexemas[posTL].texto,lexema);
            Lexemas[posTL].token=DetToken(lexema);
            posTL+=1;
            posLex=0;
            estado = 0;
            LimpiarChar();
        break;

        case 9: // Estado 9 reconocimiento cadenas ""
        if(isalpha(caracter) || isdigit(caracter) || caracter == '/' || caracter == '.' || caracter == '_' || caracter == '-' || caracter ==' '){
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
            //printf("RECONOCIO CADENA \n");
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
            //printf("reconocio comentario \n");
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
        //printf("Ocurrio un error en el estado %i \n",estado_anterior);
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
    //printf("El numero es %i\n",atoi(SizeDisco));
    //printf("El numero2 es  %i\n",Dimension);
    posP+=1;
    SD=1;
    //printf("reconocio tk 6 \n");
    }else{
    printf("ER:El valor ingresado no es correcto \n");
    }
    }else{
    printf("ER:l tamanio ya fue definido anteriormente \n");
    }
}else if(Parametros[posP].token==7){ //valida intruccion path
    if(PD==0){
    posP+=1;
    if(Parametros[posP].token==15){
    strcpy(PathDisco,Parametros[posP].texto);
    //printf("path: %s \n",PathDisco);
    posP+=1;
    PD=1;
    //printf("reconocio tk 7 \n");
    }else{
    printf("ER: El path es incorrecoto \n");
    }
    }else{
    printf("ER:Ya habia ingresado el path \n");
    }
}else if(Parametros[posP].token==8){ //valida intruccion name
    if(ND==0){
    posP+=1;
    if(Parametros[posP].token==15){
    strcpy(NameDisco,Parametros[posP].texto);
        if(strstr(NameDisco,".dsk")!=0){
        //printf("Si contiene la extension .dsk\n");
        posP+=1;
        ND=1;
        //printf("reconocio tk 8 \n");
        }else{
        printf("ER: El nombre no contiene la extension .dsk\n");
        }
    }else{
    printf("ER: El nombre del disco es incorrecto \n");
    }
    }else{
    printf("ER: ya habia ingresado el nombre del disco \n");
    }
}else if(Parametros[posP].token==9){ //valida intruccion unidad
    if(UD==0){
    posP+=1;
    if(Parametros[posP].token==19){
    strcpy(UnitDisco,Parametros[posP].texto);
    posP+=1;
    UD=1;
    //printf("reconocio tk 9 \n");
    }else if(Parametros[posP].token==18){
    strcpy(UnitDisco,Parametros[posP].texto);
    posP+=1;
    UD=1;
    //printf("reconocio tk 9 \n");
    }else{
    printf("ER: La unidad no es valida \n");
    break;
    }
    }else{
    printf("ER: Ya habia ingresado la unidid para el disco \n");
    }
}else{
    printf("ER: Parametros invalidos para instruccion mkdisk \n");
    break;
}

}
if(SD==1 && PD==1 && ND==1){

//printf("parametros obligatorios correctos \n");
//existeC = system("ls /home/david/Escritorio/Discos");
//printf("el retorno es: %i \n", existeC);
//system("mkdir /home/david/Escritorio/Discos ");
//existeC = system("ls /home/david/Escritorio/Discos");
//printf("el retorno es: %i \n", existeC);

char ValPath[100];
char instruccion[10]="mkdir ";
strcat(instruccion,PathDisco);
//printf("concat: %s \n",instruccion);

//strcpy(ValPath,instruccion);
/////validacion path
struct stat buffer;
int ExisteCarpeta = 0;
ExisteCarpeta = stat(PathDisco, &buffer);
        if(ExisteCarpeta!=0){
        //printf("Se debera crear \n");
        system(instruccion);
        }else{
        //printf("Ya existe carpeta \n");
        }
/////////////////////
        char PathCompleto[127];
        strcpy(PathCompleto,PathDisco);
        strcat(PathCompleto,NameDisco);
        //printf("ruta disco: %s \n",PathCompleto);
FILE* arch = fopen(PathCompleto,"w+b");

    if(arch){
   //printf("si se creo \n");
    if(UD!=0){
      if(strcasecmp(UnitDisco,"k")==0){
        TamDisco = Dimension*K;
      }else if(strcasecmp(UnitDisco,"m")==0){
        TamDisco = Dimension*M;
      }
    }else{
        TamDisco = Dimension*M;
            }
    //printf("el tamonio del disco es: %i \n",TamDisco);
    char A = '\0';
    MBR MBRD;
    MBRD.mbr_disk_signature=NumeroRandom();
    MBRD.mbr_tamano=TamDisco;
    ////particiones/////
    Particion P1;
    Particion P2;
    Particion P3;
    Particion P4;
    P1.part_status='1';
    P2.part_status='1';
    P3.part_status='1';
    P4.part_status='0';
    P1.part_start=1070;
    P2.part_start=500;
    P3.part_start=2530;
    P4.part_start=1;
    P1.part_size=1000;
    P2.part_size=500;
    P3.part_size=1000;
    P4.part_size=0;
    MBRD.mbr_partition_1=P1;
    MBRD.mbr_partition_2=P2;
    MBRD.mbr_partition_3=P3;
    MBRD.mbr_partition_4=P4;
    /////cap. fecha y hora //////
  time_t t;
  struct tm *tm;
  char fechayhora[100];
  t=time(NULL);
  tm=localtime(&t);
  strftime(fechayhora, 100, "%d/%m/%y %H:%M:%S", tm);
    /////////////////////////////
    strcpy(MBRD.mbr_fecha_creacion,fechayhora);
    fseek(arch,0,SEEK_SET);
    fwrite(&MBRD,sizeof(MBR),1,arch);
    fseek(arch,TamDisco,SEEK_SET);
    fwrite(&A,sizeof(char),1,arch);
    fclose(arch);
    printf("Creo --> Disco: %s Size: %i Path: %s \n",NameDisco,TamDisco,PathDisco);
    }else{
    printf("ER: No se pudo crear el Disco \n");
    }
}else{
    if(SD==0){
    printf("ER: falta parametro size \n");
    }
    if(PD==0){
    printf("ER: falta parametro path \n");
    }
    if(ND==0){
    printf("ER: falta parametro name \n");
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
        //printf("reconocio instruccion path y direccion correctamente \n");
        }else{
        printf("Parametro de path incorrecto \n");
        break;
        }

    }else{
    printf("Parametros incorrectos \n");
    break;
    }

    if(PRMD!=0){
        //printf("path del disco:  %s \n",PathRMDisco);
        struct stat buffer2;
        int ExisteDSK = 0;
        ExisteDSK = stat(PathRMDisco, &buffer2);
        if(ExisteDSK!=0){
        printf("ER: No esxiste el disco \n");
        }else{
        //printf("si existe el disco \n");
        char Instruccion2[10]="rm -i ";
        strcat(Instruccion2,PathRMDisco);
        //printf("inst: %s \n",Instruccion2);
        system(Instruccion2);
        }
    }else{
    printf("ER: no se puede ejecutar rmdisk \n");
    }

}




}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////RETORNAR INICIO ///////////////////////////////////////////////////////////////


int EsEntre(int n, int i,int f){
    if(n >= i && n <= f){
    return 1;
    }else{
    return 0;
    }
}




int BuscarInicio(int TamDsk,int Tam, int iA,int fA, int iB,int fB,int iC, int fC){
int B1=0;
int B2=0;
int B3=0;
int B4=0;
int M1=0;
int M2=0;
int M3=0;
int M4=0;

int BDisp=0;
int Bpos=153;
int PosInicial=153;
      if(iA != 0){B1=1;}if(iB != 0){B2=1;}if(iC != 0){B3=1;}


      while(BDisp != Tam){

        if(Bpos > TamDsk){
        return 0;
        }

            if(B1==1 && B2==1 && B3==1){

                if(EsEntre(Bpos,iA,fA)==0){

                    if(EsEntre(Bpos,iB,fB)==0){

                        if(EsEntre(Bpos,iC,fC)==0){
                         BDisp+=1;
                         Bpos+=1;
                        }else{
                        PosInicial=fC+1;
                        Bpos=fC+1;
                        }

                    }else{
                    PosInicial=fB+1;
                    Bpos=fB+1;
                    }

                }else{
                PosInicial=fA+1;
                Bpos=fA+1;
                }

      }else if(B1==1 && B2==1 && B3==0){

      }else if(B1==1 && B2==0 && B3==1){

      }else if(B1==1 && B2==0 && B3==0){

      }else if(B1==0 && B2==1 && B3==1){

      }else if(B1==0 && B2==1 && B3==0){

      }else if(B1==0 && B2==0 && B3==1){

      }


      }


}

void OrdenarInicios(int inicios[], int fines[]){
    int Temp1;
    int Temp2;
    int i=0;
    int j=0;

    for(i;i<4;i++){

        for(j;j<3;j++){

            if(inicios[j]>inicios[j+1]){
                Temp1 =inicios[j];
                Temp2 =fines[j];
                inicios[j]=inicios[j+1];
                fines[j]=fines[j+1];
                inicios[j+1]=Temp1;
                fines[j+1]=Temp2;
            }
        }

    }
    int c=0;
    for(c;c<4;c++){
    printf("I: %i  F: %i \n",inicios[c],fines[c]);

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
char FitParticion='w'; //BF,FF,WF <WF>
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
/////// Constantes /////////////////
int K=1024;
int M=1048576;

while(posP<20){

    if(Parametros[posP].token==0){
    break;
    }

    if(Parametros[posP].token==6){ //size
        posP+=1;
        if(SP==0){
            if(Parametros[posP].token==14){
            if(atoi(Parametros[posP].texto)>0){
            SizeParticion=atoi(Parametros[posP].texto);
            SP=1;
            posP+=1;
            }else{
            printf("el tamano debe ser mayor a 0 \n");
            }
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
            UnitParticion='b';
            UP=1;
            posP+=1;
            }else if(Parametros[posP].token==19){
            UnitParticion='m';
            SizeParticion=SizeParticion*M;
            UP=1;
            posP+=1;
            }else if(Parametros[posP].token==18){
            UnitParticion='k';
            SizeParticion=SizeParticion*K;
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
            TypeParticion='p';
            TP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"e")==0){
            TypeParticion='e';
            TP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"l")==0){
            TypeParticion='l';
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
            //strcpy(FitParticion,Parametros[posP].texto);
            FitParticion='b';
            FP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"ff")==0){
            //strcpy(FitParticion,Parametros[posP].texto);
            FitParticion='f';
            FP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"wf")==0){
            //strcpy(FitParticion,Parametros[posP].texto);
            FitParticion='w';
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
            if(strcasecmp(Parametros[posP].texto,"fast")==0){
            strcpy(DeleteParticion,Parametros[posP].texto);
            DP=1;
            posP+=1;
            }else if(strcasecmp(Parametros[posP].texto,"full")==0){
            strcpy(DeleteParticion,Parametros[posP].texto);
            DP=1;
            posP+=1;
            }else{
            printf("tipo de eliminacion incorrecta \n");
            }

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

    //validacion de existencia

        struct stat buffer3;
        int ExisteD = 0;
        printf("%s \n",PathParticion);
        ExisteD = stat(PathParticion, &buffer3);
        if(ExisteD!=0){
        printf("ER: No esxiste el disco \n");
        }else{
        //Codico de Acciones

          if(SP!=0 && PP!=0 && NP!=0){
    printf("creara particion \n");

    FILE* archivo = fopen("/home/david/Escritorio/Ds/D1.dsk","r+b");
    if(archivo){
    MBR MBRTemp;
    fseek(archivo,0,SEEK_SET);
    fread(&MBRTemp,sizeof(MBR),1,archivo);
    //fclose(archivo);
    //////
    printf("tamano: %i \n",MBRTemp.mbr_tamano);
    printf("signature: %i \n",MBRTemp.mbr_disk_signature);
    printf("fecha: %s \n",MBRTemp.mbr_fecha_creacion);
    printf("mbr p2 stat: %c \n",MBRTemp.mbr_partition_2.part_status);
    /////

    ////pasar el tamanio a k
    if(UP==0){
    int tamTemp;
    tamTemp=SizeParticion*K;
    SizeParticion=tamTemp;
    printf("size cob : %i \n",SizeParticion);
    }
    printf("aqui 1 \n");
    //Inicio-Fin
    int I1=0;
    int F1=0;
    int I2=0;
    int F2=0;
    int I3=0;
    int F3=0;
    int I4=0;
    int F4=0;
    //obtener Inicios y Fines
    if(MBRTemp.mbr_partition_1.part_status=='1'){
     I1=MBRTemp.mbr_partition_1.part_start;
     F1=MBRTemp.mbr_partition_1.part_start+MBRTemp.mbr_partition_1.part_size;
    }
    if(MBRTemp.mbr_partition_2.part_status=='1'){
     I2=MBRTemp.mbr_partition_2.part_start;
     F2=MBRTemp.mbr_partition_2.part_start+MBRTemp.mbr_partition_2.part_size;
    }
    if(MBRTemp.mbr_partition_3.part_status=='1'){
     I3=MBRTemp.mbr_partition_3.part_start;
     F3=MBRTemp.mbr_partition_3.part_start+MBRTemp.mbr_partition_3.part_size;
    }
    if(MBRTemp.mbr_partition_4.part_status=='1'){
     I4=MBRTemp.mbr_partition_4.part_start;
     F4=MBRTemp.mbr_partition_4.part_start+MBRTemp.mbr_partition_4.part_size;
    }
    ///////////////////////////////////

    printf("aqui 2\n");

    if(TypeParticion=='p'){
    printf("aqui 2.5 \n");
    int TamEx;
    TamEx=sizeof(MBRTemp);
    printf("tam mbr %i \n",TamEx);
    if(SizeParticion <= (MBRTemp.mbr_tamano - TamEx)){

    //ninguna particion
    if(MBRTemp.mbr_partition_1.part_status=='1' && MBRTemp.mbr_partition_2.part_status=='1' && MBRTemp.mbr_partition_3.part_status=='1' && MBRTemp.mbr_partition_4.part_status=='1'){
        printf("ER: Maximo de particiones primarias permitidas \n");
    }else if(MBRTemp.mbr_partition_1.part_status=='0' && MBRTemp.mbr_partition_2.part_status=='0' && MBRTemp.mbr_partition_3.part_status=='0' && MBRTemp.mbr_partition_4.part_status=='0'){
        MBRTemp.mbr_partition_1.part_fit=FitParticion;
        strcpy(MBRTemp.mbr_partition_1.part_name,NameParticion);
        MBRTemp.mbr_partition_1.part_size=SizeParticion;
        MBRTemp.mbr_partition_1.part_start=153;
        MBRTemp.mbr_partition_1.part_status='1';
        MBRTemp.mbr_partition_1.part_type='p';
        fseek(archivo,0,SEEK_SET);
        fwrite(&MBRTemp,sizeof(MBR),1,archivo);
        fclose(archivo);
    }else{
        int I[3]={I1,I2,I3,7000};
        int F[3]={F1,F2,F3,8000};
        OrdenarInicios(I,F);

    }

    }else{
    printf("Espacio insuficiente para realizar particion \n");
    }

      //////
    }else if(TypeParticion=='e'){

    }else if(TypeParticion=='l'){

    }




    }else{
    printf("no se pudo modificar disco \n");
    }

    }else if(DP!=0 && PP!=0 && NP!=00){
    printf("eliminara particion \n");
    }else if(AP!=00 && UP!=0){
    printf("modificara particion \n");
    }else{
    printf("error en parametros  ejc\n");
    }


        ///////////////////
        }

/*
    FILE* archivo2 = fopen("/home/david/Escritorio/Ds/D1.dsk","r+b");
    if(archivo2){
    printf("si se abrio \n");
    MBR pruebaMBR;
    fseek(archivo2,0,SEEK_SET);
    fread(&pruebaMBR,sizeof(MBR),1,archivo2);
    fclose(archivo2);
    printf("fit P1: %c \n",pruebaMBR.mbr_partition_1.part_fit);
    printf("nombre P1: %s \n",pruebaMBR.mbr_partition_1.part_name);
    printf("size P1: %i \n",pruebaMBR.mbr_partition_1.part_size);
    printf("start P1: %i \n",pruebaMBR.mbr_partition_1.part_start);
    printf("status P1: %c \n",pruebaMBR.mbr_partition_1.part_status);
    printf("type P1: %c \n",pruebaMBR.mbr_partition_1.part_type);

    }else{
    printf("no se abrio \n");
    }
*/

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MONTAR DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MontarDisco(Lex Parametros[]){}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// DESMONTAR DISCO //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DesmontarDisco(Lex Parametros[]){}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////MAIN//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
int a=0;
while(a==0){
AT();
AS(Lexemas);
}
int i = 0;

return 0;

}
