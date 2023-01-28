#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NAME_COMPARE "name"
#define SURNAME_COMPARE "surname"
#define GROUP_COMPARE "group"


typedef struct ElectronicItem {
    char name[50];
    char brand[20];
    char model[20];
    char price[5];
    char stock[5];
}ElectronicItem;

typedef struct node
{
    struct node *next;
    struct node *prev; 
    ElectronicItem data;
}node;

void free_list(node *lista) {
    if (lista == NULL) {
        return;
    }
    free_list(lista->next);
    free(lista);
}

int add(node **lista, ElectronicItem dane)
{   
    if (*lista == NULL) {
        *lista = malloc(sizeof(node));
        if( lista==NULL)
        {
            return 1;
        }
        (*lista)->data = dane;
        (*lista)->prev = NULL;
        (*lista)->next = NULL;
    } else {
        node *new = malloc(sizeof(node));
        if (new == NULL) 
        {
        free_list(*lista);
        return 1;
        }
        new->data = dane;
        new->prev = *lista;
        new->next = NULL;
        (*lista)->next = new;
        *lista = new;
    }
}

node *get_first(node *lista) 
{
  while (lista->prev != NULL && lista!=NULL) {
    lista = lista->prev;
  }
  return lista;
}

void clear_list(node** lista) 
{
node* current = *lista;
    while (current != NULL && lista!=NULL) 
    {
        node* next = current->prev;
        free(current);
        current = next;
    }

  *lista = NULL;
}

void free_on_error(node **lista) {
  free_list(*lista);
  *lista = NULL;
}

void read_from_csv(node **lista)
{
    FILE *file = fopen("ece_item.csv", "r");
    if(file == NULL)
    {
        printf("Error: Could not open file\n");
        free_on_error(lista);
        return;
    }

    char line[1024];
    while (fgets(line, 1024, file))
    {
        // Split the line into fields
        char *field = strtok(line, ";");
        int i = 0;
        ElectronicItem dane;
        while (field)
        {
            // Convert the field from a string to the appropriate data type
            // and store it in the 'dane' struct
            if (i == 0) strcpy(dane.name, field);
            else if (i == 1) strcpy(dane.brand, field);
            else if (i == 2) strcpy(dane.model, field);
            else if (i == 3) strcpy(dane.price, field);
            else if (i == 4) strcpy(dane.stock, field);

            field = strtok(NULL, ";");
            i++;
        }

        // Add the 'dane' struct to the linked list
        add(lista, dane);
    }
    fclose(file);
}

void print_list(node *lista) {
  while (lista->prev != NULL) {
    lista = lista->prev;
  }
  while (lista != NULL) {
    printf("name: %-20s brand: %-20s model: %-20s price: %-5s stock: %-5s\n", lista->data.name, lista->data.brand, lista->data.model, lista->data.price, lista->data.stock);
    lista = lista->next;
  }
}


int main()
{node *lista = NULL;ElectronicItem dane;
    read_from_csv(&lista);
    print_list(lista);
 printf("siema");
}