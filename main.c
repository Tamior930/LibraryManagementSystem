#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM 32

enum genre_type
{
    Horror = 1,
    Abenteuer,
    Romantik,
    Sachbuch
};

struct book
{
    char title[MAX_NUM];
    enum genre_type genre;
    int year;
    int amount;
};

struct bookNode
{
    struct book content;
    struct bookNode* next;
};

struct borrowNode
{
    char title[MAX_NUM];
    char name[MAX_NUM];
    struct borrowNode* next;
};

void print_genre(enum genre_type genre)
{
    switch (genre)
    {

    case 1:
        printf("Horror ");
        return;

    case 2:
        printf("Abenteuer ");
        return;

    case 3:
        printf("Romantik ");
        return;

    case 4:
        printf("Sachbuch ");
        return;

    }
}

void printList(struct bookNode * bookList)
{
    int i = 0;
    struct bookNode * printOut = bookList;

    while(printOut != NULL)
    {
        printf("\n%d: %s, ", i + 1, printOut->content.title);
        print_genre(printOut->content.genre);
        printf("(%d)", printOut->content.year);
        printOut = printOut->next;
        i++;
    }
    printf("\n");
}

void printListInOrder(struct bookNode * bookList)
{
    int counter = 0;
    struct bookNode * current = bookList;

    while (current != NULL)
    {
        counter++;
        current = current->next;
    }

    struct bookNode **bookArray = (struct bookNode **)malloc(counter * sizeof(struct bookNode *));

    current = bookList;
    for (int i = 0; i < counter; i++)
    {
        bookArray[i] = current;
        current = current->next;
    }

    for (int i = 0; i < counter - 1; i++)
    {
        for (int j = 0; j < counter - i - 1; j++)
        {
            if (bookArray[j]->content.year > bookArray[j + 1]->content.year)
            {
                struct bookNode *temp = bookArray[j];
                bookArray[j] = bookArray[j + 1];
                bookArray[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < counter; i++)
    {
        printf("\n%d: %s, ", i + 1, bookArray[i]->content.title);
        print_genre(bookArray[i]->content.genre);
        printf("(%d)", bookArray[i]->content.year);
    }
    printf("\n");

    free(bookArray);
}


void insertBook(struct bookNode ** list, struct book * bookInput, int * counter)
{

    struct bookNode * newNode = (struct bookNode*)malloc(sizeof(struct bookNode));

    while(1)
    {
        printf("\nGeben Sie den Titel ein: ");
        scanf("%31s", bookInput->title);
        if(strlen(bookInput->title) > 0 && strlen(bookInput->title) < 32)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    while(1)
    {
        printf("\nGeben Sie das Genre ein. Horror (1), Abenteuer (2), Romantik (3), Sachbuch (4): ");
        scanf("%d", (int *)&bookInput->genre);
        if(bookInput->genre == 1 || bookInput->genre == 2 || bookInput->genre == 3 || bookInput->genre == 4)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    while(1)
    {
        printf("\nGeben Sie das Erscheinungsjahr ein: ");
        scanf("%d", &bookInput->year);
        if(bookInput->year > 0)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    while(1)
    {
        printf("\nGeben Sie ein wieviele Exemplare vorhanden sind: ");
        scanf("%d", &bookInput->amount);
        if(bookInput->amount > 0)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    (*counter)++;
    newNode->content = *bookInput;
    newNode->next = *list;
    *list = newNode;
}

void borrowBook(struct borrowNode ** borrowList, struct bookNode ** bookList, int counter)
{

    if (*bookList == NULL)
    {
        printf("\n");
        printf("\nEs sind keine Buecher im Inventar vorhanden.");
        return;
    }

    int input = 0;
    struct borrowNode* newNode = (struct borrowNode*)malloc(sizeof(struct borrowNode));

    printList(*bookList);

    while (1)
    {
        printf("\nWelchen Titel moechten Sie leihen? (1-%d): ", counter);
        scanf("%d", &input);
        if (input > 0 && input <= counter)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    struct bookNode * selectedBook = *bookList;
    for (int i = 1; i < input; i++)
    {
        selectedBook = selectedBook->next;
    }

    strcpy(newNode->title, selectedBook->content.title);

    while (1)
    {
        printf("\nGeben Sie Ihren Namen ein: ");
        scanf("%31s", newNode->name);
        if (strlen(newNode->name) > 0 && strlen(newNode->name) < 32)
            break;
        else
            printf("\nUngueltige Eingabe!");
    };

    if (selectedBook->content.amount == 0)
    {
        printf("\nBereits alle Exemplare ausgeliehen!");
        free(newNode);
        return;
    }

    struct borrowNode* current = *borrowList;
    while (current != NULL)
    {
        if (strcmp(current->title, newNode->title) == 0 && strcmp(current->name, newNode->name) == 0)
        {
            printf("\nSie haben diesen Titel bereits ausgeliehen!");
            free(newNode);
            return;
        }
        current = current->next;
    }

    selectedBook->content.amount--;
    newNode->next = *borrowList;
    *borrowList = newNode;

}

void returnBook(struct borrowNode **borrowList, struct bookNode **bookList)
{
    if (*borrowList == NULL)
    {
        printf("\n");
        printf("\nEs sind keine Titel ausgeliehen!");
        return;
    }

    int i = 0;
    struct borrowNode *printOut = *borrowList;

    while (printOut != NULL)
    {
        printf("\n%d: %s geliehen von %s", i + 1, printOut->title, printOut->name);
        printOut = printOut->next;
        i++;
    }
    printf("\n");

    int input;
    while (1)
    {
        printf("\nWelchen Titel moechten Sie retournieren? (1-%d): ", i);
        scanf("%d", &input);
        if (input > 0 && input <= i)
            break;
        else
            printf("\nUngueltige Eingabe!");
    }

    struct borrowNode *selectedUser = *borrowList;
    for (int j = 1; j < input; j++)
    {
        selectedUser = selectedUser->next;
    }

    struct bookNode *currentBook = *bookList;
    while (currentBook != NULL)
    {
        if (strcmp(selectedUser->title, currentBook->content.title) == 0)
        {
            currentBook->content.amount++;
            break;
        }
        currentBook = currentBook->next;
    }

    if (input == 1)
    {
        struct borrowNode *temp = *borrowList;
        *borrowList = (*borrowList)->next;
        free(temp);
    }
    else
    {
        struct borrowNode *temp = *borrowList;
        struct borrowNode *prev = NULL;
        for (int j = 0; j < input - 1; j++)
        {
            prev = temp;
            temp = temp->next;
        }

        if (temp != NULL)
        {
            prev->next = temp->next;
            free(temp);
        }
    }
}

struct borrowNode * freeBook1(struct borrowNode * head)
{
    if (head != NULL)
    {
        freeBook1(head->next);
        free(head);
    }
    return NULL;
}

struct bookNode * freeBook2(struct bookNode * head)
{
    if (head != NULL)
    {
        freeBook2(head->next);
        free(head);
    }
    return NULL;
}

int main ()
{

    char input;
    int bookTimes = 0;
    struct book bookInput;
    struct bookNode * bookList = NULL;
    struct borrowNode * borrowList = NULL;

    do
    {

        printf("\nNeues Buch eingeben (n), Buch ausleihen (b), Buch zurueckgeben (r), Buecher auflisten (l), Buecher sortieren (s), Programm beenden (x)");
        printf("\nAuswahl: ");
        scanf(" %c", &input);

        switch(input)
        {
        case 'n':

            insertBook(&bookList, &bookInput, &bookTimes);
            break;

        case 'b':

            borrowBook(&borrowList, &bookList, bookTimes);
            break;

        case 'r':

            returnBook(&borrowList, &bookList);
            break;

        case 'l':

            printList(bookList);
            break;

        case 's':

            printListInOrder(bookList);
            break;

        default:

            if(input != 'x')
            {
                printf("\nUngueltige Eingabe!");
                break;
            }
            else break;

        }

    }
    while(input != 'x');

    freeBook1(borrowList);
    freeBook2(bookList);

    return 0;
}
