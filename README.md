# Carleton-SCSI-2019
Some of the Code from my time at the Summer Computer Science Institute at Carleton

# Research:
  This code has to do with my research project on Parallel and Distributed Systems. 
Here we test how adding more processors would effect 3 different problems (using the MPI Framework), these problems runtime where then compared between two different systems.

  ### 1. Parallel Conway's Game of Life .       
      - Built in C, it splits the board into segments for each worker to process.
      - Information is sent to and from the head node at the start and end of each generation.
      
      - We found that the processing time for each generation was very small.
      - We found that the amount of time spent communicating became the bottle neck.
      - adding more processors made the game compute much slower.
