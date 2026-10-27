#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define K 100
typedef struct expresion{
    char izq[K];
    char der[K];
}Gr;
typedef struct nodo{
    Gr expresion;
    struct nodo *sig;
    struct nodo *ant;
}*Lista2;

typedef struct
{
    Lista2 primero;
    Lista2 ultimo;
}ListaD;


void init_lista(Lista2 *a, Lista2 *b){
    *a=NULL;
    *b=NULL;
}

int crear_nodo(Lista2 *nuevo, Gr dato){
    int res = 0;
    *nuevo = (Lista2)malloc(sizeof(struct nodo));
    if(*nuevo){
        res = 1;
        (*nuevo)->expresion = dato;
        (*nuevo)->ant=NULL;
        (*nuevo)->sig=NULL;
    }
    return res;
}

int insercion_inicio(ListaD *lista, Gr dato){
    int res = 0;
    Lista2 aux, nuevo;
    res = crear_nodo(&nuevo,dato);
    if(res) {
            if(lista->primero == NULL){

                lista->primero = nuevo;
                lista->ultimo = nuevo;
            }
            else{
                lista->primero->ant = nuevo;
                nuevo->sig = lista->primero;
                lista->primero = nuevo;
            }
    }
    return res;
}

int insercion_final(ListaD *lista, Gr dato){
    int res = 0;
    Lista2 aux, nuevo;
    res = crear_nodo(&nuevo,dato);
    if(res){
        if(lista->ultimo == NULL){

                lista->primero = nuevo;
                lista->ultimo = nuevo;
        }
        else{
            lista->ultimo->sig = nuevo;
            nuevo->ant = lista->ultimo;
            lista->ultimo=nuevo;
        }

    }
    return res;
}

void eliminar_lista(ListaD *lista){

    Lista2 actual = lista->primero;
    Lista2 next = NULL;
    while(actual != NULL){
        next = actual->sig;
        free(actual);
        actual = next;

    }
}


void captura_nombre(char *nombre, int tam){
    printf("Ingresa el nombre del archivo (sin extension): ");
    fgets(nombre, tam, stdin);
    nombre[strcspn(nombre, "\n")]= '\0';
    strcat(nombre,".txt");
}
void mostrar_lista(ListaD *lista){
    Lista2 temporal = lista->primero;
    printf("\n--- Reglas cargadas en la lista ---\n");
    while(temporal != NULL){
        Gr expresion = temporal->expresion;
        printf("%s -> %s\n", expresion.izq, expresion.der); // \n para forzar la impresión
        temporal = temporal->sig;
    }
    printf("------------------------------------\n");
}


int mostrar_contenido(char *nombre, ListaD *lista){
    FILE *archivo = fopen(nombre, "r");
    if(archivo == NULL){
        printf("ERROR al abrir archivo\n");
        return 0;
    }
    
    char buff[256];
    while (fgets(buff, sizeof(buff), archivo) != NULL)
    {
        buff[strcspn(buff, "\n")] = '\0';

        
        if (strlen(buff) == 0) continue;

        char no_terminal[K] = "";
        char produccion[K] = "";

        int i = 0;
        
        while(buff[i] != '-' && buff[i] != '\0' && i < K - 1){
            no_terminal[i] = buff[i];
            i++;
        }
        no_terminal[i] = '\0';

        
        if (buff[i] == '-' && buff[i+1] == '>') {
            i += 2; 
        } else {
            continue;
        }

        
        int j = 0;
        while (buff[i] != '\0' && j < K - 1)
        {
            produccion[j] = buff[i];
            i++;
            j++;
        }
        produccion[j] = '\0';

        
        Gr dato;
        strcpy(dato.izq, no_terminal);
        strcpy(dato.der, produccion);
        insercion_final(lista, dato);
    }

    fclose(archivo); 
    mostrar_lista(lista);
    return 1;
}

void menu(){
    ListaD lista;
    init_lista(&lista.primero,&lista.ultimo);
    char nombre[50];
    int op = -1;
    int init = 0;
        do
    {
        op = -1;
        printf("-----------MENU------------\n");
        if(init == 0)printf("1) Iniciar\n ");
        else printf("1) Trabajar con otro archivo?: ");
        printf("\n2) Salir\n");
        scanf("%d", &op);

        if(op == 1){
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            captura_nombre(nombre, sizeof(nombre));

            if(!mostrar_contenido(nombre,&lista)){
                printf(" el archio no existe\n");
                
            }
            init = 1;
         
        }
        else if(op != 2){
            printf("Opcion no valida\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            op = -1;
        }
        
    }while(op!= 2); 
}

int main(){
    menu();
    return 0;
}