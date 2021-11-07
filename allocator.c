#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80

void *memory;
int max;

int readline(char input[MAX_LINE]){
    char line[MAX_LINE];
    int c;
    int i = 0;

    while (1){
        if (i == MAX_LINE){
            fprintf(stderr, "allocator: Line too long\n");
            return 1;
        }
        c = getchar();
        if (c == EOF || c == '\n'){
            line[i] = '\0';
            break;
        }
        else{
            line[i] = c;
        }
        i++;
    }
    strcpy(input, line);
    return 0;
}

void rq(char *input){
    char *token;
    int i = 0;
    char *args[MAX_LINE/2 + 1];
    
    token = strtok(input, " ");
    while (token != NULL){
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    
}

void rl(char *input){
    char *token;
    int i = 0;
    char *args[MAX_LINE/2 + 1];
    
    token = strtok(input, " ");
    while (token != NULL){
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }

}

void c(){

}

void stat(){

}


int main(int argc, char* argv[]){
    int should_run = 1;
    char input[MAX_LINE];
    char *token;

    if (argc != 2){
        fprintf(stderr, "usage: ./allocator [size]\n");
        return -1;
    }
    /* alloc mem */
    max = atoi(argv[1]);
    memory = malloc(max);

    while(should_run){
        printf("allocator> ");
        fflush(stdout);
        /* check for long line */
        if (readline(input) == 1){
            continue;
        }
        token = strtok(input, " ");
        if (token != NULL){
            if (strcmp(token, "RQ") == 0){
                rq(input);
            }
            else if (strcmp(token, "RL") == 0){
                rl(input);
            }
            else if (strcmp(token, "C") == 0){
                c();
            }
            else if (strcmp(token, "STAT") == 0){
                stat();
            }
            else if (strcmp(token, "X") == 0){
                should_run = 0;
            }
            else{
                fprintf(stderr, "commands: RQ, RL, C, STAT, X\n");
            }
        }
        else{
            fprintf(stderr, "commands: RQ, RL, C, STAT, X\n");
        }
    }
    return 1;
}