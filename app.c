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
} PROBLEM;


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
    printf("FR_R:\n");
    for (i = 0; problem->fr_r[i] != NULL; i++) {
        printf("  C: %c\n", problem->fr_r[i]->c);
        printf("  S: %.2f\n", problem->fr_r[i]->s);
        printf("  P: %.2f\n", problem->fr_r[i]->p);
    }
    printf("...\n");
}


/* ****************
 * MAIN FUNCTIONS *
 **************** */


int main(int argc, char *argv[]) {
    // read
    PROBLEM *problem = load(argv[1]);
    debug_problem(problem);

    // evaluate
    // TODO evaluate program

    // print
    // TODO print results

    return 0;
}
