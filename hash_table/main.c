#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hash_table.h"

typedef struct Contact {
    char *phone;
    char *addr;
} Contact;

void destructor(Pointer data) {
    free(data);
}

void destructor1(Pointer data){
    data = NULL;
}

int main() {
    {
        HashTable ht;
        ht_init(&ht, 10, 0, destructor1);
        ht_set(&ht, "Vasya", "333-45-67");
        ht_set(&ht, "Brother", "89056784533");
        ht_set(&ht, "Ambulance", NULL);
        assert(!strcmp(ht_get(&ht, "Vasya"), "333-45-67"));
        assert(ht_get(&ht, "FF NSU") == 0);
        assert(ht.size == 10);
        ht_resize(&ht, 20);
        assert(ht.size == 20);
        ht_resize(&ht, 10);
        assert(ht.size == 10);
        //ht_resize(&ht, 2);
        ht_delete(&ht, "Vasya");
        assert(!ht_has(&ht, "Vasya"));
        ht_destroy(&ht);
        assert(ht.size == -1);
    }
    {
        HashTable ht;
        ht_init(&ht, 10, 0, destructor);
        {
            Contact *pc = malloc(sizeof(Contact));
            if (pc == NULL) {
                printf("Memory error\n");
                exit(1);
            }
            pc->phone = "02";
            pc->addr = "Kutateladze";
            ht_set(&ht, "Police", pc);
        }
        ht_set(&ht, "Ambulance", NULL);
        assert(ht_has(&ht, "Police"));
        {
            Contact *pc2 = malloc(sizeof(Contact));
            if (pc2 == NULL) {
                printf("Memory error\n");
                exit(1);
            }
            pc2->phone = "911";
            pc2->addr = "NY";
            ht_set(&ht, "Police", pc2);
            assert(ht_get(&ht, "Police") == pc2);
        }
        assert(ht.size == 10);
        ht_resize(&ht, 20);
        assert(ht.size == 20);
        ht_resize(&ht, 10);
        assert(ht.size == 10);
        {
            Contact *pc1 = malloc(sizeof(Contact));
            if (pc1 == NULL) {
                printf("Memory error\n");
                exit(1);
            }
            pc1->phone = "88005553535";
            pc1->addr = "Moscow";
            ht_set(&ht, "Shop", pc1);
        }
        ht_delete(&ht, "Shop");
        assert(ht_has(&ht, "Shop") == 0);
        ht_destroy(&ht);
        assert(ht.size == -1);
    }
    // Send all reports to STDOUT
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

    _CrtDumpMemoryLeaks();

    return 0;
}