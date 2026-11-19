#include <stdio.h>
#include <string.h>
#include "team_list.h"
#include "match_list.h"

#define TEAMS_FILE "times.csv"
#define MATCHES_FILE "partidas.csv"
#define OUTPUT_FILE "classificacao.csv"
#define LINE_MAX_LEN 256
#define NAME_COL_WIDTH 15

static int next_line(FILE *f, char *buf, int size, int first) {

    while (fgets(buf, size, f)) {

        if (*first) {

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
        return NULL/


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


    }

}