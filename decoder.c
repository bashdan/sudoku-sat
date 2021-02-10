// SAT to sudoku decoder
// by Daniel Brashaw

// usage: ./decoder <minisat output>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ       8192

int main(int argc, char *argv[]) {

    FILE *fp = NULL;
    if (argc == 1) {
        fprintf(stderr, "usage: decoder <filename>\n");
        exit(-1);
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "error opening %s\n", argv[1]);
        }
    }
    else {
        fprintf(stderr, "unknown usage error\n");
        exit(-1);
    }

    char buffer[BUFFER_SZ], bufcopy[BUFFER_SZ];
    memset(buffer, 0, BUFFER_SZ);
    buffer[BUFFER_SZ-1] = '\0';
    int board[9][9];
    memset(board, 0, 9*9*sizeof(int));

    while (fgets(buffer, BUFFER_SZ, fp) != NULL) {
        if (!strncmp("SAT", buffer, 3)) {
            puts("Satisfied\n");
        }
        else if (!strncmp("UNSAT", buffer, 5)){
            puts("Unsatisfied\n");
            fclose(fp);
            exit(1);
        }
        else {
            char *token = strtok(buffer, " ");
            int raw, r, c, v;
            while (token != NULL) {
                raw = atoi(token);
                if (abs(raw) >= 111 && raw > 0) {
                    v = raw % 10;
                    c = (raw % 100) / 10;
                    r = raw / 100;
                    board[r-1][c-1] = v;
                }
                token = strtok(NULL, " ");
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}