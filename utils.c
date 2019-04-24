//
// Created by Josh Martin on 2019-04-10.
//

#include <string.h>
#include "utils.h"

void FIFO_algorthim(memory *_this, int pageNumber, int frameNumber) {

    int i;  // if it's already in the TLB, break
    for(i = 0; i < _this->TLB.entries; i++){
        if(_this->TLB.pageNumber[i] == pageNumber){
            break;
        }
    }

    if(i == _this->TLB.entries){
        if(_this->TLB.entries < TLB_SIZE){
            // insert the page and frame on the end
            _this->TLB.pageNumber[_this->TLB.entries] = pageNumber;
            _this->TLB.frameNumber[_this->TLB.entries] = frameNumber;
        }
        else{// otherwise shift everything
            for(i = 0; i < TLB_SIZE - 1; i++){
                _this->TLB.pageNumber[i] = _this->TLB.pageNumber[i + 1];
                _this->TLB.frameNumber[i] = _this->TLB.frameNumber[i + 1];
            }
            _this->TLB.pageNumber[_this->TLB.entries-1] = pageNumber;
            _this->TLB.frameNumber[_this->TLB.entries-1] = frameNumber;
        }
    }

    else{ // index is not <==> to # of entries
        for(; i < _this->TLB.entries - 1; i++){
            _this->TLB.pageNumber[i] = _this->TLB.pageNumber[i + 1];
            _this->TLB.frameNumber[i] = _this->TLB.frameNumber[i + 1];
        }
        if(_this->TLB.entries < TLB_SIZE){// if there is room, put @ end
            _this->TLB.pageNumber[_this->TLB.entries] = pageNumber;
            _this->TLB.frameNumber[_this->TLB.entries] = frameNumber;
        }
        else{// put the page and frame @  (entries - 1)
            _this->TLB.pageNumber[_this->TLB.entries-1] = pageNumber;
            _this->TLB.frameNumber[_this->TLB.entries-1] = frameNumber;
        }
    }
}

void setIntoTLB(memory *_this, int pageNumber, int frameNumber) {

    FIFO_algorthim(_this, pageNumber, frameNumber);

    if(_this->TLB.entries < TLB_SIZE){
        _this->TLB.entries++;
    }
}

#define BUFFER_SIZE  FRAME_SIZE // number of bytes to read

void getStore(memory *_this, int pageNumber) {
    signed char buffer[BUFFER_SIZE];

    if (fseek(_this->backing_store, pageNumber * BUFFER_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking in backing store\n");
    }

    // now read BUFFER_SIZE bytes from the backing store to the buffer
    if (fread(buffer, sizeof(signed char), BUFFER_SIZE, _this->backing_store) == 0) {
        fprintf(stderr, "Error reading from backing store\n");
    }

    // load the bits into the first available frame in the physical memory 2D array
    for(int i = 0; i < BUFFER_SIZE; i++){
        _this->physicalMemory[_this->firstAvailableFrame][i] = buffer[i];
    }

    // load the frame number into the page table in the first available frame
    _this->page.TableNumbers[_this->firstAvailablePageTableNumber] = pageNumber;
    _this->page.TableFrames[_this->firstAvailablePageTableNumber] = _this->firstAvailableFrame;

    //track the next available frames
    _this->firstAvailableFrame++;
    _this->firstAvailablePageTableNumber++;
}
