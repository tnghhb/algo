/**
 * LinkedList with head node
 *
 * List works based on index
 * accepting only non-null value
 */

#include <stdlib.h>

typedef struct List {
    ListNode * head;
    int size;
} List;

static int betterIndex(int size, int index) {
    if (size == 0) {
        return index;
    }
    index = index % size;
    if (size > 6) {
        if (index >= 0) {
            if (index > size / 2 + 1) {
                index -= size + 1;
            }
        } else {
            if (index < -size / 2 - 1) {
                index += size + 1;
            }
        }
    }
    return index;
}

List * List_malloc() {
    List * self = calloc(1, sizeof(List));
    self->head = ListNode_malloc();
    self->size = 0;
    ListHead_enlinkNext(&self->head->linkage, &self->head->linkage); // cyclic
    return self;
}

void List_free(List * self) {
    List_clear(self);
    ListNode_free(self->head);
    free(self);
}

void List_clear(List * self) {
    while (!List_isEmpty(self)) {
        ListNode * p = ListNode_fromListHead(ListHead_removeNext(&self->head->linkage));
        --self->size;
        ListNode_free(p);
    }
}

int List_size(List * self) {
    return self->size;
}

boolean List_isEmpty(List * self) {
    return List_size(self) == 0;
}

void * List_get(List * self, int index) {
    index = betterIndex(self->size, index);
    index = index >= 0 ? index + 1 : index;
    ListNode * p = ListNode_offset(self->head, index);
    return p->value;
}

void * List_set(List * self, int index, void * value) {
    index = betterIndex(self->size, index);
    index = index >= 0 ? index + 1 : index;
    ListNode * p = ListNode_offset(self->head, index);
    void * old = p->value;
    p->value = value;
    return old;
}

void List_insert(List * self, int index, void * value) {
    // assert value != null;
    index = betterIndex(self->size, index);
    ListNode * p = ListNode_offset(self->head, index);
    ListNode * futureNext = ListNode_malloc();
    futureNext->value = value;
    ListHead_insertNext(&p->linkage, &futureNext->linkage);
    self->size++;
}

void * List_remove(List * self, int index) {
    index = betterIndex(self->size, index);
    ListNode * p = ListNode_offset(self->head, index);
    p = ListNode_fromListHead(ListHead_removeNext(&p->linkage));
    self->size--;
    void * value = p->value;
    ListNode_free(p);
    return value;
}

/**
 * negative for not found
 */
int List_indexOf(List * self, int start, void * value, compare_f * compare) {
    start = betterIndex(self->size, start);
    start = start >= 0 ? start + 1 : start;
    ListNode * p = ListNode_offset(self->head, start);
    while (p != NULL && p != self->head) {
        if (compare(p->value, value) == 0) {
            return start;
        }
        p = ListNode_offset(p, 1);
        ++start;
    }
    return -1;
}