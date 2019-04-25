//
// Created by Josh Martin on 2019-04-10.
//

#ifndef HW5_VIRTUAL_MEMORY_H
#define HW5_VIRTUAL_MEMORY_H


#define FRAME_SIZE 256
#define TOTAL_NUMBER_OF_FRAMES 128
#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 256
#define ADDRESS_BUFFER_SIZE  10

#include <stdio.h>

typedef struct {
    int frame_number;
    int reference_bit;
}Page_table_item;

typedef struct {
    int pageNumber;
    int frameNumber;
} Translation_Lookaside_Buffer;

//typedef struct page_data {
//    int TableNumbers[PAGE_TABLE_SIZE];//holds the page numbers
//    int TableFrames[PAGE_TABLE_SIZE]; //holds the frame numbers

//} Page_data;
//
typedef struct TLB_data {
int hits;//counts TLB hits
    int entries;//counts the number of entries in the TLB
} TLB_data;

typedef struct data_struct {
    FILE *address_file;
    FILE *backing_store;
//    Page_data page;
    TLB_data TLB;
    int faults;// counts page faults

    Page_table_item pageTable[PAGE_TABLE_SIZE];
    Translation_Lookaside_Buffer TLB_table[TLB_SIZE];
    int frame_table[TOTAL_NUMBER_OF_FRAMES]; // TODO: Change name
    signed char physicalMemory[TOTAL_NUMBER_OF_FRAMES][FRAME_SIZE];
    int firstAvailableFrame; //tracks the first available frame
    int firstAvailablePageTableNumber; //tracks the first available page table entry
} memory;

memory * new_virtual_memory();

#endif //HW5_VIRTUAL_MEMORY_H
