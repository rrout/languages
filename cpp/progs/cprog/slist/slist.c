#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

list_t *insert(list_t *root, int data) {
    list_t *p = root, *q;
    list_t *node = (list_t *) malloc (sizeof(list_t));
    node->data = data;
    node->next = NULL;
    if (root == NULL) {
        return node;
    } else if (data < root->data)
    {
        node->next = root;
        return node;
    } else {
        q = p;
        while (p) {
            if (data < p->data) {
                node->next = p;
                q->next = node;
                return root;
            }
            q = p;
            p = p->next;
        }
        q->next = node;
    }
    return root;
}
void printlist(list_t *root) {
    for (;root; root = root->next) {
        printf("%d\t", root->data);
    }
}

int main() {
    list_t *root = NULL;
    int i;
    int arr[] = {11, 22, 33, 44, 66, 55, 88, 77};
    for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
        root = insert(root, arr[i]);
    }
    printlist(root);
    return 1;
}