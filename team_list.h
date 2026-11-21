#ifndef TEAM_LIST_H
#define TEAM_LIST_H

#include "team.h"

typedef struct TeamList TeamList;
typedef struct TeamNode TeamNode;

TeamList *team_list_create(void);

void team_list_destroy(TeamList *list);

void team_list_destroy_with_teams(TeamList *list);

int team_list_size(const TeamList *list);

int team_list_append(TeamList *list, Team *t);

int team_list_insert_sorted(TeamList *list, Team *t);


Team *team_list_find(const TeamList *list, int id);

TeamNode *team_list_head(const TeamList *list);
TeamNode *team_node_next(const TeamNode *node);
Team *team_node_team(const TeamNode *node);

#endif