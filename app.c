#include <stdio.h>
#include <stdlib.h>


/* *****************
 * DATA STRUCTURES *
 ***************** */


typedef struct {
    char c;
    float s;
    float p;
} FR_R;


typedef struct {
    int q_c;  /* how many in Q */
    char* q_k;  /* keys of Q */
    int* q_v;  /* values of Q */
    int t;
    float w;
} M;


typedef struct {
    FR_R** fr_r;  /* null terminated array of resources */
    M* m; /* pointer to M */

    // useful constants
    int fr_r_size;
} PROBLEM;


int fr_r_length(FR_R** fr_r) {
    if (fr_r == NULL) {
        return 0;
    }

    int i = 0;
    for (i = 0; fr_r[i] != NULL; i++) {

    }
    return i;
}


FR_R** fr_r_append(FR_R** inlet, FR_R* thing) {
    int size = fr_r_length(inlet);
    FR_R** outlet = (FR_R**) malloc((2+size) * sizeof(FR_R*));
    int i;

    for (i = 0; i < size; i++) {
        outlet[i] = inlet[i];
    }
    outlet[size] = thing;
    outlet[size+1] = NULL;

    return outlet;
}


void fr_r_free(FR_R** fr_r) {
    free(fr_r);
    fr_r = NULL;
}


/* *****
 * I/O *
 ***** */


PROBLEM* load(char* input_file) {
    PROBLEM* problem = (PROBLEM*) malloc(sizeof(PROBLEM));
    M* m = (M*) malloc(sizeof(M));
    FR_R** fr_r = NULL;
    FILE* fp = fopen(input_file, "r");
    int i, j, d;
    float f;
    char c;

    fscanf(fp, "%c %d\n", &c, &d);
    while (c != 'X') {
        switch (c) {
            case 'Q':
                m->q_c = d;
                m->q_k = (char*) malloc(sizeof(m->q_c));
                m->q_v = (int*) malloc(sizeof(m->q_c));
                for (i = 0; i < m->q_c; i++) {
                    fscanf(fp, "%c %d\n", &c, &d);
                    m->q_k[i] = c;
                    m->q_v[i] = d;
                }
            break;

            case 'T':
                m->t = d;
            break;

            case 'W':
                m->w = d;
            break;

            case 'F':
                fr_r = (FR_R**) malloc((d+1)*sizeof(FR_R*));
                fr_r[d] = NULL;
                problem->fr_r_size = d;
                for (i = 0; i < d; i++) {
                    fr_r[i] = (FR_R*) malloc(sizeof(FR_R));
                    for (j = 0; j < 3; j++) {
                        fscanf(fp, "%c ", &c);
                        if (c == 'C') {
                            fscanf(fp, "%c\n", &c);
                            fr_r[i]->c = c;
                        } else if (c == 'S') {
                            fscanf(fp, "%f\n", &f);
                            fr_r[i]->s = f;
                        } else if (c == 'P') {
                            fscanf(fp, "%f\n", &f);
                            fr_r[i]->p = f;
                        }
                    }
                }
            break;
        }
        fscanf(fp, "%c %d\n", &c, &d);
    }
    fclose(fp);

    problem->fr_r = fr_r;
    problem->m = m;

    return problem;
}


void debug_fr_r(FR_R** fr_r) {
    int i;

    if (fr_r == NULL) {
        printf("FR_R: null\n");
    } else {
        printf("FR_R:\n");
        for (i = 0; fr_r[i] != NULL; i++) {
            printf("  C: %c\n", fr_r[i]->c);
            printf("  S: %.2f\n", fr_r[i]->s);
            printf("  P: %.2f\n", fr_r[i]->p);
        }
    }
}


void debug_problem(PROBLEM* problem) {
    int i;

    printf("---\n");
    printf("M:\n");
    printf("  W: %.2f\n", problem->m->w);
    printf("  T: %d\n", problem->m->t);
    printf("  Q:\n");
    for (i = 0; i < problem->m->q_c; i++) {
        printf("    %c: %d\n", problem->m->q_k[i], problem->m->q_v[i]);
    }
    debug_fr_r(problem->fr_r);
    printf("...\n");
}


void debug_solution(FR_R** solution) {
    printf("--- # chosen P\n");
    debug_fr_r(solution);
    printf("...\n");
}


/* **************
 * BACKTRACKING *
 ************** */


int validate(M* m, FR_R** p) {
    int is_q = 1;
    int is_t = 1;
    int is_w = 1;
    int i, j;
    float price;
    int count;

    if (p == NULL) {
        return 0;
    }

    is_q = 1;
    for (j = 0; j < m->q_c; j++) {
        count = 0;
        price = 0;
        for (i = 0; p[i] != NULL; i++) {
            price += p[i]->p;
            count += (p[i]->c == m->q_k[j])? 1 : 0;
        }
        is_q = is_q && (count == m->q_v[j]);
    }
    is_t = i == m->t;
    is_w = price <= m->w;


    return is_q && is_t && is_w;
}


float score(M* m, FR_R** p) {
    float result = -1/.0;
    int i = 0;

    if (validate(m, p)) {
        result = 0;
        for (i = 0; p[i] != NULL; i++) {
            result += p[i]->s;
        }
    }

    return result;
}



FR_R** solve(PROBLEM* problem, FR_R** p, int i) {
    if (i == problem->fr_r_size) {
        return p;
    }

    FR_R** result = NULL;
    FR_R** p_1 = solve(problem, fr_r_append(p, problem->fr_r[i]), i+1);
    FR_R** p_2 = solve(problem, p, i+1);
    float s_1 = score(problem->m, p_1);
    float s_2 = score(problem->m, p_2);

    if (s_1 > s_2) {
        result = p_1;
        fr_r_free(p_2);
    } else {
        result = p_2;
        fr_r_free(p_1);
    }

    return result;
}


/* ****************
 * MAIN FUNCTIONS *
 **************** */


int main(int argc, char *argv[]) {
    // read
    PROBLEM *problem = load(argv[1]);
    debug_problem(problem);

    // evaluate
    FR_R** solution = solve(problem, NULL, 0);

    // print
    debug_solution(solution);

    return 0;
}
