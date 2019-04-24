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


typedef struct page_data {
    int TableNumbers[PAGE_TABLE_SIZE];//holds the page numbers
    int TableFrames[PAGE_TABLE_SIZE]; //holds the frame numbers
    int faults;// counts page faults
} Page_data;

typedef struct TLB_data {
    int pageNumber[TLB_SIZE];
    int frameNumber[TLB_SIZE];
    int hits;//counts TLB hits
    int entries;//counts the number of entries in the TLB
} TLB_data;

typedef struct data_struct {
    FILE *address_file;
    FILE *backing_store;
    Page_data page;
    TLB_data TLB;
    int physicalMemory[TOTAL_NUMBER_OF_FRAMES][FRAME_SIZE];
    int firstAvailableFrame; //tracks the first available frame
    int firstAvailablePageTableNumber; //tracks the first available page table entry
} memory;

#endif //HW5_VIRTUAL_MEMORY_H
