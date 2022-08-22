#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.c"

// Tasks to run
int tasks[] = {
    0, // 6.1
    0, // 6.2
    0, // 6.3
    0, // 6.4
    1, // 6.5
};

// 6.1
typedef struct {
    char *name;
    char *desc;
    int price;
    char *shelf;
} Item;

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
void input_item() {
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
    print_item(&item);
}

// 6.5
char *magick(char **arr1, char **arr2, char **arr3, int array_length) {
    char buf[255];
    int offset = 0;
    int random;

    random = 0;
    buf[offset] = *arr1[random];
    offset += string_length(arr1[random]) + 1;
    buf[offset] = '-';
    offset++;

    random = 2;
    buf[offset] = *arr2[random];
    offset += string_length(arr2[random]) + 1;
    buf[offset] = ' ';
    offset++;

    random = 0;
    buf[offset] = *arr3[random];
    offset += string_length(arr3[random]) + 1;
    buf[offset] = '\0';

    return strdup(buf);
}

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