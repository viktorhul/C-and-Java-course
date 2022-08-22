#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "utils.c"

// Tasks to run
int tasks[] = {
    0, // 6.1
    0, // 6.2
    0, // 6.3
    0, // 6.4
    1, // 6.5
    0, // 6.6
};

// 6.1
typedef struct  {
    char *name;
    char *desc;
    int price;
    char *shelf;
} Item;

//struct Item{
//    char *name;
//    char *desc;
//    int price;
//    char *shelf;}; 
//
//typedef struct Item item_t; 
//typedef struct Item Item;
//typedef struct Item item_t;



// 6.1
bool ask_question_shelf(char *shelf) {
    if (!(*shelf >= 65 && *shelf <= 90) && !(*shelf >= 97 && *shelf <= 122))
        return false;

    while (*++shelf != '\0') 
        if (!isdigit(*shelf)) 
            return false;

    return true;
}

// 6.2
void print_item(Item *item) {
    printf(
        "Name: %s\n"
        "Desc: %s\n"
        "Price: %d.%d SEK\n"
        "Shelf: %s\n",
        item->name,
        item->desc,
        (item->price / 100), // kronor
        (item->price % 100), // ören
        item->shelf
    );
}

// 6.3
Item make_item(char *name, char *desc, int price, char *shelf) {

    Item new_item = {
        name,
        desc,
        price,
        shelf
    };

    return new_item;
}

// 6.4
Item input_item() {
    char *name = ask_question_string("Ange produktens namn");
    char *desc = ask_question_string("Ange en beskrivning");
    double price = ask_question_float("Ange ett pris (xx.xx)");
    char *shelf;
    do {
        shelf = ask_question_string("Ange lagerhylla (en bokstav följt av siffror)");
    } while (!ask_question_shelf(shelf));

    Item item = make_item(
        name, 
        desc, 
        price * 100, 
        shelf
    );
    return (item);
}

// 6.5

void add_strings(int *offset, char *str, char *to_copy) {
    int char_num = string_length(to_copy); 
    //printf("\n\n%d",char_num);
    
    for (int i = 0; i < char_num; i++) {
        //printf("\ni=%d adress=%d  letter: ",i,*offset);
        str[*offset]=to_copy[i];
        //printf("%c",str[*offset]);
        *offset=*offset+1;
    }

}

char *magick(char **arr1, char **arr2, char **arr3, int array_length) {
    char buf[255]={0};
    int offset=0;
    char *spec_char[] = {"-", " ", "\0"};

    int random = rand() % 3;
    
    add_strings(&offset,buf,arr1[random]);
    add_strings(&offset,buf,spec_char[0]);
    random = rand() % 3;
    add_strings(&offset,buf,arr2[random]);
    add_strings(&offset,buf,spec_char[1]);
    random = rand() % 3;
    add_strings(&offset,buf,arr3[random]);
    add_strings(&offset,buf,spec_char[2]);

    return strdup(buf);
}

void list_db(Item *items, int no_items){

    for (int i = 1; i <= no_items; i++)
    {
          printf("%d. ",i);
          printf("%s\n",items++->name);
        
    }
}

void edit_db(Item *db){
    int to_change = ask_question_int("Vilken vara vill du editera");
    //Item printish = db[1];
    print_item(&db[to_change-1]);
    db[to_change-1] = input_item();


    

}

//    for (int i = 0; i < no_items; i++) {
//        if (!not_empty(items->name)) break;
//        
//        printf("%d. %s\n", 
//        i+1, 
//        items->name++ );
//    }


int main(int argc, char *argv[]) {
    srand(time(0));
    char *array1[] = {"Laser", "Polka", "Extra"}; // TODO: Lägg till!
    char *array2[] = { "Laser", "Polka", "Extra" }; // TODO: Lägg till!
    char *array3[] = { "Laser", "Polka", "Extra" }; // TODO: Lägg till!

    if (argc < 2) {
        printf("Usage: %s number\n", argv[0]);
    } else {
        Item db[16]; // Array med plats för 16 varor
        int db_siz = 0; // Antalet varor i arrayen just nu

        int items = atoi(argv[1]); // Antalet varor som skall skapas

        if (items > 0 && items <= 16) {
            for (int i = 0; i < items; ++i) {
            // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
            Item item = input_item();
            db[db_siz] = item;
            ++db_siz;
        }
    } else {
        puts("Sorry, must have [1-16] items in database.");
        return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i) {
        char *name = magick(array1, array2, array3, 3);// TODO: Lägg till storlek
        char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
        int price = rand() % 200000;
        char shelf[] = { 
            rand() % ('Z'-'A') + 'A',
            rand() % 10 + '0',
            rand() % 10 + '0',
            '\0'
        };
        
        Item item = make_item(name, desc, price, shelf);
        
        db[db_siz] = item;
        print_item(&db[db_siz]);
        ++db_siz;
      }

     // Skriv ut innehållet
    for (int i = 0; i < db_siz; ++i) {
        print_item(&db[i]);
    }
    
    list_db(db,16);

    edit_db(db);
    list_db(db,16);


  return 0;
  }

/*
int main(void) {
    Item item1 = { "Alkohol", "Sångvatten, life juice, kärt barn har många namn", 525, "B25" };

    // 6.1
    if (tasks[0]) {
        printf("Shelf storage format (%s) is %s\n\n",
            item1.shelf,
            ask_question_shelf(item1.shelf) ? "OK" : "NOT OK"
        );
    }

    // 6.2
    if (tasks[1]) {
        print_item(&item1);
        printf("\n");
    }

    // 6.3
    if (tasks[2]) {
        Item item2 = make_item("Kaffe", "En svensk utan kaffe vill ingen råka träffa", 4999, "F95");
        print_item(&item2);
        printf("\n");
    }

    // 6.4
    if (tasks[3]) {
        input_item();
    }

    // 6.5
    if (tasks[4]) {
        char *array1[] = { "Laser", "Polka", "Extra" };
        char *array2[] = { "förnicklad", "smakande", "ordinär" };
        char *array3[] = { "skruvdragare", "kola", "uppgift" };

        char *str = magick(array1, array2, array3, 3);

        puts(str);
    }
}
*/
}

