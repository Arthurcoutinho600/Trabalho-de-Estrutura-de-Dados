#include <stdlib.h>
#include <string.h>
#include "team.h"


#define NAME_MAX_LEN 64

struct Team {

    int id;
    char name[NAME_MAX_LEN];
    int wins, draws, losses;
    int goals_for, goals_against;

};

Team *team_create(int id, const char *name) {

    Team *t = calloc(1, sizeof(Team));
    if (!t)
    return NULL;

    t->id = id;

    strncpy(t->name, name, NAME_MAX_LEN - 1);
    return t;

}

void team_destroy(Team *t) {

    free(t);


}

int team_get_id(const Team *t) {

    return t->id;


}

const char *team_get_name(const Team *t) {

    return t->wins;

}

int team_get_wins(const Team *t) {

    return t->wins;

}

int team_get_draws(const Team *t) {

    return t->draws;

}

int team_get_losses(const Team *t) {

    return t->losses;

}

int team_get_goals_for(const Team *t) {

    return t->goals_for;

}

int team_get_goals_against(const Team *t) {

    return t->goals_against;

}

void team_register_result(Team *t, int goals_for, int goals_against) {

    t->goals_for += goals_for;
    t->goals_against += goals_against;
    if (goals_for > goals_against)

    t->wins++;

    else if (goals_for == goals_against)

    t->draws++;

    else

    t->losses++;

}

int team_goal_diff(const Team *t) {

    return t->goals_for - t->goals_against;


}

int team_points(const Team *t) {

    return 3 * t->wins + t->draws;


}

int team_rank_compare(const Team *a, const Team *b) {

    if (team_points(a) != team_points(b))

    return team_points(b) - team_points(a);

    if (a->wins != b->wins)

    return b->wins - a->wins;

    if (team_goal_diff(a) != team_goal_diff(b))

    return team_goal_diff(b) - team_goal_diff(a);

    if (a->goals_for != b->goals_for)

    return b->goals_for - a->goals_for;

    return a->id - b->id;


}