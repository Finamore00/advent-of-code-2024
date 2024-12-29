#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct ListNode_s {
    int key;
    int value;
    struct ListNode_s *next;
} ListNode;

int list_add(ListNode *node, int key, int value) {
    if (node == NULL) {
        return -1;
    }

    ListNode *current = 0;
    for (current = node; current != NULL; current = current->next) {
        if (current->key == key) {
            current->value = value; //Overwrite value if already present
            return 0;
        }

        if (current->next == NULL) {
            break;
        }
    }

    ListNode *new_node = malloc(sizeof(ListNode));
    new_node->key = key;
    new_node->value = value;
    current->next = new_node;

    return 0;
}

typedef struct HashTable_s {
    ListNode **arr;
    int (*hash)(struct HashTable_s *table, int key);
    size_t len;
} HashTable;

int hash_table_hash(HashTable *table, int key) {
    if (table == NULL) {
        return -1;
    }

    return (key % table->len);
}

HashTable *new_hash_table(size_t capacity) {
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;
    }

    table->arr = calloc(capacity, sizeof(ListNode *));
    if (table->arr == NULL) {
        free(table);
        return NULL;
    }

    table->len = capacity;
    table->hash = hash_table_hash;
    return table;
}


int hash_table_register(HashTable *table, int key, int value) {
    if (table == NULL) {
        return -1;
    }

    int h = table->hash(table, key);
    if (table->arr[h] == NULL) {
        table->arr[h] = malloc(sizeof(ListNode));
        table->arr[h]->key = key;
        table->arr[h]->value = value;
    } else {
        list_add(table->arr[h], key, value);
    }

    return 0;
}

int hash_table_get(HashTable *table, int key) {
    if (table == NULL) {
        return -1;
    }

    int h = table->hash(table, key);
    for (ListNode *current = table->arr[h]; current != NULL; current = current->next) {
        if (current->key == key) {
            return current->value;
        }
    }

    return -1;
}

void hash_table_destroy(HashTable *table) {
    if (table == NULL) {
        return;
    }

    if (table->arr != NULL) {
        for (int i = 0; i < table->len; i++) {
            for (ListNode *current = table->arr[i]; current != NULL;) {
                ListNode *prev = current;
                current = current->next;
                free(prev);
            }
        }
        free(table->arr);
    }

    free(table);
    return;
}

int split(char *str, size_t len, char delim) {
    for (int i = 0; i < len; i++) {
        if (str[i] == delim) {
            return i;
        }
    }

    return -1;
}

int main(void) {
    int list1[1000];
    int list2[1000];

    FILE *f = fopen("./input", "r");

    if (f == NULL) {
        fprintf(stderr, "ERROR: Couldn't open input file.\n");
        exit(1);
    }

    char buff[51];
    char n1_str[25];
    char n2_str[25];

    for (int line = 0; line < 1000; line++) {
        if (!fgets(buff, 50, f)) {
            fprintf(stderr, "ERROR: Error reading from file.\n");
            exit(1);
        }

        int sp_idx = split(buff, strlen(buff), ' ');
        if (sp_idx < 0) {
            fprintf(stderr, "ERROR: you goofed up");
            exit(1);
        }

        strncpy(n1_str, buff, sp_idx);
        strncpy(n2_str, buff + sp_idx + 1, strlen(buff + sp_idx + 1));

        list1[line] = atoi(n1_str);
        list2[line] = atoi(n2_str);
    }

    int res = 0;
    HashTable *table = new_hash_table(50);

    for (int i = 0; i < 1000; i++) {
        if (hash_table_get(table, list1[i]) == -1) {
            int counter = 0;
            for (int j = 0; j < 1000; j++) {
                if (list2[j] == list1[i]) {
                    counter++;
                }
            }
            hash_table_register(table, list1[i], counter);
        }

        res += list1[i] * hash_table_get(table, list1[i]);
    }

    printf("Answer is %d\n", res);
    hash_table_destroy(table);
    return 0;
}