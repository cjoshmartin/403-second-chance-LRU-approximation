//
// Created by Josh Martin on 2019-04-25.
//
#include "virtual_memory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
memory * new_virtual_memory(){

    memory * self =(memory *)(malloc(sizeof(memory)));
    int i = 0;
    for(i = 0; i < TOTAL_NUMBER_OF_FRAMES; i++)
        self->frame_table[i] = -1;

    for (i =0; i < PAGE_TABLE_SIZE; i++ )
        self->pageTable[i].frame_number = -1 ;

    for (i = 0; i < TLB_SIZE; i++)
        self->TLB_table[i].frameNumber = -1;

    return self;
}