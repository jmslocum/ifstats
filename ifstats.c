/*************************************************************************************
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "ifstats.h"

static int readProcNetDevFile(char** output, int32_t* size);

/**

*/
IFStats_t* getIfStats(){

   char* fileText = NULL;
   int32_t size = 0;
   int error = 0;

   //Read in the /proc/net/dev file
   if ((error = readProcNetDevFile(&fileText, &size)) != 0){
      if (!fileText){
         errno = error;
         return NULL;
      }
   }

   IFStats_t *returnStats = NULL, *stats = NULL; 

   //Scan the file text and pull out the approperate data.
   char *lineptr, *tokenptr;
   char *line, *token;
   int i = 0;
   for (char* lines = fileText; ; lines = NULL){
      line = strtok_r(lines, "\n", &lineptr);
      if (line == NULL){
         break;
      }

      //Throw away the first two lines of text
      if (++i <= 2){
         continue;
      }

      stats = (IFStats_t*)malloc(sizeof(IFStats_t));
      if (!stats){
         error = errno;
         releaseStats(returnStats);
         errno = error;
         return NULL;
      }

      memset(stats, 0, sizeof(IFStats_t));

      //Break down the line into the values needed. 
      int j = 0;
      uint64_t* currentStat = &stats->rxBytes;  //Going to do some C magic here
      for (char* tokens = line; ; tokens = NULL, j++){
         token = strtok_r(tokens, ": ", &tokenptr);
         if (token == NULL){
            break;
         }
         
         if (j == 0){
            strncpy(stats->device, token, IFSTATS_NAME_LENGTH-1);
         }
         else {
            *currentStat = (uint64_t)strtoull(token, NULL, 10);
            currentStat++; //Move the pointer forward through the struct
         }
      }

      //Attach the structure just loaded
      if (returnStats == NULL){
         returnStats = stats;
      }
      else {
         IFStats_t* current;
         for (current = returnStats; current->next != NULL; current = current->next);
         current->next = stats;
      }
   }
  
   free(fileText);
   return returnStats;
}

/**
   This will release the stats objects obtained by the getIfStats function

   @param stats - The IFStats_t object to free
*/
void releaseStats(IFStats_t* stats){
   IFStats_t* next = NULL;
   while(stats){
      next = stats->next;
      free(stats);
      stats = next;
   }

   return;
}

/**
   Helper function to read in the /proc/net/dev file from the 
   filesystem and return the text in the file. It also gives 
   back the size of the text.

   @param output - The text pointer location to link the read text to
   @param size - The final size of the output text
   @return 0 on success, errno on error.
*/
static int readProcNetDevFile(char** output, int32_t* size){
   //Clear out errno for use during this function
   errno = 0;
   *output = NULL;
   *size = 0;

   int index = 0;
   int textSize = 64;
   char* filetext = (char*)malloc(sizeof(char) * textSize);

   if (!filetext){
      return errno;
   }

   //Open the proc file that has the necessary info
   FILE* devFile = fopen("/proc/net/dev", "r");
   
   if (!devFile){
      return errno;
   }

   //Since proc files done report their size (at least not this one)
   //Read character by character until EOF is hit.
   int c = 0;
   while ((c = fgetc(devFile)) != EOF){
      filetext[index++] = c;
      if (index >= textSize){
         textSize += 64;
         //Increase memory as needed
         filetext = (char*)realloc(filetext, sizeof(char) * textSize);
         if (!filetext){
            return errno;
         }
      }
   }

   fclose(devFile);

   filetext = (char*)realloc(filetext, sizeof(char) * (index + 1));
   if (!filetext){
      return errno;
   }
   
   filetext[index] = '\0';
   *output = filetext;
   *size = index;

   //should be 0, unless fclose failed for some reason, but even if
   //it did it is not a game stopper.
   return errno;
}

#ifdef IFSTATS_TEST
int main(int argc, char* argv[]){
   IFStats_t* stats = getIfStats();
   if (!stats){
      printf("Unable to fetch stats! %d (%s)\n", errno, strerror(errno));
      return EXIT_FAILURE;
   }

   for (IFStats_t* current = stats; current != NULL; current = current->next){
      printf("Device: %s\n", current->device);
      printf("\trxBytes:      %llu\n", current->rxBytes);
      printf("\trxPackets:    %llu\n", current->rxPackets);
      printf("\trxErrors:     %llu\n", current->rxErrors);
      printf("\trxDrop:       %llu\n", current->rxDrop);
      printf("\trxFifo:       %llu\n", current->rxFifo);
      printf("\trxFrame:      %llu\n", current->rxFrame);
      printf("\trxCompressed: %llu\n", current->rxCompressed);
      printf("\trxMulticast:  %llu\n", current->rxMulticast);
      printf("\ttxBytes:      %llu\n", current->txBytes);
      printf("\ttxPackets:    %llu\n", current->txPackets);
      printf("\ttxErrors:     %llu\n", current->txErrors);
      printf("\ttxDrop:       %llu\n", current->txDrop);
      printf("\ttxFifo:       %llu\n", current->txFifo);
      printf("\ttxCollisions: %llu\n", current->txCollisions);
      printf("\ttxCarrier:    %llu\n", current->txCarrier);
      printf("\ttxCompressed: %llu\n", current->txCompressed);
   }

   releaseStats(stats);  
}
#endif
