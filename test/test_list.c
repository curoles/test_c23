#include <assert.h>
#include <stdio.h>
#include "list.h"

typedef struct mylist {
    list_head_t list;
    int id;
} mylist_t;

int test_list(void)
{
    constexpr unsigned int SIZE = 16;
    mylist_t items[SIZE];

    LIST_HEAD(list);
    assert(list_empty(&list));

    items[0].id = 0;
    list_add_tail(&items[0].list, &list);
    assert(!list_empty(&list));
    assert(list_is_singular(&list));

    list_del(&items[0].list);
    assert(list_empty(&list));
    list_add_tail(&items[0].list, &list);
    assert(!list_empty(&list));

    mylist_t* p = list_entry(&items[0].list, mylist_t, list);
    printf("%p id:%d\n", p, p->id);
    assert(p->id == 0);

    items[1].id = 1;
    list_add_tail(&items[1].list, &list);
    p = list_entry(&items[1].list, mylist_t, list);
    assert(p->id == 1);

    items[2].id = 2;
    list_add_tail(&items[2].list, &list);
    p = list_entry(&items[2].list, mylist_t, list);
    assert(p->id == 2);
    
    assert(list_count_nodes(&list) == 3);

    int i = 0;
    for (list_head_t* ptr = list.next; ptr != &list; ptr = ptr->next) {
        p = list_entry(ptr, mylist_t, list);
        printf("id:%d\n", p->id);
        assert(p->id == i);
        ++i;
    }

    return 0;
}
