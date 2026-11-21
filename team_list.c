#include <stdlib.h>
#include "team_list.h"

struct TeamNode {

    Team *team;
    TeamNode *prev, *next;

};


struct TeamList {


    TeamNode *head, *tail;
    int size;

};

TeamList *team_list_create(void) {

    return calloc(1, sizeof(TeamList));

}

static void destroy(TeamList *list, int free_teams) {


    if(!list)
    return;

    TeamNode *n = list->head;

    while (n) {

        TeamNode *next = n->next;

        if (free_teams)

        team_destroy(n->team);
        free(n);
        n = next;


    }

    free(list);

}

void team_list_destroy(TeamList *list) {

    destroy(list, 0);


}


void team_list_destroy_with_teams(TeamList *list) {

    destroy(list, 1);


}

int team_list_size(const TeamList *list) {


    return list->size;


}

static void link_before(TeamList *list, TeamNode *n, TeamNode *pos) {

    n->next = pos;
    n->prev = pos ? pos->prev : list->tail;

    if(n->prev)

    n->prev->next = n;

    else

    list->head = n;

    if (pos)

    pos->prev = n;

    else
    list->tail = n;
    list->size++;

}

int team_list_append(TeamList *list, Team *t) {

    TeamNode *n = malloc(sizeof(TeamNode));
    if (!n)

    return -1;

    n->team = t;

    link_before(list, n, NULL);
    return 0;


}

int team_list_insert_sorted(TeamList *list, Team *t) {



    TeamNode *n = malloc(sizeof(TeamNode));

    if (!n)

    return -1;

    n->team = t;


    TeamNode *pos = list->head;

    while (pos && team_rank_compare(pos->team, t) < 0)

    pos = pos->next;
    link_before(list, n, pos);
    return 0;

}

Team *team_list_find(const TeamList *list, int id) {

    for (TeamNode *n = list->head; n; n = n->next)

        if (team_get_id(n->team) == id)

        return n->team;
    
    return NULL;


}

TeamNode *team_list_head(const TeamList *list) {


    return list->head;

}

TeamNode *team_node_next(const TeamNode *node) {


    return node->next;

}

Team *team_node_team(const TeamNode *node) {

    return node->team;

}