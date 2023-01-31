#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NAME_COMPARE 1
#define BRAND_COMPARE 2
#define PRICE_COMPARE 3

//*ADD SHOPPING CART , ADD TO CART, DELETE FROM CART, BUY ITEMS
typedef struct ElectronicItem {
    char name[50];
    char brand[20];
    char model[20];
    float price;
    char stock[5];
}ElectronicItem;

typedef struct node
{
    struct node *next;
    struct node *prev; 
    ElectronicItem data;
}node;

typedef struct ShoppingCart
{
    ElectronicItem cart_slots[10];
    int items;
    int capacity;
    float value;
    
}ShoppingCart;



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
            else if (i == 3) dane.price = atof(field);
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
    printf("name: %-20s brand: %-20s model: %-20s price: %-5.2lf stock: %-5s\n", lista->data.name, lista->data.brand, lista->data.model, lista->data.price, lista->data.stock);
    lista = lista->next;
  }
}

void swap(node *a, node *b)
{
    ElectronicItem temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void to_lower(char *name)
{
    int i = 0;
    while (name[i])
    {
        name[i] = tolower(name[i]);
        i++;
    }
}

void sort_list(node **lista, int field) {
  if(field!= NAME_COMPARE && field !=BRAND_COMPARE && field != PRICE_COMPARE) {
        printf("Error: Invalid field.\n");
        return;
  }

  node *current = *lista;

  while (current->prev != NULL) {
    current = current->prev;
  }

  int swapped;
  node *ptr1;
  node *lptr = NULL;

  if (current == NULL)
    return;

  do
  {
    swapped = 0;
    ptr1 = current;

    while (ptr1->next != lptr)
    {
        if (field == NAME_COMPARE) {
            char name1[20], name2[20];
            if(strlen(ptr1->data.name)>=19) 
                strncpy(name1, ptr1->data.name,19);
            else
                strcpy(name1, ptr1->data.name);
            if(strlen(ptr1->next->data.name)>=19) 
                strncpy(name2, ptr1->next->data.name,19);
            else
                strcpy(name2, ptr1->next->data.name);
            to_lower(name1);
            to_lower(name2);
            if (strcmp(name1, name2) > 0)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
        } else if (field== BRAND_COMPARE) {
            char brand1[20], brand2[20];
            if(strlen(ptr1->data.brand)>=19) 
                strncpy(brand1, ptr1->data.brand,19);
            else
                strcpy(brand1, ptr1->data.brand);
            if(strlen(ptr1->next->data.brand)>=19) 
                strncpy(brand2, ptr1->next->data.brand,19);
            else
                strcpy(brand2, ptr1->next->data.brand);
            to_lower(brand1);
            to_lower(brand2);
            if (strcmp(brand1, brand2) > 0)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
        } else {
            float price1=0; float price2=0;
                price1 = ptr1->data.price;
                price2 = ptr1->next->data.price;

            if (price1<price2)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
        }

        ptr1 = ptr1->next;
    }
    lptr = ptr1;
  } while (swapped);
}

void search_name(node *lista, char search[20])
{
    node *current = get_first(lista);
    int i;
    to_lower(search);
    while (current != NULL)
    {
        char name_temp[20],brand_temp[20];
        strcpy(name_temp,current->data.name);
        strcpy(brand_temp,current->data.brand);
        to_lower(name_temp);
        to_lower(brand_temp);
        
        if (strstr(name_temp, search) != NULL || strstr(brand_temp, search) != NULL)
        {
            printf("found:  ");
            printf("name: %-20s brand: %-20s model: %-20s price: %-5.2lf stock: %-5s\n", current->data.name, current->data.brand, current->data.model, current->data.price, current->data.stock);
        }
        current = current->next;
    }
}

void init_shopping_cart(ShoppingCart *cart)
{
    cart->capacity = 10;
    cart->items = 0;
    cart->value = 5.25;
}

void add_to_cart(ShoppingCart *cart, char *name, char *model, node **lista)
{
    if(cart->capacity==cart->items)
    {
        printf("Cart is full! Max items in cart is 10!");
        return;
    }
    strcpy(cart->cart_slots[cart->items].name, name);
    strcpy(cart->cart_slots[cart->items].model, model);
    cart->items++;
}

void menu_main(int * choice)
{
    printf("\n1) SORT LIST\n2) SEARSCH\n 3) ADD TO CART\n 4) SHOW CART\n 5) BUY\n 6) QUIT\n");
    while (1) {
    printf("What do you want to do: ");
    if (scanf("%d", &*choice) == 1) {
      break;
    } else {
      printf("Invalid input, please try again.\n");
      while (getchar() != '\n');
    }
  }
}

void menu_sort(int * choice2)
{
    printf("Type what you want to do?\n");
    printf("\n1)sort by name\n2)sort by brand\n3)sort by price\n");
    while (1) {
    printf("What do you want to do: ");
    if (scanf("%d", &*choice2) == 1) {
      break;
    } else {
      printf("Invalid input, please try again.\n");
      while (getchar() != '\n');
    }
  }
}

void menu_find(int * choice3)
{
    printf("Type what you want to do?\n");
    printf("\n1)find by name and name\n2)find by brand\n");
    while (1) {
    printf("what do you want to do: ");
    if (scanf("%d", &*choice3) == 1) {
      break;
    } else {
      printf("Invalid input, please try again.\n");
      while (getchar() != '\n');
    }
  }
}

void show_shopping_cart(ShoppingCart cart, node * lista)
{
    printf("Your Cart contains:\n");
    for (int i = 0; i < cart.items; i++) 
    {
        printf("\n");
        printf("- %s %s\n", cart.cart_slots[i].name,cart.cart_slots[i].model);
    }

    printf("Value : %.2f $", cart.value);
}

int main()
{node *lista = NULL;ElectronicItem dane;char name[20];ShoppingCart cart;int choice;int choice2;int choice3;char name_searsch[20];char brand_searsch[20];

read_from_csv(&lista);
init_shopping_cart(&cart);
print_list(lista);
    do
    {
        menu_main(&choice);
        switch(choice)
        {
            //SORT
            case 1:
            {

    menu_sort(&choice2);
    switch(choice2)
    {
        case 1:
        {
            sort_list(&lista, NAME_COMPARE);
            print_list(lista);
        }break;
        case 2:
        {
            sort_list(&lista, BRAND_COMPARE);
            print_list(lista);
        }break;
        case 3:
        {
            sort_list(&lista, PRICE_COMPARE);
            print_list(lista);
        }break;

    }
    
            }break;
            //FIND
            case 2:
            {
                menu_find(&choice3);
                switch(choice3)
                {
                    case 1:
                    printf("Enter name to search: \n");
                scanf("%s", name_searsch);
                    search_name(lista, name_searsch);
                    break;
                    case 2:
                    printf("Enter brand to search: \n");
                scanf("%s", brand_searsch);
                        search_name(lista, brand_searsch);
                    search_name(lista, brand_searsch);
                }
                
            }break;
            //ADD to cart
            case 3:
            {
                add_to_cart(&cart, "Transistor", "2SC5200 ", &lista);               
            }break;
            
            //SHOW CARD
            case 4:
            {
                show_shopping_cart(cart, lista);
            }break;
            /**BUY
             * @brief case 5
             * {
             *  
             * }
             * 
             */
            //Quitting;
            case 6:
            {
                printf("QUITTING!");
                clear_list(&lista);
                printf("Free memory!");
                return 0;
            }
        }
    } while (choice<=6 && choice>=1);

}