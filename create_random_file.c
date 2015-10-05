#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

// Since we are only working with characters in the buffers
#define ITEM_SIZE sizeof(char)

// Booleans are defined in C
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

   // Check if verbose flag is found
   if (argc == 5 && strcmp(argv[4], "-v") == 0) {
      verbose = true;
   }

   // Try to convert str to long type
   // If there is an error ('123k232' instead of '123232')
   // Then an error will be set at err and we check that to 
   // see whether it was successful or not.
   char* err;   
   long totalBytes = strtol(argv[2], &err, 10);
   
   if (!*err) {
      if (verbose)
         printf("Total Bytes Entered: %lu\n", totalBytes);
   } else {
      printf("Invalid Total bytes value (Not a number).\n");
      goto errexit;
   }

   long buffSize = strtol(argv[3], &err, 10);
   
   if (!*err) {
      if (verbose)
         printf("Block Size Entered: %lu\n", buffSize);
   } else {
      printf("Invalid Block Size entered (Not a number).\n");
      goto errexit;
   }

   char* randString = malloc(buffSize * sizeof(char));
   long bytesWritten = 0;

   // time_before, time_after, time_result (after diff)
   struct timeval ti_b, ti_a, ti_r;
   double totalBuffTimeMS;
   double totalTimeMS;
   double totalTimeS;

   FILE *fp = fopen(argv[1], "w");

   int fullBuffs = totalBytes / buffSize;

   // Iterate and write the full buffer lengths out to the file
   // if there is an amount left which is less than the buffer amount
   // given, then move onto the next code block
   int j;
   for (j=0; j < fullBuffs; j++) {
      random_array(randString, buffSize);
      
      // Start Timer
      gettimeofday(&ti_b, NULL);

      fwrite(randString, ITEM_SIZE, buffSize, fp);
      fflush(fp);

      // Stop Timer
      gettimeofday(&ti_a, NULL);

      // Find diff between before and after tiemstamp
      timersub(&ti_a, &ti_b, &ti_r);

      totalBuffTimeMS = (ti_r.tv_sec * 1000.0) + (ti_r.tv_usec / 1000.0);

      if (verbose) {
         printf("Buffer %d stats: ", j+1);
         printf("Bytes Written: %lu ", ITEM_SIZE * buffSize);
         printf("Time to write (MS): %f\n\n", totalBuffTimeMS);
      }

      totalTimeMS += totalBuffTimeMS;
      totalBytes -= buffSize;
      bytesWritten += ITEM_SIZE * buffSize;
   }

   // That means there is still somedata left to be written but
   // it won't take up the entire buffer size. So we just finish writing out
   // out the rest, using the totalBytes as a remainder of bytes left to write.
   if (totalBytes != 0) {

      random_array(randString, totalBytes);

      // Start Timer
      gettimeofday(&ti_b, NULL);

      fwrite(randString, ITEM_SIZE, totalBytes, fp);
      fflush(fp);

      // Stop Timer
      gettimeofday(&ti_a, NULL);
      timersub(&ti_a, &ti_b, &ti_r);

      totalBuffTimeMS = (ti_r.tv_sec * 1000.0) + (ti_r.tv_usec / 1000.0);

      if (verbose) {
         printf("Buffer %d stats: ", j+1);
         printf("Bytes Written: %lu ", ITEM_SIZE * totalBytes);
         printf("Time (MS): %f\n", totalBuffTimeMS);
      }

      totalTimeMS += totalBuffTimeMS;
      bytesWritten += ITEM_SIZE * totalBytes;
      totalBytes -= totalBytes;
   }

   if (verbose) {
      printf("\n-----\n");
   }

   totalTimeS = totalTimeMS * 1000;

   printf("Total Bytes Written: %lu\n", bytesWritten);
   printf("Total Time to write (MS): %f\n", totalTimeMS);
   printf("Write rate: %f bytes/sec\n", (int) bytesWritten/totalTimeS);  

   // Clean up
   fclose(fp);
   free(randString);
   remove(argv[1]);
   
   return EXIT_SUCCESS;

errexit:
   return EXIT_FAILURE;
}
