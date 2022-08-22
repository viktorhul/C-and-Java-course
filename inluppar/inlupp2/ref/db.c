#include <time.h>
#include "utils.c"
#include "utils.h"
#include "functions.h"
#include "../inlupp1/common.h"

/*--------------------------------------------------------------------------------------------------------*/

char *magic(char *array1[], char *array2[], char *array3[], int argc)
{
    char buf[255];
    strcpy(buf, array1[rand() % argc + 1]);
    buf[strlen(buf)] = '-';
    strcpy(&buf[strlen(buf)], array2[rand() % argc + 1]);
    buf[strlen(buf)] = ' ';
    strcpy(&buf[strlen(buf)], array3[rand() % argc + 1]);
    buf[strlen(buf)] = '\0';
    return strdup(buf);
}


/*--------------------------------------------------------------------------------------------------------*/

void print_item(merch_t *some_item)
{

    printf("Name: %s\n", (char*)((some_item)->name.pointer));
    printf("Desc: %s\n", (some_item)->desc);
    printf("Price: %d.%d SEK\n", ((some_item)->price)/100, (some_item)->price % 100);
}

/*--------------------------------------------------------------------------------------------------------*/

merch_t make_item(elem_t name, char *desc, int price, char *shelf)
{
    merch_t item_made = {.name = name, .desc = desc, .price = price, .locations = shelf};
    return item_made;
}
/*--------------------------------------------------------------------------------------------------------*/

bool input_item(database_t *db)
{
    char *strname = ask_question_string("What's the name of the item?\n");
    char *desc = ask_question_string("What's the description of the item?\n");
    int price = ask_question_int("What's the price of the item?\n");
    char *shelf = ask_question_shelf("Which shelf?\n");
    elem_t name = {.pointer = (void *)(strname)};

    return add_merch(db, name, desc, price);
}

/*--------------------------------------------------------------------------------------------------------*/

void print_menu() 
{
    char *A = "[A]dd merch";
    char *R = "[R]emove an item";
    char *E = "[E]dit an item";
    char *U = "[U]ndo";
    char *L = "[L]ist all items in the store";
    char *Q = "[Q]uit";
    printf("%s\n%s\n%s\n%s\n%s\n%s\n", A, R, E, U, L, Q);
}

/*--------------------------------------------------------------------------------------------------------


int remove_item_from_db(merch_t *databas, int db_siz)
{
    if (db_siz == 0)
    {
        puts("Empty!!");
        return db_siz;
    }
    merch_t *chosen_order;
    int order;
    order = ask_question_int("Enter an order number:\n")-1;
    while (order >= db_siz)
    {
        printf("%d is not in the database\n", order+1);
        order = ask_question_int("Enter an order number:\n")-1;
    }
    chosen_order = &databas[order];
    print_item(chosen_order);

    if (order == db_siz)
    {
        db_siz--;
    }
    for (int i = order; i < db_siz; i++)
    {

        databas[i] = databas[i+1];
    }
    db_siz--;

    return db_siz;

}

--------------------------------------------------------------------------------------------------------


void edit_db(merch_t *db_items, int db_siz)
{
    if (db_siz == 0)
    {
        puts("Empty!!");
        return;
    }
    merch_t *chosen_order;
    int order;
    order = ask_question_int("Enter an order number:\n")-1;
    while (order >= db_siz)
    {
        printf("%d is not in the database\n", order+1);
        order = ask_question_int("Enter an order number:\n")-1;
    }
    chosen_order = &db_items[order];
    print_item(chosen_order);
    // insert a new item
    elem_t *name;
    char *desc;
    int price;
    char *shelf;
    db_items[order] = input_item(name, desc, price, shelf);
    puts("New database:");   
} 

--------------------------------------------------------------------------------------------------------*/

void list_db(merch_t *items, int no_items)
{
    printf("%d. %s\n", no_items + 1, items->name);
}

/*--------------------------------------------------------------------------------------------------------*/

char ask_question_menu()
{
    char option;
    while (option == '\0')
    {
        print_menu();
        option = *ask_question_string("");
        if (option == 'l' || option == 'L')
        {
            return 'L';
        }
        else if(option == 't' || option == 'T')
        {
            return 'T';
        }
        else if(option == 'r' || option == 'R')
        {
            return 'R';
        }
        else if(option == 'g' || option == 'G')
        {
            return 'G';
        }
        else if(option == 'h' || option == 'H')
        {
            return 'H';
        }
        else if(option == 'a' || option == 'A')
        {
            return 'A';
        }
        else
        {
            option = '\0';
        }
    }
    return '\0';
}

/*--------------------------------------------------------------------------------------------------------*/

void print_list(merch_t *databas, int db_siz)
{
    for (int i = 0; i < db_siz; ++i)
    {
        merch_t *items = &databas[i];
        list_db(items, i);
    } 
}

/*--------------------------------------------------------------------------------------------------------*/

void event_loop(merch_t *databas, int db_siz)
{
    char choice;
    while (choice != 'A')
    {   
        choice = ask_question_menu();
        switch (choice)
        {
        case 'L':
            db_siz = add_merch_to_db(databas, db_siz);
            break;
        case 'T':
            db_siz = remove_item_from_db(databas, db_siz);
            break;
        case 'R':
            edit_db(databas, db_siz);
            break;
        case 'G':
            puts("Not yet implemented");
            break;
        case 'H':
            print_list(databas, db_siz);
            break;
        default:
            puts("Error, incorrect input!");
            break;
        }
        puts("\n");
    }
} 
/*--------------------------------------------------------------------------------------------------------*/



int main(void)
{
    char *array1[] = { "Laser",        "Polka",    "Extra" };
    char *array2[] = { "förnicklad",   "smakande", "ordinär" };
    char *array3[] = { "skruvdragare", "kola",     "uppgift" };
    srand(time(NULL));

    int data_size = 16;
    merch_t db[data_size]; 
    int db_siz = 0;
    
    event_loop(db, db_siz);

  return 0;
}








/*
int main ()
{
    char g = ask_question_menu();
    printf("%c",g);
    return 0;
}
*/



/*
// print list_db
int main()
{
    merch_t items = {.name = "mango", .desc ="fruit", .price = 5, .shelf = "a7"};
    list_db(&items, 1);
}
*/

/* prints magic
int main()
{
    char *array1[] = { "Laser", "Polka", "Extra" };
    char *array2[] = { "förnicklad", "smakande", "ordinär" };
    char *array3[] = { "skruvdragare", "kola", "uppgift" };
    char *str = magic(array1, array2, array3, 2);
    printf("%s\n", str);
}
*/


/* prints input_item
int main ()
{
    char *name;
    char *desc;
    int price;
    char *shelf;
    input_item(name, desc, price, shelf);
}
*/


/* prints print_item()
int main()
{
    merch_t some_item = {.name = "Apple", .desc = "Fruit", .price = 390, .shelf = "F1", };
    print_item(&some_item);
}
*/



/*prints make_item()
int main()
{
   merch_t new_item = make_item("Orange", "Fruit", 500, "F2");
   print_item(&new_item);
}
*/


/* redovisning achievement (pekare, osv)?
int main ()
{
    char *name;
    char *desc;
    int price;
    char *shelf;
    merch_t some_item1 = make_item(name, desc, price, shelf);
    print_item(&some_item1);
    merch_t some_item2 = input_item(name, desc, price, shelf);
    print_item(&some_item2);
}

*/