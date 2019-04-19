HMW 6:

This project is an extension of the Virtual memory manager described in Chapter 9 of your book (p458/461). The memory manager application is to be implemented in C and run on the VM that you built in HMW 1.

You will modify the size of the physical memory to 128 frames which is smaller than the virtual memory of 256 pages. This will require implementing a replacement policy. The replacement policy that you will implement is the second chance LRU approximation which is described in section 9.4.5.2. All other parameters and configuration remain the same including the TLB and the size of the TLB. For the TLB replacement, you can re-use the replacement policy that you implemented in HMW 5.

Please note that you are not required to handle evicted pages. However, you need to update the content of the physical memory with the new pages that are read from the backing_store.

(8 pts) implementation of the second chance replacement algorithm

(4 pts) reporting statistics

(4 pts) overall working program for the test file

(4 pts) report. Make sure you compare your previous statistics with the ones obtained in this implementation.