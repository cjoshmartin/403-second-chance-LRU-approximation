//
// Created by Josh Martin on 2019-04-10.
//

#include <string.h>
#include "utils.h"

void FIFO_algorthim(memory *_this, int pageNumber, int frameNumber) {

    int i;  // if it's already in the TLB, break
    for(i = 0; i < _this->TLB.entries; i++){
        if(_this->TLB_table[i].pageNumber == pageNumber){
            break;
        }
    }

    if(i == _this->TLB.entries){
        if(_this->TLB.entries < TLB_SIZE){
            // insert the page and frame on the end
            _this->TLB_table[_this->TLB.entries].pageNumber = pageNumber;
            _this->TLB_table[_this->TLB.entries].frameNumber = frameNumber;
        }
        else{// otherwise shift everything
            for(i = 0; i < TLB_SIZE - 1; i++){
                _this->TLB_table[i].pageNumber = _this->TLB_table[i + 1].pageNumber;
                _this->TLB_table[i].frameNumber = _this->TLB_table[i +1].frameNumber;
            }
            _this->TLB_table[_this->TLB.entries-1].pageNumber = pageNumber;
            _this->TLB_table[_this->TLB.entries -1 ].frameNumber = frameNumber;
        }
    }
    else{ // index is not <==> to # of entries
        for(; i < _this->TLB.entries - 1; i++){
            _this->TLB_table[i].pageNumber = _this->TLB_table[i+1].pageNumber;
            _this->TLB_table[i].frameNumber = _this->TLB_table[i+1].frameNumber;
        }
//        if(_this->TLB.entries < TLB_SIZE){// if there is room, put @ end
//            _this->TLB.pageNumber[_this->TLB.entries] = pageNumber;
//            _this->TLB.frameNumber[_this->TLB.entries] = frameNumber;
//        }
//        else{// put the page and frame @  (entries - 1)
//            _this->TLB.pageNumber[_this->TLB.entries-1] = pageNumber;
//            _this->TLB.frameNumber[_this->TLB.entries-1] = frameNumber;
//        }
    }
}

void setIntoTLB(memory *_this, int pageNumber, int frameNumber) {

    FIFO_algorthim(_this, pageNumber, frameNumber);

    if(_this->TLB.entries < TLB_SIZE){
        _this->TLB.entries++;
    }
}

#define BUFFER_SIZE  FRAME_SIZE // number of bytes to read

void getStore(memory *_this, int pageNumber, int frame_number) {

    if (fseek(_this->backing_store, pageNumber * BUFFER_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking in backing store\n");
    }

    // load the bits into the first available frame in the physical memory 2D array
    if (fread(_this->physicalMemory[frame_number], sizeof(signed char), BUFFER_SIZE, _this->backing_store) == 0) {
        fprintf(stderr, "Error reading from backing store\n");
    }

    _this->TLB_table[_this->TLB.entries].frameNumber = frame_number;
    _this->TLB_table[_this->TLB.entries].pageNumber = pageNumber;

    _this->TLB.entries++;
    _this->TLB.entries %= TLB_SIZE;
}
