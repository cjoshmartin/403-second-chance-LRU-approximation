#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <sys/errno.h>

#include "utils.h"
#include "virtual_memory.h"

int updateFramePointer(memory * _this, int frame_number){
    frame_number = _this->firstAvailableFrame++;
    _this->firstAvailableFrame %= TOTAL_NUMBER_OF_FRAMES;

    return frame_number;
}
int second_chance(memory * _this, int frame_number, int pageNumber){
    if(_this->frame_table[frame_number] != -1 ){

        int checkRefBit = _this->frame_table[frame_number];
        while (_this->pageTable[checkRefBit].reference_bit){ // while there is no one to pray on Mahaha
            _this->pageTable[checkRefBit].reference_bit = 0;

            frame_number = updateFramePointer(_this,frame_number);
            checkRefBit = _this->frame_table[checkRefBit];
        }

        // Evict from the LRU
        for (int i = 0; i < TLB_SIZE ; ++i)
            if(_this->TLB_table[i].pageNumber == checkRefBit)
                _this->TLB_table[i].pageNumber = -1;

        _this->pageTable[checkRefBit].frame_number = -1;
        _this->pageTable[checkRefBit].reference_bit = 0;
    }

    _this->frame_table[frame_number] = pageNumber;

    getStore(_this, pageNumber, 0); // gets data from .bin
    _this->pageTable[pageNumber].frame_number = -1;
    _this->pageTable->reference_bit = 0;

    return frame_number;

}

int find_frame(memory * _this, int pageNumber){
    int i,
    frame_number = - 1;

    for(i = 0; i < TLB_SIZE; i++){
        if(_this->TLB_table[i].pageNumber == pageNumber){
            frame_number = _this->TLB_table[i].frameNumber;
            _this->TLB.hits++;

            _this->pageTable[i].reference_bit = 1;
        }
    }

    if(frame_number == -1){ // frame_number not found
        frame_number = _this->pageTable[pageNumber].frame_number;

        if(frame_number == -1){// the page is not found in those contents
            _this->faults++;

            frame_number = updateFramePointer(_this, frame_number);
            frame_number= second_chance(_this, frame_number, pageNumber);


        }
        else{
            _this->pageTable[pageNumber].reference_bit = 1;

            _this->TLB_table[_this->firstAvailablePageTableNumber].frameNumber = frame_number;
            _this->TLB_table[_this->firstAvailablePageTableNumber].pageNumber = pageNumber;

            _this->firstAvailablePageTableNumber++;
            _this->firstAvailablePageTableNumber %= TLB_SIZE;

        }
    }
    return frame_number;
}

void getPage(
        memory * _this,
        int logical_address// reads in 32-bit numbers
        ){
    // Only concerned w/  RIGHTMOST 16-bit addresses of logical_address
    // logical_address is broken into 2, 8-bit segments:
    int pageNumber = ((logical_address & 0xFF00)>>8), // 8-bit page number
            offset = (logical_address & 0x00FF), // 8-bit page offset
            frame_number,
            physical_address;

     frame_number = find_frame(_this, pageNumber);

    setIntoTLB(_this, pageNumber, frame_number);
    physical_address = (frame_number << 8) | offset;
    printf("Virtual address: %d Physical address: %d Value: %d\n",
            logical_address,
            physical_address,
           _this->physicalMemory[frame_number][offset]
            );
}

FILE * openFile(char fileName[100], char read[3]){
    FILE * filPtr;
    if ((filPtr = fopen(fileName, read))  == NULL) {
        fprintf(stderr, "Error (%s): %s \n",fileName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return filPtr;
}

void print_stats(memory * _this, double total_addresses){

    // calculate and print out the stats
    printf("Number of translated addresses = %.0f\n", total_addresses);

    printf("Page Miss Rate: %.3f\n",_this->faults / total_addresses);
    printf("TLB Hit Rate: %.3f\n", _this->TLB.hits / total_addresses);

}

int main(int argc, char *argv[]) {

    double addresses_seen = 0.0;
    char address[ADDRESS_BUFFER_SIZE];
     memory * _this = new_virtual_memory();

    if (argc != 2) {
        fprintf(stderr,"Error: wrong number of Arguments passed");
        exit(EXIT_FAILURE);
    }
    
    _this->backing_store = openFile("BACKING_STORE.bin", "rb");
    _this->address_file = openFile(argv[1], "r");

    while (fgets(address, ADDRESS_BUFFER_SIZE, _this->address_file) != NULL ) {
        getPage(_this, atoi(address)); // get the physical address and value stored at that address
        addresses_seen++;  // increment the number of translated addresses
    }

    print_stats(_this, addresses_seen);
    fclose(_this->address_file);
    fclose(_this->backing_store);
    
    return 0;
}

