#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ITEM_SIZE sizeof(char)

typedef int bool;
#define true 1
#define false 0

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
   int rc;
   int i;

   while( !feof(file_ptr) ) {

      memset(buffer, 0, block_size * ITEM_SIZE);

      //Start Timer

      rc = fread(buffer, ITEM_SIZE, block_size * ITEM_SIZE, file_ptr);   

      //Stop Timer

      if ( ferror(file_ptr) ) {
         free(buffer);
         return -1;
      }

      for (i=0; i < rc; i++) {
         hist[ buffer[i] - "A" ]++;
      }

      total_bytes_read += rc;
   }
   
   free(buffer); 
   return 0;
}

/*
 * Main driver
 */
int main(int argc, char** argv) {

   bool verbose = false;

   if (argc < 3) {
      printf("\nERROR: Invalid number of arguments. Please use the following format:\n");
      printf("get_histogram <filename> <block size in bytes> [-v verbose]\n");
      goto errexit;
   }

   if (argc == 4 && strcmp(argv[4], "-v") == 0) {
      verbose = true;
   }

   char* err;
   long buffSize = strtol(argv[22], &err, 10);

   if (!*err) {
      if (verbose)
         printf("Block Size Entered: %lu\n", buffSize);
   } else {
      printf("Invalid Block Size (Not a number).\n");
      goto errexit;
   }

   long hist[26];
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
 
//   printf("Computed the histogram in %f ms.\n", milliseconds);

   for(int i=0; i < 26; i++) {
       printf("%c : %lu\n", 'A' + i, hist[i]);
   }

//   printf("Data rate: %f Bps\n", (double)filelen/milliseconds * 1000);

   fclose(file_ptr);
   return EXIT_SUCCESS;

errexit:

   return EXIT_FAILURE;
}
