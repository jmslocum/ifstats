/*************************************************************************************
The ifstats library provides a C interface to the information provided in 
/proc/net/dev.

Copyright (C) 2012 James Slocum

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, 
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to the following 
conditions:

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************/

#ifndef IF_STATS_H
#define IF_STATS_H

#include <stdint.h>
#include <stdbool.h>

#define IFSTATS_NAME_LENGTH 64

typedef struct _ifstats_t {
   char           device[IFSTATS_NAME_LENGTH];
   uint64_t       rxBytes;
   uint64_t       rxPackets;
   uint64_t       rxErrors;
   uint64_t       rxDrop;
   uint64_t       rxFifo;
   uint64_t       rxFrame;
   uint64_t       rxCompressed;
   uint64_t       rxMulticast;
   uint64_t       txBytes;
   uint64_t       txPackets;
   uint64_t       txErrors;
   uint64_t       txDrop;
   uint64_t       txFifo;
   uint64_t       txCollisions;
   uint64_t       txCarrier;
   uint64_t       txCompressed;

   struct _ifstats_t* next;
} IFStats_t;

#ifdef __cplusplus
extern "C" {
#endif

IFStats_t* getIfStats();
void releaseStats(IFStats_t* stats);

#ifdef __cplusplus
}
#endif

#endif

