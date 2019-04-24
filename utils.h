//
// Created by Josh Martin on 2019-04-10.
//

#ifndef HW5_UTILS_H
#define HW5_UTILS_H

#include <stdio.h>
#include "virtual_memory.h"
void setIntoTLB(memory *_this, int pageNumber, int frameNumber);
void getStore(memory *_this, int pageNumber);
#endif //HW5_UTILS_H
