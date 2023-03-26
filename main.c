#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define swap(a, b) a = a + b; \
                    b = a - b;\
                    a = a - b;

int16_t input_int() { // функция ввода целого числа с проверками
    uint8_t output = 0, err = 0;
    char temp;
    bool minus = false;

    while ((temp = getchar()) != '\n') { // проверка на переход на новую строку
        if (!minus && temp == '-' && output == 0) {// проверка на минус
            minus = true;
        } else if (temp >= '0' && temp <= '9') { // проверка что символ является числом
            output = output * 10 + temp - '0';
        } else err++;
    }

    if (err)
        printf("Error: Check the input, %d errors detected\n", err); // сообщим пользователю о найденных ошибках
    if (minus) // учёт знака
        return -output;
    else return output;
}

typedef struct Vector { // структура для расширяемого массива
    int32_t *values; // содержимое
    uint16_t size; // реальный размер
    uint16_t capacity; // ёмкость массива
} Vector;


Vector *init_vector(int16_t n) { // функция инициализации маассива -> создание, установка ёмкости, размера, выделение памяти
    Vector *vec = (Vector *) malloc(sizeof(Vector));

    if (n < 0) { // обработка ввода отрицательного размера
        vec->capacity = 0;
        puts("Error: The size of the set must be >= 0");
    } else vec->capacity = n;
    vec->size = 0;
    vec->values = malloc(sizeof(int) * n);

    return vec;
}

void read_array(int size, int *values) { // функция считывания массива
    for (uint16_t i = 0; i < size; ++i) {
        printf("values[%d] = ", i);
        values[i] = input_int();
    }
}

void print_alphabet(uint8_t n) {
    if (n <= 9) printf("%c", n + '0');
    else printf("%c", (n + 'A' - 10));
}


void rec_print_sys(int in, int base) { // рекурсивная фунцкия для перевода из одной системы счисления в другую
    if (in < base) {
        print_alphabet(in);
        return;
    }
    rec_print_sys(in / base, base);
    print_alphabet(in % base);
}

void print_array(uint16_t n, int *values, int16_t system) { // функция вывода массива
    if (system == 10) // если система счисления 10ая, то просто выводим массив
        for (uint16_t i = 0; i < n; ++i) {
            printf("values[%d] = %d\n", i, values[i]);
        }
    else { // если другая, то переводим каждый элемент в нужную систему счисления
        for (uint16_t i = 0; i < n; ++i) {
            printf("values[%d] = ", i);
            if (values[i] < 0) { // проверка на отрицательные числа
                putchar('-');
                rec_print_sys(-values[i], system);
            } else rec_print_sys(values[i], system);
            putchar('\n');
        }
    }
}

void read_vector(Vector *vec) { // функция ввода массива - структуры
    vec->size = vec->capacity; // устанавливаем размер массива равный вместимости массива
    read_array(vec->size, vec->values); // считываем массив
}

void print_vector(Vector *vec, int16_t system) { // функция вывода массива - структуры
    printf("size: %d\n", vec->size); // выводим размер массива
    print_array(vec->size, vec->values, system);
}

void del_identical_num(Vector *vec) { // функция проверки на налицие одинаковых элементов в множестве и их удаления
    int *values = vec->values;
    for (uint16_t i = 0; i < vec->size; ++i) { // перебираем все элементы множества
        bool fl = true;
        for (uint16_t j = i + 1; j < vec->size; ++j) { // ищем дубликат текущего элемента
            if (values[i] == values[j])
                fl = false;
        }
        if (!fl) { //если элемент нашёлся, то удаляем его и сдвигаем массив на один элемент влево и уменьшаем размер на 1
            for (uint16_t j = i; j < vec->size; ++j)
                vec->values[j] = vec->values[j + 1];
            i--;
            vec->size--;
        }
    }
}


void sort_vector(Vector *vec) { // сортировка элементов множества пузырьком
    // сортировка пузырьком
    for (uint16_t i = 0; i < vec->size - 1; i++)
        for (uint16_t j = 0; j < vec->size - i - 1; j++)
            if (vec->values[j] > vec->values[j + 1]) {
                swap(vec->values[j], vec->values[j + 1]); // меняем элементы местами
            }
}

void free_vec(Vector *vec) {
    free(vec->values);
    free(vec);
}


int main() {
    // инициазизация перемнных
    int16_t s_size, system_base;
    Vector *s;

    // ввод размера множества
    puts("Size of s: ");
    s_size = input_int();

    // ввод системы счисления (по умолчанию 2)
    puts("To base: (Press enter to use 2)");
    if ((system_base = input_int()) < 2) {
        puts("Error: System base >= 2\nSet default base 2");
        system_base = 2;
    }


    s = init_vector(s_size); // инициализация множества
    read_vector(s);

    del_identical_num(s); // проврка на наличие повторяющихся элементов

    // вывод неотсортированных множеств
    puts("In 10 base result:");
    print_vector(s, 10);

    printf("In %d base result: \n", system_base);
    print_vector(s, system_base);

    sort_vector(s); // сортировка

    // вывод отсортированных множеств
    puts("In 10 base sorted result:");
    print_vector(s, 10);

    printf("In %d base sorted result: \n", system_base);
    print_vector(s, system_base);

    free_vec(s); // очистка выделенной памяти
    return 0;
}