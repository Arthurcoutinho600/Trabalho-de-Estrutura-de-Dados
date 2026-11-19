#include <stdlib.h>
#include "match.h"

struct Match
{

    int id, team1, team2, goals1, goals2;
};


Match *match_create(int id, int team1, int team2, int goals1, int goals2) {

    Match *m = malloc(sizeof(Match));
    if (!m)
    return NULL;

    m->id = id;
    m->team1 = team1;
    m->team2 = team2;
    m->goals1 = goals1;
    m->goals2 = goals2;

    return m;
}

void match_destroy(Match *m) {

    free(m);

}

int match_get_id(const Match *m) {

    return m->id;

}

int match_get_team1(const Match *m) {

    return m->team1;

}

int match_get_team2(const Match *m) {

    return m->team2;

}

int match_get_goals1(const Match *m) {

    return m->goals1;

}

int match_get_goals2(const Match *m) {

    return m->goals2;

}

int match_goal_margin(const Match *m) {

    int d = m->goals1 - m->goals2;

    return d < 0 ? -d: d;

}