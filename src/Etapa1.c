#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void captura_nombre(char *nombre, int tam){
    printf("Ingresa el nombre del archivo (sin extension): ");
    fgets(nombre, tam, stdin);
    nombre[strcspn(nombre, "\n")]= '\0';
    strcat(nombre,".txt");
}

int mostrar_contenido(char *nombre){
    FILE *archivo = fopen(nombre,"r");
    if(archivo == NULL){
        printf("ERROR al abrir archivo");
        return 0;
    }
    char buff[256];
    while (fgets(buff, sizeof(buff), archivo) != NULL)
    {
        printf("%s", buff);
    }
    return 1;
}

void menu(){
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

            if(!mostrar_contenido(nombre)){
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