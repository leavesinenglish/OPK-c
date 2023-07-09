#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hash_table.h"

unsigned jenkins_one_at_a_time_hash(char *key) {
    unsigned hash = 0;
    for (; *key; ++key) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

///* Инициализировать таблицу.
/// size - размер базового массива;
/// hf   - хеш-функция;
/// dtor - деструктор. Этой функции будут передаваться data удаляемых элементов
///        (ht_destroy, ht_delete, а также при перезаписи элементов в ht_set).
/// Если hf=0, берется стандартная функция (Дженкинс).
/// Если dtor=0, деструктор отсутствует (не будет вызываться).///
void ht_init(HashTable *ht, size_t size, HashFunction hf, Destructor dtor) {
    ht->size = size;
    if (hf == 0) ht->hashfunc = jenkins_one_at_a_time_hash;
    ht->dtor = dtor;
    ht->table = calloc(size, sizeof(elem));
    if (ht->table == NULL) {
        printf("Memory error\n");
        exit(1);
    }
}

///* Уничтожить таблицу *///
void ht_destroy(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i].key) {
            ht->dtor(ht->table[i].data);
        }
    }
    free(ht->table);
    ht->table = NULL;
    ht->size = -1;
}

int _ht_find_place(HashTable *ht, char *key) {
    unsigned hash = ht->hashfunc(key);
    unsigned probe = hash % ht->size;
    if (ht->table[probe].key == NULL)
        return (int) probe;
    if (strcmp(ht->table[probe].key, key) == 0)
        return (int) probe;
    int i = 0;
    while (ht->table[probe].key != NULL && probe < ht->size) {
        probe = (hash + i) % ht->size;
        i++;
    }
    if (ht->table[probe].key == NULL)
        return (int) probe;
    probe = 0;
    i = 0;
    while (ht->table[probe].key != NULL && probe < hash % ht->size) {
        probe = i;
        i++;
    }
    if (ht->table[probe].key == NULL)
        return (int) probe;
    else
        return -1;
}

///* Записать в таблицу соответствие key -> data. Если key уже существовал,проверяем ячейки массива,
/// пока не найдем первую свободную ячейку, в которую и запишем новый элемент *///
Pointer ht_set(HashTable *ht, char *key, Pointer data) {
    int probe = _ht_find_place(ht, key);
    if (probe == -1) {
        size_t new_size = (int) (ht->size * 1.5);
        elem *new_table = realloc(ht->table, new_size);
        if (new_table == NULL) {
            printf("Memory error");
            exit(1);
        }
        probe = ht->size; // чтобы в probe был старый конец + 1
        ht->size = new_size;
        ht->table = new_table;
    } else if (ht->table[probe].data != NULL && ht->dtor != 0)
        ht->dtor(ht->table[probe].data);
    ht->table[probe].key = key;
    ht->table[probe].data = data;
    return ht;
}

///* Получить значение по ключу. Если ключа нет в таблице, вернуть 0. *///
Pointer ht_get(HashTable *ht, char *key) {
    int probe = _ht_find_place(ht, key);
    if (probe == -1 || ht->table[probe].key == NULL)
        return 0;
    if (strcmp(ht->table[probe].key, key) == 0)
        return ht->table[probe].data;
    else return 0;
}

///* Проверка существования ключа key в таблице. 1 - есть, 0 - нет. *///
int ht_has(HashTable *ht, char *key) {
    if (ht_get(ht, key) == 0)
        return 0;
    return 1;
}

///* Удалить элемент с ключом key из таблицы (если он есть) *///
void ht_delete(HashTable *ht, char *key) {
    int probe = _ht_find_place(ht, key);
     if (probe == -1 || ht->table[probe].key == NULL)
         return;
     if (strcmp(ht->table[probe].key, key) == 0) {
         ht->dtor(ht->table[probe].data);
         ht->table[probe].key = NULL;
     }
}

///* Обход таблицы с посещением всех элементов. Функция f будет вызвана для
/// всех пар (key, data) из таблицы *///
void ht_traverse(HashTable *ht, void (*f)(char *key, Pointer data)) {
    for (int i = 0; i < ht->size; i++)
        f(ht->table[i].key, ht->table[i].data);
}

///* Изменить размер базового массива.
/// Примерный алгоритм: выделить новый массив table и перенести в него все
/// существующие структуры elem из старой table, затем старую освободить и
/// заменить новой.
/// Это эффективнее, чем создавать новую таблицу и делать в нее полноценные вставки. *///
void ht_resize(HashTable *ht, size_t new_size) {
    elem *new_table = calloc(new_size, sizeof(struct elem));
    if (new_table == NULL) {
        printf("Memory error");
        exit(1);
    }
    int not_null_elem = 0;
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i].key != NULL)
            not_null_elem++;
    }
    if (not_null_elem > new_size) {
        printf("Not enough space for this action. Please input bigger size. For example:" "%d", not_null_elem);
        exit(1);
    } else {
        for (int i = 0; i < ht->size; i++) {
            if (ht->table[i].key != NULL) {
                unsigned hash = ht->hashfunc(ht->table[i].key);
                unsigned probe = hash % new_size;
                if (new_table[probe].key != NULL) {
                    int j = 0;
                    while (new_table[probe].key != NULL && probe < new_size) {
                        probe = (hash + j) % new_size;
                        j++;
                    }
                    if (new_table[probe].key != NULL) {
                        probe = 0;
                        j = 0;
                        while (new_table[probe].key != NULL && probe < hash % new_size) {
                            probe = j;
                            j++;
                        }
                    }
                }
                new_table[probe].data = ht->table[i].data;
                new_table[probe].key = ht->table[i].key;
            }
        }
        ht->size = new_size;
        free(ht->table);
        ht->table = new_table;
    }
}