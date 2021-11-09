#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

#define MAX_LINE 80
#define FREE "P0"

int max;
Block* memory = NULL;

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

void first_fit(char* process, unsigned int size){
    Block* temp = memory;
    Block* newBlock = NULL;
    Block* prev = NULL;

    while(temp != NULL){
        if (strcmp(temp->process, FREE) == 0 && temp->size > size){
            newBlock = (Block *)malloc(sizeof(Block));
            newBlock->process = strdup(process);
            newBlock->start = temp->start;
            newBlock->size = size;
            newBlock->next = temp;
            temp->start += size + 1;
            temp->size -= size;
            if (prev == NULL){
                memory = newBlock;
                return;
            }
            prev->next = newBlock;
            return;
        }
        else if (strcmp(temp->process, FREE) == 0 && temp->size == size){
            temp->process = strdup(process);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL){
        fprintf(stderr, "rq: No space.\n");
        return;
    }
}

void best_fit(char* process, unsigned int size){
    Block* temp = memory;
    Block* newBlock = NULL;
    Block* prev = NULL;
    Block* bestHolePrev = NULL;
    Block* bestHole = NULL; 

    while(temp != NULL){
        if (strcmp(temp->process, FREE) == 0 && temp->size > size){
            if (bestHole == NULL){
                bestHole = temp;
                bestHolePrev = prev;
            }
            else{
                if (temp->size < bestHole->size){
                    bestHole = temp;
                    bestHolePrev = prev;
                }
            }
        }
        else if (strcmp(temp->process, FREE) == 0 && temp->size == size){
            temp->process = strdup(process);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    if (bestHole == NULL){
        fprintf(stderr, "rq: No space.\n");
        return;
    }
    newBlock = (Block *)malloc(sizeof(Block));
    newBlock->process = strdup(process);
    newBlock->start = bestHole->start;
    newBlock->size = size;
    newBlock->next = bestHole;
    bestHole->start += size + 1;
    bestHole->size -= size;
    if (bestHolePrev == NULL){
        memory = newBlock;
        return;
    }
    bestHolePrev->next = newBlock;
    return;
}


void worst_fit(char* process, unsigned int size){
    Block* temp = memory;
    Block* newBlock = NULL;
    Block* prev = NULL;
    Block* worstHolePrev = NULL;
    Block* worstHole = NULL; 

    while(temp != NULL){
        if (strcmp(temp->process, FREE) == 0 && temp->size > size){
            if (worstHole == NULL){
                worstHole = temp;
                worstHolePrev = prev;
            }
            else{
                if (temp->size > worstHole->size){
                    worstHole = temp;
                    worstHolePrev = prev;
                }
            }
        }
        else if (strcmp(temp->process, FREE) == 0 && temp->size == size){
            temp->process = strdup(process);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    if (worstHole == NULL){
        fprintf(stderr, "rq: No space.\n");
        return;
    }
    newBlock = (Block *)malloc(sizeof(Block));
    newBlock->process = strdup(process);
    newBlock->start = worstHole->start;
    newBlock->size = size;
    newBlock->next = worstHole;
    worstHole->start += size + 1;
    worstHole->size -= size;
    if (worstHolePrev == NULL){
        memory = newBlock;
        return;
    }
    worstHolePrev->next = newBlock;
    return;
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
    if (strcmp(args[3], "F") == 0){
        first_fit(args[1], atoi(args[2]));
    }
    else if (strcmp(args[3], "B") == 0){
        best_fit(args[1], atoi(args[2]));
    }
    else if (strcmp(args[3], "W") == 0){
        worst_fit(args[1], atoi(args[2]));
    }
    else{
        fprintf(stderr, "rq: takes only F, B, or W as 3rd arg\n");
        return;
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
    Block *compact = (Block *)malloc(sizeof(Block));
    compact->process = FREE;
    compact->start = 0;
    compact->size = 0;
    compact->next = 0;
    Block *prev = NULL;
    Block *temp = memory;
    Block *next_block = NULL;
    int cur_start = 0;
    while(temp != NULL){
        if(strcmp(temp->process, FREE) == 0){
            compact->size += temp->size;
            next_block = temp->next;
            free(temp);
            if(prev != NULL){
                prev->next = next_block;
            }
            else{
                memory = next_block;
            }
            temp = next_block;
        }
        else{
            temp->start = cur_start;
            cur_start += temp->size + 1;
        }

    }
    if(prev == NULL){
        memory = compact;
        compact->start = cur_start;
        return;
    }
    prev->next = compact;
    compact->start = cur_start;
}

void stat(){
    Block* temp = memory;
    while(temp != NULL){
        if (strcmp(temp->process, FREE) == 0){
            printf("Addresses [%d:%d] Unused\n", temp->start, temp->start + temp->size);
        }
        else{
            printf("Addresses [%d:%d] Process %s\n", temp->start, temp->start + temp->size, temp->process);
        }
        temp = temp->next;
    }
    return;
}


int main(int argc, char* argv[]){
    int should_run = 1;
    char input[MAX_LINE];
    char inputcpy[MAX_LINE];
    char *token;

    if (argc != 2){
        fprintf(stderr, "usage: ./allocator [size]\n");
        return -1;
    }
    /* alloc mem */
    max = atoi(argv[1]);
    memory = (Block *)malloc(sizeof(Block));
    memory->process = FREE;
    memory->start = 0;
    memory->size = max;
    memory->next = NULL;

    while(should_run){
        printf("allocator> ");
        fflush(stdout);
        /* check for long line */
        if (readline(input) == 1){
            continue;
        }
        strcpy(inputcpy, input);
        token = strtok(inputcpy, " ");
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