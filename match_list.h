#ifndef MATCH_LIST_H
#define MATCH_LIST_H
#include "match.h"

typedef struct MatchList MatchList;
typedef struct MatchNode MatchNode;

MatchList *match_list_create(void);
void match_list_destroy(MatchList *list);
int match_list_size(const MatchList *list);

int match_list_append(MatchList *list, Match *m);

MatchNode *match_list_head(const MatchList *list);
MatchNode *match_node_next(const MatchNode *node);
Match *match_node_match(const MatchNode *node);


#endif