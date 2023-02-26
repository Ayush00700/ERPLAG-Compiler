#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 100
#define MAX_SYMBOLS 100

int num_rules;
char *rules[MAX_RULES][MAX_SYMBOLS];
int num_non_terminals;
char *non_terminals[MAX_SYMBOLS];
int first_sets[MAX_SYMBOLS][MAX_SYMBOLS];
int follow_sets[MAX_SYMBOLS][MAX_SYMBOLS];

void find_first_set(int nt_idx) {
    int i, j, k;
    for (i = 0; i < num_rules; i++) {
        if (strcmp(rules[i][0], non_terminals[nt_idx]) == 0) {
            j = 2;
            while (j < MAX_SYMBOLS && rules[i][j] != NULL) {
                if (rules[i][j][0] >= 'a' && rules[i][j][0] <= 'z') {
                    first_sets[nt_idx][rules[i][j][0]-'a'] = 1;
                    break;
                } else {
                    for (k = 0; k < num_non_terminals; k++) {
                        if (strcmp(rules[i][j], non_terminals[k]) == 0) {
                            memcpy(first_sets[nt_idx], first_sets[k], MAX_SYMBOLS*sizeof(int));
                            if (rules[i][j+1] == NULL) {
                                follow_sets[k][nt_idx] = 1;
                            } else {
                                int l = j+1;
                                while (l < MAX_SYMBOLS && rules[i][l] != NULL) {
                                    if (rules[i][l][0] >= 'a' && rules[i][l][0] <= 'z') {
                                        follow_sets[k][rules[i][l][0]-'a'] = 1;
                                        break;
                                    } else {
                                        int m;
                                        for (m = 0; m < num_non_terminals; m++) {
                                            if (strcmp(rules[i][l], non_terminals[m]) == 0) {
                                                memcpy(follow_sets[k], first_sets[m], MAX_SYMBOLS*sizeof(int));
                                                if (first_sets[m][0] == 1) {
                                                    l++;
                                                } else {
                                                    break;
                                                }
                                            }
                                        }
                                        if (m == num_non_terminals) {
                                            memcpy(follow_sets[k], first_sets[nt_idx], MAX_SYMBOLS*sizeof(int));
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if (k == num_non_terminals) {
                        break;
                    }
                }
            }
            if (j == MAX_SYMBOLS || rules[i][j] == NULL) {
                memcpy(first_sets[nt_idx], first_sets[k], MAX_SYMBOLS*sizeof(int));
            }
        }
    }
}

void find_follow_sets() {
    int i, j;
    follow_sets[0][strlen(non_terminals[0])] = 1;
    for (i = 0; i < num_non_terminals; i++) {
        for (j = 0; j < num_rules; j++) {
            int k;
            for (k = 2; k < MAX_SYMBOLS && rules[j][k] != NULL; k++) {
                if (strcmp(rules[j][k], non_terminals[i]) == 0) {
                    if (rules[j][k+1] == NULL) {
                        memcpy(follow_sets[i], follow_sets[j], MAX_SYMBOLS*sizeof(int));
                    } else {
                        int l = k+1;
                        while (l < MAX_SYMBOLS && rules[j][l] != NULL) {
                            if (rules[j][l][0] >= 'a' && rules[j][l][0] <= 'z') {
                                follow_sets[i][rules[j][l][0]-'a'] = 1;
                                break;
                            } else {
                                int m;
                                for (m = 0; m < num_non_terminals; m++) {
                                    if (strcmp(rules[j][l], non_terminals[m]) == 0) {
                                        memcpy(follow_sets[i], first_sets[m], MAX_SYMBOLS*sizeof(int));
                                        if (first_sets[m][0] == 1) {
                                            l++;
                                        } else {
                                            break;
                                        }
                                    }
                                }
                                if (m == num_non_terminals) {
                                    memcpy(follow_sets[i], follow_sets[j], MAX_SYMBOLS*sizeof(int));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // Initialize the grammar
    num_rules = 3;
    rules[0][0] = "S";
    rules[0][1] = "->";
    rules[0][2] = "A";
    rules[0][3] = "B";
    rules[1][0] = "A";
    rules[1][1] = "->";
    rules[1][2] = "a";
    rules[2][0] = "B";
    rules[2][1] = "->";
    rules[2][2] = "b";
    non_terminals[0] = "S";
    non_terminals[1] = "A";
    non_terminals[2] = "B";
    num_non_terminals = 3;
    memset(first_sets, 0, sizeof(first_sets));
    memset(follow_sets, 0, sizeof(follow_sets));

    // Compute the first sets
    int i;
    for (i = 0; i < num_non_terminals; i++) {
        find_first_set(i);
    }

    // Compute the follow sets
    follow_sets[0][strlen(non_terminals[0])] = 1;
    for (i = 0; i < num_non_terminals; i++) {
        find_follow_sets(i);
    }

    // Print the results
    printf("Non-terminals: ");
    for (i = 0; i < num_non_terminals; i++) {
        printf("%s ", non_terminals[i]);
    }
    printf("\n");

    printf("Terminals: ");
    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (is_terminal(i)) {
            printf("%c ", i + 'a');
        }
    }
    printf("\n");

    printf("First sets:\n");
    for (i = 0; i < num_non_terminals; i++) {
        printf("%s: ", non_terminals[i]);
        print_set(first_sets[i]);
        printf("\n");
    }

    printf("Follow sets:\n");
    for (i = 0; i < num_non_terminals; i++) {
        printf("%s: ", non_terminals[i]);
        print_set(follow_sets[i]);
        printf("\n");
    }

    return 0;
}
