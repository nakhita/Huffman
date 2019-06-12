#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void escribir(char cadena[]){
    FILE*archivo;
    int i=0;
    if(archivo=fopen("texto.txt","w")!=NULL){
        while(cadena[i]!='\0'){
            fputc(cadena[i],archivo);
            printf("Letra escrita %c\n",cadena[i]);
            i++;
        }
    }
    fclose(archivo);
}

void leer(){
    FILE*archivo;
    char cadena[100];
    int i=0;
    if(archivo=fopen("texto.txt","r")!=NULL){
        while(fgets(cadena,100,archivo)){
            while(cadena[i]!='\0'){
                printf("Caracter leido %c",cadena[i]);
            }
        }
        fclose(archivo);
    }
}

int main(){
    char cadena[100];
    puts("Ingrese la cadena a escribir");
    gets(cadena);
    escribir(cadena);
    puts("Imprimiendo contenido en el archivo");
    leer();
}
