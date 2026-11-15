#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int* data;
    int top, size;
}stack;

static inline stack* new_stack(){
    stack* stk = (stack*) malloc(sizeof(stack));
    if(!stk) exit(1);
    stk->size = 32;
    stk->data =(int*) malloc(stk->size * sizeof(int));
    if(!stk->data) exit(1);
    stk->top = -1;
    return stk;
} 

static inline void stk_push(stack* stk, int value){
    if(!stk) return;
    if(stk->top >= stk->size - 1){
        int* tmp = (int*) realloc(stk->data, 2 * stk->size * sizeof(int));
        if(!tmp) exit(1);
        stk->size *= 2;
        stk->data = tmp;
    }
    stk->data[++stk->top] = value;
}

static inline int stk_pop(stack* stk){
    return stk->data[stk->top--];
}

static inline int stk_top(stack* stk){
    return stk->data[stk->top];
}

static inline int stk_isempty(stack* stk){
    if(!stk) return -1;
    return (stk->top == -1);
}

static inline void free_stk(stack **stk){
    if(!stk || !*stk) return;

    free((*stk)->data);
    free(*stk);
    *stk = NULL;
}

#endif
