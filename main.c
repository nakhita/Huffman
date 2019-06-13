
/*Codigo de Huffman*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>

/* Esta variable puede ser evitada calculando la altura del arbol*/
#define MAX_TREE_HT 100 
#define cant 500


/*Struct para letra con su codificación
 *Con esto armaremos una Lista simplemente enlazada
 *que nos ayudara a decodificar 
 * es decir que será como nuestro diccionario
 */
struct Code{
    char letra;
    char codigo[256];
    struct Code *siguiente;
};

struct Code *inicio=0;

/*Un nodo del arbol*/

struct MinHeapNode {     
    char data; /*Uno de los caracteres de entrada*/
    unsigned freq; /*Frecuencia del caracter*/      
    struct MinHeapNode *left, *right; /*Nodos hijo izquierdo y derecho*/
};

/*Struct Min-Heap */
/*Coleccion de nodos armados en base Min-Heap*/
/*Min-Heap: Ordena los nodos de manera que el nodo padre siempre sea el menor de su rama*/
struct MinHeap {     
    unsigned size;              /*Tamaño actual del Min-Heap*/
    unsigned capacity;          /*Capacidad del Min-Heap*/ 
    struct MinHeapNode** array; /*Vector de punteros de Min-Heap*/
};
/*Un funcion que crea un nuevo nodo con un caracter y una frecuencia dadas*/
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*) malloc(sizeof (struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

/*Una funcion que crea un Min-Heap con una capacidad dada*/

struct MinHeap* createMinHeap(unsigned capacity)
 {
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof (struct MinHeap));
    minHeap->size = 0; // El tamaño es 0
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(minHeap->capacity * sizeof (struct MinHeapNode*));
    return minHeap;
}

/*Una funcion para intercambiar dos nodos*/

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
 {

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

/*Funcion en donde hace el Min-Heap*/

void minHeapify(struct MinHeap* minHeap, int idx)
 {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]-> freq < minHeap->array[smallest]->freq){
        smallest = left;
    }
    if (right < minHeap->size && minHeap->array[right]-> freq < minHeap->array[smallest]->freq){
        smallest = right;
    }
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

/*La funcion que revisa si el tamaño del Min-Heap es uno o no*/

int isSizeOne(struct MinHeap* minHeap) {

    return (minHeap->size == 1);
}

/*Una funcion estandar que extrae el valor minimo del nodo del Min-Heap*/

struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

/*La funcion que inserta el nuevo nodo del Min-Heap*/

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

/*Una funcion que construye el Min-Heap*/
void buildMinHeap(struct MinHeap* minHeap){
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i){
        minHeapify(minHeap, i);
    }
}



/*Funcion que revisa si este nodo es una hoja*/

int isLeaf(struct MinHeapNode* root){
    return !(root->left) && !(root->right);
}

/*Crea un Min-Heap de capacidad igual a la variable size
 *e inserta todos los caractes de data[]
 *en el Min-Heap. Inicialmente el tamaño del Min-Heap
 * es igual a la capacidad */

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size){
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i){
        minHeap->array[i] = newNode(data[i], freq[i]);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

/*La funcion principal que crea el arbol de Huffman*/

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size){ 
    struct MinHeapNode *left, *right, *top;
    /*Paso 1: Crea un Min-Heap de capacidad
     *igual a la variable size. Inicialmente hay modos igual al tamaño
     */
    
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    /*Itera mientra el tamaño del heap no sea 1*/
    while (!isSizeOne(minHeap)) { 
        /*Paso 2: Extrae las dos minimas frecuencias
         *del Min-Heap
         */
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        /*Paso 3: Crea un nuevo nodo interno
         *con frecuencia igual a la suma de los dos
         * nodos extraidos del Min-Heap
         * Toma los dos nodos extraidos como hijos de
         * izquierda y derecha de este nuevo nodo
         * Agrega este nodo al Min-Heap
         * '$' es un valor especial para nodos internos, no usado!
         */
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    /*Paso 4: El nodo restante es el nodo raiz y
     *el arbol esta completo
     */

    return extractMin(minHeap);
}
/*La funcion que imprime un vector de tamaño n*/

void printArr(char arr[], int n) { 
    int i;

    for (i = 0; i < n; ++i)
        printf("%c", arr[i]);

    printf("\n");
}

/*Crea el diccionario*/
void CrearDiccionario(struct Code **p,char letra,char arr[], int size){
    struct Code *auxiliar;
    struct Code *v;
    struct Code *actual;
    struct Code *anterior;
    int i;
    int bandera=1;
    
    actual= *p;
    
    if(actual){
        /*Busca si existe la letra*/
        while(bandera&&actual){
            if(actual->letra == letra){
                bandera=0;
            }
            else{
                actual= actual->siguiente;
            }
        }
        /*Si no existe lo crea y le da el codigo*/
        if(bandera){
            auxiliar= (struct Code*)malloc(sizeof(struct Code));
            auxiliar->letra=letra;
            for(i=0;i<size;i++){
                auxiliar->codigo[i]=arr[i];
                printf("%c",auxiliar->codigo[i]);
            }
            auxiliar->codigo[i+1]='\0';
            auxiliar->siguiente=0;
            actual=*p;
            bandera=1;
            /*Acomoda nodo*/
            while(bandera&&actual){
                int actualetra=actual->letra;
                int auxletra=letra;
                if(auxletra < actualetra){
                    bandera=0;
                }
                else{
                    anterior=actual;
                    actual= actual->siguiente;
                }
            }
            if(actual==*p){
                auxiliar->siguiente=actual;
                *p=auxiliar;
            }
            else{
                anterior->siguiente=auxiliar;
                auxiliar->siguiente=actual;
            }
        }
        /*No se debe contemplar el caso de que no exista
         *Ya que siempre al crearlo le estamos
         * agregando el codigo
         */
    }
    else{
        auxiliar= (struct Code*)malloc(sizeof(struct Code));
        auxiliar->letra=letra;
        for(i=0;i<size;i++){
            auxiliar->codigo[i]=arr[i];
            printf("%c",auxiliar->codigo[i]);
        }
        auxiliar->codigo[i+1]='\0';
        auxiliar->siguiente=0;
        *p= auxiliar;
    }
}

/*Imprime el codigo de Huffman de la raiz del arbol
 *Esto usa arr[] para almacenar los codigos
 */

void printCodes(struct MinHeapNode* root, char arr[], int top)
 {
    //printf("->primer PrintCodes %p: <----------\n",****p);
    /*Asigna 0 a la izquiera al final y recorre*/
    
    
    if (root->left) {
        arr[top] = '0';
        printCodes(root->left, arr, top + 1);
    }
    /*Asigna 1 a la derecha al final y recorre*/

    if (root->right) {
        arr[top] = '1';
        printCodes(root->right, arr, top + 1);
    }
  
    /*Si este tiene un nodo 'Hoja', entonces
     *este contiene uno de los caracteres de entrada
     *imprime el caracter y
     * su codigo de arr[]
     */
    
    if (isLeaf(root)) {
        printf("\n %c: ", root->data);
        printArr(arr, top);   
        CrearDiccionario(&inicio,root->data,arr,top);     
    }
    
}



/*La funcion principal que contruye un
 * arbol de Huffman e imprime los codigos
 * mientras atraviesa el arbol de Huffman construido
 */

void HuffmanCodes(char data[], int freq[], int size)
 {
    /*El contrstuctor del arbol de Huffman*/
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    /*Imprime el codigo de Huffman usando
     *el arbol de Huffman construido arriba
     */
    
    char arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

/*Funcion donde convierte 1 byte encriptado
 *en base a Huffman en un caracter nuevo
 */
char Letra(char byte[]){
    int aux=1,i=0,nletra=0;
    char caracter;
    printf("\nBYTE\n");
    printf("\ni = %i",i);
    for(i=7;i>=0;--i){
        printf("\nI: %i",i);
        printf("\t %c",byte[i]);
        if(byte[i]=='1'){
            nletra=nletra+aux;
        }
        printf("\t letra: %i",nletra);
        aux=aux*2;
    }
    return nletra;
}

/*Funcion donde guarda en un FILE
 *El nuevo texto encriptado
 */
void escribir(char *cadena[]){
    FILE*archivo;
    int i=0;
    archivo=fopen("texto.txt","w");
    if(archivo){
        while(cadena[i]!='\0'){
            fputc(cadena[i],archivo);
            printf("LEtra num: %i",cadena[i]);
            printf("\nLetra escrita %c\n",*cadena[i]);
            i++;
        }
        printf("\nEl texto encriptado se guardo exitosamente en 'texto.txt'");
    }
    else{
        printf("\nERRROR NO PUDO GUARDARSE EL TEXTO ENCRIPTADO");
    }
    fclose(archivo);
}

/*Funcion donde recibe texto en "Codigo binario" 
 * Este parsea en 8 "bits" el texto y recibe el
 * nuevo caracter
 */
void Parseo(char texto[], int puntero){
    int tam,aux,condicion,agregar,i,opc;
    int contador=0,bandera=1;
    char letraux;
    aux=puntero%8;
    if(aux){
        agregar=8-aux;
        aux=agregar+puntero;
    }
    tam=aux/8;
    char temp[8],v[tam+1];
    for(i=0;i<puntero;i++){
        condicion=i%8;
        aux=condicion;
        if((!condicion) && (i!=0)){
            letraux=Letra(temp);
            printf("\n-->LETRA %c",letraux);
            v[contador]=letraux;
            contador++;
        }
 
        temp[aux]=texto[i];
    }
    printf("\n-->LETRA %c",letraux);
    if(agregar){ 
        for(i=0;i<=agregar;i++){
            condicion=i%8;
            aux=puntero%8;
            aux=8-aux;
            if(!condicion){
                temp[aux]=texto[i];
                v[contador]=Letra(temp);
                contador++;
            }
            temp[aux]='0';
        }
    }
    v[contador]='\0';
    int canti=strlen(v);
    for(int j=0;j<canti;j++){
        printf("\nLetra [%i] : %i",j,v[j]);
    }
    //escribir(v);
    
}

/*Funcion vuelve el texto a binario */
void LetraBin(struct Code **p,char texto[],int tamanio){
    int tam=tamanio*8;
    struct Code*actual;
    struct Code*auxiliar;
    char TextoCod[tam];
    int i=0,j,bandera=1,puntero=0;
    while(texto[i]!='\0'){
        actual=*p;
        while(bandera && actual){
            if(actual->letra==texto[i]){
                bandera=0;
            }
            else{
                actual= actual->siguiente;
            }
        }
        if(!bandera){
            int n= strlen(actual->codigo);
            int tope=n+puntero;
            int indice;
            for(j=puntero;j<tope;j++){
                indice=j-puntero;
                TextoCod[j]=actual->codigo[indice];
            }
            puntero=j;
        }
        else{
            printf("\n ERROR ERROR ERROR ERROR ERROR");
        }
        printf("\ni: %i",i);
        i++;
        bandera=1;
    }
    TextoCod[puntero]='\n';
    printf("\n\t***TEXTO INGRESADO***\n\n");
    for(i=0;i<tamanio;i++){
        printf("%c",texto[i]);
    }
    printf("\n\n\t***TEXTO ENCRIPTADO***\n\n");
    for(i=0;i<puntero;i++){
        printf("%c",TextoCod[i]);
    }
    printf("\n \n \n");
    
    /*Funcion para que cada caracter de 1 y 0 se vuelva 
     * el nuevo caracter encriptado*/
    Parseo(TextoCod,puntero);
    
    /*Funcion para preguntar si desea guardarlo en archivo o no*/
}



/*Funcion para obtener las letras*/
void ObtCantidad(char texto[], int tamanio){
    int freq[cant],i ,j ,bandera,size;
    char arr[cant];
    size=0;
    for(i=0; i< tamanio; i++){
        bandera=1;
        for(j=0; j< size; j++){
            if(texto[i]==arr[j]){
                freq[j]= freq[j] + 1;
                j=size;
                bandera=0;
            }
        }
        if(bandera){
            arr[j]=texto[i];
            freq[j]=1;
            size++;   
        }
    }
    for(j=0; j< size; j++){
        printf("\n Letra: %c Freq: %i\n",arr[j],freq[j]);
    }
    HuffmanCodes(arr, freq, size);

    LetraBin(&inicio,texto,tamanio);
    
}

/*Funcion que pide el texto*/
void Texto(){
    char texto[cant];
    int size, * tam;
    printf("\nIngrese el texto a encriptar: \n");
    while(getchar()!='\n');
    gets(texto);
    size = strlen(texto)/ sizeof(char);
    printf("\nTamaño: %i <---------\n",size);
    /*Obtiene la frecuencia de los caracteres para
     *armar el arbol binario
     */
    ObtCantidad(texto,size);
    
    
}


/*Programa conductor que testea las funciones de arriba*/

int main() { 
    int opc;
    do{
        
        printf("\n\t****BIENVENIDO AL MENU****\n");
        printf("\nElija una opcion:\n");
        printf("\n1.Encriptar\n");
        printf("\n2.Desencriptar\n");
        printf("\n3.Guardar en un Archivo el texto encriptado\n");
        printf("\n0.Salir\n");
        printf("\nOpcion: ");
        scanf("%i",&opc);
        switch(opc){
            case 1:
                printf("\nInicio 1: %p",inicio);
                inicio=0;
                printf("\nInicio 2: %p",inicio);
                Texto();
                break;
            case 2:;break;
            case 3:;break;
            case 0:printf("\nCerrando programa\n");sleep(3);break;
            default:printf("\nOpcion ingresada incorrecta, vuelva a intentar\n");break;
        }
    }while(opc!=0);
    
    return 0;
} 