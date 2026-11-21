#ifndef TEAM_H
#define TEAM_H

typedef struct Team Team;

Team *team_create(int id, const char *name);
void team_destroy(Team *t);

int team_get_id(const Team *t);

const char *team_get_name(const Team *t);
int team_get_wins(const Team *t);
int team_get_draws(const Team *t);
int team_get_losses(const Team *t);
int team_get_goals_for(const Team *t);
int team_get_goals_against(const Team *t);

void team_register_result(Team *t, int goals_for, int goals_against);


int team_goal_diff(const Team *t);
int team_points(const Team *t);


int team_rank_compare(const Team *a, const Team *b);

#endif
