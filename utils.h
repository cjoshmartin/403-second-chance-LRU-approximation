//
// Created by Josh Martin on 2019-04-10.
//

#ifndef HW5_UTILS_H
#define HW5_UTILS_H

#include <stdio.h>
#include "defines.h"
#include "virtual_memory.h"
void setIntoTLB(virtual_memory *_this, int pageNumber, int frameNumber);
void getStore(virtual_memory *_this, int pageNumber);
#endif //HW5_UTILS_H
