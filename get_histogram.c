#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

// Only working with reading and writing chars
#define ITEM_SIZE sizeof(char)

// Maximum size of the histogram array for the alphabet
#define NUM_ALPHABETS 26

// Bools not defined in C
typedef int bool;
#define true 1
#define false 0

bool VERBOSE = false;

/**
 * file_ptr : the file pointer, ready to be read from.
 * hist: an array to hold 26 long integers.  hist[0] is the
 *       number of 'A', and hist[1] is the number of 'B', etc.
 * block_size: the buffer size to be used.
 * milliseconds: time it took to complete the file scan
 * total_bytes_read: the amount data in bytes read
 *
 * returns: -1 if there is an error.
 */
int get_histogram(
    FILE *file_ptr, 
    long hist[], 
    long block_size, 
    double *milliseconds, 
    long *total_bytes_read) {

   char* buffer = malloc(block_size * ITEM_SIZE);

   // Return code of fread; will contain the # of bytes read
   int rc;
   int i;

   // Keep track of how many times we read
   int j = 0;

   // Character we will be currently reading in the buffer
   int ch;

   // Use this ASCII value to calculate the position of the letter
   // in the hist array
   int AA = 'A';

   // time_before, time_after, time_result (after diff)
   struct timeval ti_b, ti_a, ti_r;
   double totalBuffTimeMS;

   // Clear out any junk data from the array
   memset(hist, 0, NUM_ALPHABETS * sizeof(long));

   while( !feof(file_ptr) ) {

      // Clear out the buffer for fresh use
      memset(buffer, 0, block_size * ITEM_SIZE);

      //Start Timer
      gettimeofday(&ti_b, NULL);

      rc = fread(buffer, ITEM_SIZE, block_size * ITEM_SIZE, file_ptr);   

      //Stop Timer
      gettimeofday(&ti_a, NULL);

      if ( ferror(file_ptr) ) {
         free(buffer);
         return -1;
      }

      // Iterate over all characters read
      // update correct array indice in the histogram array
      for (i=0; i < rc; i++) {
         ch = buffer[i];
         hist[ ch - AA ]++;
      }

      // Find diff between before and after tiemstamp
      timersub(&ti_a, &ti_b, &ti_r);

      totalBuffTimeMS = (ti_r.tv_sec * 1000.0) + (ti_r.tv_usec / 1000.0); 
      *milliseconds += totalBuffTimeMS;
      *total_bytes_read += rc;

      if (VERBOSE) {
         printf("Buffer %d stats: ", j+1);
         printf("Bytes Read: %lu ", ITEM_SIZE * rc);
         printf("Time (MS): %f\n", totalBuffTimeMS);
      }
      j++;
   }
   
   free(buffer); 
   return 0;
}

/*
 * Main driver
 */
int main(int argc, char** argv) {

   if (argc < 3) {
      printf("\nERROR: Invalid number of arguments. Please use the following format:\n");
      printf("get_histogram <filename> <block size in bytes> [-v verbose]\n");
      goto errexit;
   }

   if (argc == 4 && strcmp(argv[3], "-v") == 0) {
      VERBOSE = true;
   }

   char* err;
   long buffSize = strtol(argv[2], &err, 10);

   if (!*err) {
      if (VERBOSE)
         printf("Block Size Entered: %lu\n", buffSize);
   } else {
      printf("Invalid Block Size (Not a number).\n");
      goto errexit;
   }

   long hist[NUM_ALPHABETS];
   double milliseconds;
   long filelen;
   FILE *file_ptr = fopen(argv[1], "r");
 
   /**
    * Compute the histogram using 2K buffers
    */
   int ret = get_histogram( file_ptr, 
                            hist, 
                            buffSize,
                            &milliseconds,
                            &filelen);
   if (ret < 0) {
      printf("An Error occured while trying to read the file! \n");
      goto errexit;
   }
 
   printf("Computed the histogram in %f ms.\n", milliseconds);

   for(int i=0; i < NUM_ALPHABETS; i++) {
       printf("%c : %lu\n", 'A' + i, hist[i]);
   }

   printf("Data rate: %lu Bps\n", (long) (filelen/milliseconds * 1000.0));

   fclose(file_ptr);
   return EXIT_SUCCESS;

errexit:

   return EXIT_FAILURE;
}
