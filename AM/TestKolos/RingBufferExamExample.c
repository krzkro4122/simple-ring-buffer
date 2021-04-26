#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// Dana jest następująca struktura:
struct CharRingBuffer {
    char data[30];
    size_t head;
    size_t tail;
    size_t count;
    // TODO (1):
};
// (1) Uzupełnij strukturę CharRingBuffer tak, aby mogła opisywać bufor kołowy rozpięty na tablicy "data"

// (2) Zdefiniuj poniżej typ o nazwie CharRingBufferPtr będący wskaźnikiem do struktury CharRingBuffer
// TODO (2):
typedef struct CharRingBuffer *CharRingBufferPtr;


// (3) Napisz funkcję inicjalizującą bufor kołowy
void init(CharRingBufferPtr buffer) {
    memset(buffer->data, -1, sizeof (buffer->data));
    buffer->head = 0;
    buffer->tail = 0;
    buffer->count = 0;
// TODO (3):
}

// (4) Napisz funkcję, która wpisze do bufora kołowego łańcuch tekstowy "source".
//     Jeśli łańcuch ten nie zmieści się w całości do bufora, to nie należy go wogóle wpisywać.
//     Pamiętaj o tym, że każdy zapisany łańcuch znaków musi się kończyć zerem.
//     Funkcja ma zwracać 'true' jeśli udało się wpisać dany łańcuch lub 'false' w przeciwnym razie.
bool putText(CharRingBufferPtr buffer, const char* source) {

    int counter = 0;
    char temp[30];

    while (1){

        temp[counter] = source[counter];

        if(source[counter] == '\0')
            break;
        counter++;
    }

    if(strlen(temp) > sizeof (buffer->data) - buffer->count)
        return false;

    for( int i = 0; i <= strlen(temp); i++){

        if (i == strlen(temp))
            buffer->data[buffer->head % 30] = '\0';

        buffer->data[buffer->head % 30] = temp[i];
        buffer->head++; 
        buffer->count++;
    }
    return true;
}

// (5) Napisz funkcję, która będzie wyciągać (odczytywać i usuwać) z bufora kolejne wpisane do niego łańcuchy tekstowe.
//     Odczytany łańcuch znaków kopiowany jest do 'destination'
//     Funkcja ma zwracać 'true' jeśli udało się wyciągnąć jakiś łańcuch lub 'false' w przeciwnym razie.
bool getText(CharRingBufferPtr buffer, char* destination) {

    memset(destination, '\0', sizeof(buffer->data));

    char temp[30];    

    for(int i = 0; buffer->tail < buffer->head; i++){

        temp[i] = buffer->data[(buffer->tail + i) % 30];

        if( buffer->data[(buffer->tail + i) % 30] == '\0'){
            memcpy(destination, temp, strlen(temp));        

            for ( int j = 0; j < strlen(temp) + 1; j++)
                buffer->data[(buffer->tail + j) % 30] = -1;

            buffer->tail += i + 1;    
            buffer->count = (buffer->head % 30) - (buffer->tail % 30);
            return true;
        }
    }
    return false;
}

// (6) Napisz funkcję, która zwróci liczbę łańcuchów tekstowych zapisanych w buforze kołowym, bez wprowadzania zmian w samym buforze.
int getCount(CharRingBufferPtr buffer) {
    size_t countOfStrings = 0;
    for (size_t i = 0; i < sizeof (buffer->data); i++){
        if(buffer->data[i] == '\0')
            countOfStrings++;
    }
    return countOfStrings;
}


int main()
{
    printf("Testujemy...\n");

    struct CharRingBuffer buffer;
    char text[100];

    // najpierw inicjalizacja
    init(&buffer);
    // na początku bufor powinien być pusty
    assert(0 == getCount(&buffer));
    assert(false == getText(&buffer, text));

    // dodajemy 2 łańcuchy
    assert(true == putText(&buffer, "hello world"));
    assert(true == putText(&buffer, "good morning"));
    assert(2 == getCount(&buffer));
    // wyciągamy pierwszy
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "hello world"));
    assert(1 == getCount(&buffer));
    // wyciągamy drugi
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "good morning"));
    // teraz znów bufor powinien być pusty
    assert(0 == getCount(&buffer));
    assert(false == getText(&buffer, text));

    // wkładamy 3 łańcuchy po 8 znaków
    assert(true == putText(&buffer, "12345678"));
    assert(true == putText(&buffer, "abcdefgh"));
    assert(true == putText(&buffer, "ijklmnop"));
    // czwarty nie wejdzie...
    assert(false == putText(&buffer, "qrstuvwx"));
    // ..dopóki nie zrobimy miejsca:
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "12345678"));
    assert(true == putText(&buffer, "qrstuvwx"));

    // wyciągamy po kolei 3 łańcuchy
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "abcdefgh"));
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "ijklmnop"));
    assert(true == getText(&buffer, text));
    assert(0 == strcmp(text, "qrstuvwx"));

    // teraz znów bufor powinien być pusty
    assert(0 == getCount(&buffer));
    assert(false == getText(&buffer, text));

    printf("Koniec testu\n");
    return 0;
}
