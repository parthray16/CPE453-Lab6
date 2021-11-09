#ifndef block
#define block

typedef struct Block
{
    char* process;
    unsigned int start;
    unsigned int size;
    struct Block* next;

}Block;


#endif