#ifndef TEAM_H
#define TEAM_H

/* TAD Team: um time do campeonato. A struct é opaca (definida em team.c). */

typedef struct Team Team;

/* Cria um time com estáticas zeradas. Retorna NULL se faltar memória*/

Team *team_create(int id, const char *name);
void team_destroy(Team *t);

int team_get_id(const Team *t);

const char *team_get_name(const Team *t);
int team_get_wins(const Team *t);
int team_get_draws(const Team *t);
int team_get_losses(const Team *t);
int team_get_goals_for(const Team *t);
int team_get_goals_against(const Team *t);

/* Registra o resultado de uma partida do ponto de vista de time. */

void team_register_result(Team *t, int goals_for, int goals_against);

/* Saldo e pontos são sempre calculados, nunca armazenados. */

int team_goal_diff(const Team *t);
int team_points(const Team *t);

/* Compara dois times pelos critérios de classificação (Seção 2.4). 
* Retorna < 0 se a deve ficar à frente de b, > 0 se b à frente de a. */

int team_rank_compare(const Team *a, const Team *b);

#endif
