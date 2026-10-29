#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define K 1000
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

void eliminar_simplificar_tokens(char *token){
    int i = 0, j = 0;
    while (token[i] != '\0')
    {
        
        if(token[i]!= '|' && token[i] != ' '){
            token[j] = token[i];
            j++;
        }
        
        i++;
    }
    token[j] = '\0';
   
}


int transformar_expresion(Gr *produccion){
    char nu[K] = "";
    char token[K];
    int i = 0;
    
    char der_copia[K];
    strcpy(der_copia, produccion->der);

    char *token_ptr = strtok(der_copia, "|");
    
    int primera = 1;

    while (token_ptr != NULL) {
        while (*token_ptr == ' ') token_ptr++;
        int len = strlen(token_ptr);
        while (len > 0 && token_ptr[len - 1] == ' ') {
            token_ptr[len - 1] = '\0';
            len--;
        }

        char *p = strstr(token_ptr, produccion->izq);
        if (p != NULL) {
            char aux[K];
            char aux2[K];
            int j = 0;
            for (int i = 0; i < strlen(token_ptr); i++)
        {
            
            if(token_ptr[i] != produccion->izq[0]){
                //printf(" %c ", token_ptr[i]);    
                aux2[j] = token_ptr[i];
                j++;
            }
        }
    
            sprintf(aux, "{%s}", aux2);
            
            if (!primera) strcat(nu, " | ");
            strcat(nu, aux);
        } else {
            if (!primera) strcat(nu, " | ");
            strcat(nu, token_ptr);
        }



        primera = 0;
        token_ptr = strtok(NULL, "|");
    }
    eliminar_simplificar_tokens(nu);
    memset(produccion->der, 0, K);
    strcpy(produccion->der, nu);
    
    return 0;
}

int transformar_lista(ListaD *lista){
    if (lista == NULL) return -1;
    Lista2 aux = lista->primero;
    while (aux != NULL)
    {
        transformar_expresion(&aux->expresion);
        aux = aux->sig;
    }
    Lista2 aux_regreso = lista->ultimo;
    while(aux_regreso != NULL){
        transformar_expresion(&aux_regreso->expresion);
        aux_regreso = aux_regreso->ant;
    }
    return 0;
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
        printf("%s ->  %s\n", expresion.izq, expresion.der); // \n para forzar la impresión
        temporal = temporal->sig;
    }
    printf("------------------------------------\n");
}


int simplificar_expresion(ListaD *lista){

    if(lista == NULL || lista->primero==NULL) return -1;
    Lista2 aux=lista->primero;
    while (aux!=NULL)
    {
        Lista2 aux2 = aux->sig;    
        while (aux2!=NULL)
        {
             if (strcmp(aux->expresion.izq, aux2->expresion.izq) == 0) {
                
                // 1. Concatenamos el separador " | " y el nuevo lado derecho
                // Validamos que no exceda el tamaño K para máxima seguridad
                if (strlen(aux->expresion.der) + strlen(aux2->expresion.der) + 3 < K) {
                    strcat(aux->expresion.der, " | ");
                    strcat(aux->expresion.der, aux2->expresion.der);
                }
                Lista2 borrar;
                borrar = aux2;

                aux2=aux2->sig;
                if(borrar->sig != NULL) 
                {borrar->sig->ant = borrar->ant;}
                else{
                    lista->ultimo = borrar->ant;
                }
                if(borrar->ant != NULL) {borrar->ant->sig = borrar->sig;}
                else{
                    lista->primero =borrar->sig;
                }
                free(borrar);
            }
            else{    aux2= aux2->sig;}
        }
        aux = aux->sig;
        
        
    }
    
    
    return 0;
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
        buff[strcspn(buff, "\r\n")] = '\0';

        
        if (strlen(buff) == 0) continue;

        char no_terminal[K] = "";
        char produccion[K] = "";

        int i = 0;
        while(buff[i]== ' ' || buff[i] == '\t')i++;
        int k = 0;
        while(buff[i] != '-' && buff[i] != '\0'&& buff[i] != ' ' && buff[i] != '\t'  &&i < K - 1){
            no_terminal[k++] = buff[i++];
        }
        no_terminal[i] = '\0';

        
        if (buff[i] == '-' && buff[i+1] == '>') {
            i += 2; 
        } else {
            continue;
        }

        while(buff[i] == ' ' || buff[i] == '\t') i ++;

        int j = 0;
        while (buff[i] != '\0' && j < K - 1)
        {
            produccion[j] = buff[i];
            i++;
            j++;
        }
        produccion[j] = '\0';

        if(strlen(no_terminal) > 0) {
            Gr dato;
            strcpy(dato.izq, no_terminal);
            strcpy(dato.der, produccion);
            insercion_final(lista, dato);
        }
    }

    fclose(archivo); 
    mostrar_lista(lista);
    simplificar_expresion(lista);
    mostrar_lista(lista);
    transformar_lista(lista);
    mostrar_lista(lista);
    return 1;
}

void menu(){
    ListaD lista;
    
    char nombre[50];
    int op = -1;
    int init = 0;
        do
    {
        init_lista(&lista.primero,&lista.ultimo);
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
            eliminar_lista(&lista);
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