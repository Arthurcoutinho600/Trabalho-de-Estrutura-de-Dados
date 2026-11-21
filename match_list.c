#include <stdlib.h>
#include "match_list.h"

struct MatchNode {

    Match *match;
    MatchNode *next;
};

struct MatchList {

     MatchNode *head, *tail;
     int size;

};


MatchList *match_list_create(void) {

    return calloc(1, sizeof(MatchList));


}

void match_list_destroy(MatchList *list) {

    if (!list)

    return;

    MatchNode *n = list->head;

    while (n) {

        MatchNode *next = n->next;
        match_destroy(n->match);
        free(n);
        n = next;


    }

    free(list);

}

int match_list_size(const MatchList *list) {

    return list->size;


}

int match_list_append(MatchList *list, Match *m) {

    MatchNode *n = malloc(sizeof(MatchNode));

    if(!n)

    return -1;

    n->match = m;
    n->next = NULL;
    
    if (list->tail)

    list->tail->next = n;
    else list->head = n;
    list->tail = n;
    list->size++;

    return 0;


}

MatchNode *match_list_head(const MatchList *list) {

    return list->head;


}

MatchNode *match_node_next(const MatchNode *node) {

    return node->next;
}

Match *match_node_match(const MatchNode *node) {

    return node->match;


}