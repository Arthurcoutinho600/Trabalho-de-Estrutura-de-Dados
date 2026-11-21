#include <stdio.h>
#include <string.h>
#include "team_list.h"
#include "match_list.h"

#define TEAMS_FILE "times.csv"
#define MATCHES_FILE "partidas.csv"
#define OUTPUT_FILE "classificacao.csv"
#define LINE_MAX_LEN 256
#define NAME_COL_WIDTH 15


/* ----- Carga -----*/

/*Lê um CSV linha a linha ignorando o cabeçalho e linhas em branco. */

static int next_line(FILE *f, char *buf, int size, int *first) {

    while (fgets(buf, size, f)) {

        if (*first) { /* descarta o cabeçalho (e um eventual de UTF-8)*/

        *first = 0;
        continue;

        }
     buf[strcspm(buf, "\r\n")] = '\0';

    if (buf[0] != '\0')
     return 1;

    }

    return 0;

}




static TeamList *load_teams(const char *path) {

    FILE *f = fopen(path, "r");
    if (!f) {
        
        fprintf(stderr, "Erro: não foi possível abrir %s\n", path);

        return NULL;
    }

    TeamList *list = team_list_create();

    if (!list) {

        fclose(f);
        return NULL;

    }
 char line[LINE_MAX_LEN], name[LINE_MAX_LEN];
 int first = 1, id;
 
 while (next_line(f, line, sizeof line, &first)) {

    if (sscanf(line, "%d,%255[^\n]", &id, name) != 2) {

        fprintf(stderr, "Aviso: linha de time inválida ignorada: %s\n", line);
        continue;

    }

    Team *t = team_create(id, name);

    if (!t || team_list_append(list, t) != 0) {

        team_destroy(t);
        fprintf(stderr, "Erro: memória insuficiente\n");
        team_list_destroy_with_teams(list);
        fclose(f);
        return NULL;


    }


 }

 fclose(f);
 return list;

}


static MatchList *load_matches(const char *path) {

    FILE *f = fopen(path, "r");
    if (!f) {

        fprintf(stderr, "Erro: não foi possivel abrir %s\n", path);
        return NULL;

    }

    MatchList *list = match_list_create();

    if (!list) {

        fclose(f);
        return NULL;

    }

    char line[LINE_MAX_LEN];

    int first = 1, id, t1, t2, g1, g2;

    while (next_line(f, line, sizeof line, &first)) {

        if (sscanf(line, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) != 5) {

            fprintf(stderr, "Aviso: linha de partida inválida ignorada: %s\n", line);
            continue;
        }

    Match *m = match_create(id, t1, t2, g1, g2);

    if (!m || match_list_append(list, m) != 0) {

        match_destroy(m);

        fprintf(stderr, "Erro: memória insuficiente\n");
        fclose(f);
        return NULL;

    }

    }

    fclose(f);
    return list;
    

}

/* ----------- Apuração ------------ */

/* Atualiza os dois times de cada partida. partidas com time inexistente 
* são ignoradas com aviso, para não acessar ponteiro nulo*/



static void tally(TeamList *teams, const MatchList *matches) {

    for (MatchNode *n = match_list_head(matches); n; n = match_node_next(n)) {

        const Match *m = match_node_match(n);

        Team *a = team_list_find(teams, match_get_team1(m));
        Team *b = team_list_find(teams, match_get_team2(m));

        if (!a || !b) {

            fprintf(stderr, "Aviso: partida %d com time inexistente ignorada\n", match_get_id(m));
            continue;

        }

        team_register_result(a, match_get_goals1(m), match_get_goals2(m));
        team_register_result(b, match_get_goals2(m), match_get_goals1(m));

    }

}


/* --------- Classificação -------------*/

/* Insere cada time, um a um, numa nova lista ordenada. A nova lista só 
* guarda ponteiros: quem libera os times é a lista original. */

static TeamList *rank(const TeamList *teams) {

    TeamList *sorted = team_list_create();
    if (!sorted)

    return NULL;

    for (TeamNode *n = team_list_head(teams); n; team_node_next(n)) {

        if(team_list_insert_sorted(sorted, team_node_team(n)) != 0) {

            team_list_destroy(sorted);
            return NULL;

        }

    }

    return sorted;

}


/* ---------- Relatório -----------------*/

/*printf("%-15s") conta bytes; em UTF-8 isso desalinha acentos. Aqui o 
* preenchimento é calculado em caracteres (bytes de continuação 10xxxxxxx) 
* não contam).*/


static void print_padded(FILE *out, const char *s, int width) {

    int chars = 0;

    for (const char *p = s; *p; p++)

    if(((unsigned char) *p & 0xC0) != 0x80) chars++;

    fputs(s, out);

    for (int i = chars; i < width; i++)

    fputc(' ', out);


}


static void print_table(const TeamList * sorted) {

    printf("Pos ID %-*s V E D GM GS S PG\n", NAME_COL_WIDTH, "Time");
    int pos = 1;

    for (TeamNode *n = team_list_head(sorted); n; n = team_node_next(n), pos++) {

        const Team *t = team_node_team(n);
        printf("%3d %2d   ", pos, team_get_id(t));
        print_padded(stdout, team_get_name(t), NAME_COL_WIDTH);
        printf("%2d %2d %2d %3d %3d %3d\n", team_get_wins(t), team_get_draws(t), team_get_losses(t), team_get_goals_for(t), team_get_goals_against(t), team_goal_diff(t), team_points(t));



    }

}

static int save_table(const TeamList *sorted, const char *path) {

    FILE *f = fopen(path, "w");

    if (!f) {

        fprintf(stderr, "Erro: não foi possivel gravar %s\n", path);
        return -1;

    }
        fprintf(f, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n");
        int pos = 1;


        for (TeamNode *n = team_list_head(sorted); n; n = team_node_next(n), pos++) {

            const Team *t = team_node_team(n);
            fprintf(f, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n", pos, team_get_id(t), team_get_name(t), team_get_wins(t), team_get_draws(t), team_get_losses(t), team_get_goals_for(t), team_get_goals_against(t), team_goal_diff(t), team_points(t));

        }
        
    fclose(f);
    return 0;


}


static void print_summary(const TeamList *teams, const TeamList *sorted, const MatchList *matches) {

    int n_teams = team_list_size(teams);
    int total = n_teams * (n_teams - 1) / 2; /* turno unico, todos contra todos */
    int played = match_list_size(matches);

    printf("===== Resumo do Campeonato =====\n");
    printf("Partidas disputadas: %d de %d\n", played, total);

    if (played == 0) {

        printf("Nenhuma partida foi disputada ainda\n");
        return;

    }

    const Team *first = team_node_team(team_list_head(sorted));
    printf("%s: %s (%d pontos)\n", played == total ? "Campeão" : "Lider", team_get_name(first), team_points(first));

    /* Percorrendo a lista JÁ ordenada e usando comparação estrita, o 
    * primeiro time encontrado vence os empates (o mais bem classificado). */



    const Team *best_att = first, *best_def = first;

    for (TeamNode *n = team_list_head(sorted); n; n = team_node_next(n)) {

        const Team *t = team_node_team(n);
        if (team_get_goals_for(t) > team_get_goals_for(best_att))

        best_att = t;

        if (team_get_goals_against(t) < team_get_goals_against(best_def))
        best_def = t;

    }

    printf("Melhor ataque: %s (%d gols marcados)\n", team_get_name(best_att), team_get_goals_for(best_att));
    printf("Melhor defesa: %s (%d gols sofridos)\n", team_get_name(best_def), team_get_goals_against(best_def));


    /* Estrito (>) mantém a primeira partida do arquivo em caso de empate. */

    const Match *big = match_node_match(match_list_head(matches));

    for (MatchNode *n = match_list_head(matches); n; n = match_node_next(n))

        if (match_goal_margin(match_node_match(n)) > match_goal_margin(big))

            big = match_node_match(n);

    const Team *a = team_list_find(teams, match_get_team1(big));
    const Team *b = team_list_find(teams, match_get_team2(big));

    printf("Maior goleada: %s %d x %d %s\n", a ? team_get_name(a) : "?", match_get_goals1(big), match_get_goals2(big), b ? team_get_name(b) : "?");


}

int main(void) {

    TeamList *teams = load_teams(TEAMS_FILE);
    if (!teams)
    return 1;

    MatchList *matches = load_matches(MATCHES_FILE);

    if (!matches) {

        team_list_destroy_with_teams(teams);

        return 1;

    }


    tally(teams, matches);

    TeamList *sorted = rank(teams);
    int status = 0;

    if (!sorted || team_list_size(sorted) == 0) {

        fprintf(stderr, "Erro: nenhum time para classificar\n");
        status = 1;
    } else {

        print_table(sorted);
        print_summary(teams, sorted, matches);

        if (save_table(sorted, OUTPUT_FILE) != 0)

        status = 1;

    }

    /* A lista ordenada só tem nós; os times são liberados uma única vez, 
    * pela lista original. */

    if (sorted)
    team_list_destroy(sorted);
    team_list_destroy_with_teams(teams);
    match_list_destroy(matches);
    return status;

}