# csc443-a1
Project To help measure time spent writing to different mediums in blocks, or doing sequential reads in blocks.

create_random_file =>

   Usage: 

      bin/create_random_file <file name> <Total bytes to write> <Block/Buffer size to use> [-v verbose]

This function will write random characters into <file name> using buffers, and measure how much time the fwrite/fflush operations take.

