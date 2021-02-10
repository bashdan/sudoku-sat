// sudoku to SAT encoder
// by Daniel Brashaw

// usage: ./encoder <sudokuN.txt>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SZ       80
#define OUTPUT_FNAME    "output.cnf"

int main(int argc, char *argv[]) {

    FILE *fp = NULL;
    if (argc == 1) {
        fprintf(stderr, "usage: ./encoder <sudoku.txt>\n");
        exit(1);
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "error opening %s\n", argv[1]);
            exit(-1);
        }
    }
    else {
        fprintf(stderr, "unknown opening error\n");
    }

    char buffer[BUFFER_SZ], *token = NULL;
    buffer[BUFFER_SZ-1] = '\0';

    int row  = 0, col, v;
    int board[9][9];
    memset(board, 0, 9*9*sizeof(int));

    // Read the sudoku file into board
    while (fgets(buffer, BUFFER_SZ, fp) != NULL) {

        token = strtok(buffer, " ");
        col = 0;
        while (token != NULL) {
            v = atoi(token);
            board[row][col] = v;
            col++;
            if (col == 9) {
                row++;
            }
            token = strtok(NULL, " ");
        }
    } // end reading
    fclose(fp);

/*
    char outputfname[strlen(argv[1])];
    strcpy(outputfname, argv[1]);
    for (int i = 0; i < BUFFER_SZ; i++) {
        if (outputfname[i] == '.') {
            outputfname[i+1] = 'c';
            outputfname[i+2] = 'n';
            outputfname[i+3] = 'f';
            break;
        }
    }
*/
    // Independent name, change to uncommented to base
    // the output name on the input
    char outputfname[BUFFER_SZ];
    strcpy(outputfname, OUTPUT_FNAME);

    FILE *fo = fopen(outputfname, "w");
    if (fo == NULL) {
        fprintf(stderr, "error opening %s for writing\n", outputfname);
        exit(-1);
    }
    for (int i = 0; i < 20; i++) {
        fprintf(fo, " ");
    }
    fprintf(fo, "\n");

    unsigned int nclauses = 0;
    // write the rules for sudoku (generality)
    // at least one number per cell
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k <= 9; k++) {
                fprintf(fo, "%d%d%d ", i, j, k);
            }
            fprintf(fo, "0\n");
            nclauses++;
        }
    }
    // at most one number per cell
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k < 9; k++) {
                for (int m = k+1; m <= 9; m++) {
                    fprintf(fo, "-%d%d%d -%d%d%d 0\n", i, j, k, i, j, m);
                    nclauses++;
                }
            }
        }
    }
    // at least once in a row
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k <= 9; k++) {
                fprintf(fo, "%d%d%d ", i, k, j);
            }
            fprintf(fo, "0\n");
            nclauses++;
        }
    }
    // at most one number per row
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k < 9; k++) {
                for (int m = k+1; m <= 9; m++) {
                    fprintf(fo, "-%d%d%d -%d%d%d 0\n", i, k, j, i, m, j);
                    nclauses++;
                }
            }
        }
    }
    // at least once in a col
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k <= 9; k++) {
                fprintf(fo, "%d%d%d ", k, i, j);
            }
            fprintf(fo, "0\n");
            nclauses++;
        }
    }
    // at most one number per col
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            for (int k = 1; k < 9; k++) {
                for (int m = k+1; m <= 9; m++) {
                    fprintf(fo, "-%d%d%d -%d%d%d 0\n", k, i, j, m, i, j);
                    nclauses++;
                }
            }
        }
    }
    // at least one number per block
    for (int v = 1; v <= 9; v++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int x = 1; x <= 3; x++) {
                    for (int y = 1; y <= 3; y++) {
                        fprintf(fo, "%d%d%d ", 3*i+x, 3*j+y, v);
                    }
                }
                fprintf(fo, "0\n");
                nclauses++;
            }
        }
    }
    // at most one number per block
    for (int i = 1; i <= 9; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                for (int x = 1; x <= 3; x++) {
                    for (int y = 1; y <= 3; y++) {
                        for (int z = y+1; z <= 3; z++) {
                            fprintf(fo, "-%d%d%d -%d%d%d 0\n", 3*j+x, 3*k+y, i, 3*j+x, 3*k+z, i);
                            nclauses++;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) {
                fprintf(fo, "%d%d%d 0\n", i+1, j+1, board[i][j]);
                nclauses++;
            }
        }
    }
    fseek(fo, 0, SEEK_SET);
    fprintf(fo, "p cnf 999 %u", nclauses);
    fclose(fo);
}