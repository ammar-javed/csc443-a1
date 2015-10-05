#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

#define ITEM_SIZE sizeof(char)
typedef int bool;
#define true 1
#define false 0

/**
 * populate a random array (which is already
 * allocated with enough memory to hold n bytes.
 */
void random_array(char *array, long bytes) {
   long i;
   for (i=0; i < bytes; i++){
      array[i] = 'A' + (rand() % 26);
   }
}

/*
 * Main driver
 */
int main(int argc, char** argv) {

   bool verbose = false;

   if (argc < 4) {
      printf("\nERROR: Invalid number of arguments. Please use the following format:\n");
      printf("create_random_file <filename> <total bytes> <block size in bytes> [-v verbose]\n");
      goto errexit;
   } 

   if (argc == 5 && strcmp(argv[4], "-v") == 0) {
      verbose = true;
   }
   char* err;   
   long totalBytes = strtol(argv[2], &err, 10);
   
   if (!*err) {
      printf("Total Bytes Entered: %lu\n", totalBytes);
   } else {
      printf("Invalid Total bytes value (Not a number).\n");
      goto errexit;
   }

   long buffSize = strtol(argv[3], &err, 10);
   
   if (!*err) {
      printf("Block Size Entered: %lu\n", buffSize);
   } else {
      printf("Invalid Block Size entered (Not a number).\n");
      goto errexit;
   }

   char* randString = malloc(buffSize * sizeof(char));
   long bytesWritten = 0;

   struct timeb ti_s, ti_e;
   long endTime;
   long totalTimeMS = 0;

   FILE *fp = fopen(argv[1], "w");

   int fullBuffs = totalBytes / buffSize;

   int j;
   for (j=0; j < fullBuffs; j++) {
      random_array(randString, buffSize);
      
      // Start Timer
      ftime(&ti_s);

      fwrite(randString, ITEM_SIZE, buffSize, fp);
      fflush(fp);

      // Stop Timer
      ftime(&ti_e);
      endTime = (ti_e.time - ti_s.time) * 1000.0 + (ti_e.millitm - ti_s.millitm);

      if (verbose) {
         printf("Buffer %d stats: ", j+1);
         printf("Bytes Written: %lu ", ITEM_SIZE * buffSize);
         printf("Time to write (MS): %lu\n\n", endTime);
      }

      totalTimeMS += endTime;
      totalBytes -= buffSize;
      bytesWritten += ITEM_SIZE * buffSize;
   }

   if (totalBytes != 0) {

      random_array(randString, totalBytes);

      // Start Timer
      ftime(&ti_s);

      fwrite(randString, ITEM_SIZE, totalBytes, fp);
      fflush(fp);

      // Stop Timer
      ftime(&ti_e);
      endTime =(ti_e.time - ti_s.time) * 1000.0 + (ti_e.millitm - ti_s.millitm); 

      if (verbose) {
         printf("Buffer %d stats: ", j+1);
         printf("Bytes Written: %lu ", ITEM_SIZE * totalBytes);
         printf("Time to write (MS): %lu\n", endTime);
      }

      totalTimeMS += endTime;
      bytesWritten += ITEM_SIZE * totalBytes;
      totalBytes -= totalBytes;
   }

   printf("\n-----\n");
   printf("Total Bytes Written: %lu\n", bytesWritten);
   printf("Total Time to write (MS): %lu\n\n", totalTimeMS);
  
   fclose(fp);
   free(randString);
   remove(argv[1]);
   
   return EXIT_SUCCESS;

errexit:
   return EXIT_FAILURE;
}
