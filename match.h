#ifndef MATCH_H
#define MATCH_H


/*TAD Match: uma partida já disputada. */

typedef struct Match Match;

Match *match_create(int id, int team1, int team2, int goals1, int goals2);
void match_destroy(Match *m);

int match_get_id(const Match *m);
int match_get_teams1(const Match *m);
int match_get_teams2(const Match *m);
int match_get_goals1(const Match *m);
int match_get_goals2(const Match *m);

/* Diferença absoluta de gols (usada na "maior goleada"). */

int match_goal_margin(const Match *m);

#endif
