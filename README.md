# Carleton-SCSI-2019
Some of the Code from my time at the Summer Computer Science Institute at Carleton

# Research:
  This code has to do with my research project on Parallel and Distributed Systems. 
Here we test how adding more processors would effect 3 different problems (using the MPI Framework), these problems runtime where then compared between two different systems.

  ### 1. Parallel Conway's Game of Life .       
      - Built in C, it splits the board into segments for each worker to process.
      - Information is sent to and from the head node at the start and end of each generation.
      - Works by checking the cells around it, updating its own state if enough are alive.
  ![chartgraph](https://github.com/rbrtknwls/Carleton-SCSI-2019/blob/master/conwaysChart.png) .
  
      - We found that the processing time for each generation was very small.
      - We found that the amount of time spent communicating became the bottle neck.
      - Adding more processors made the game compute much slower.
      
  ### 2. Parallel Water Path Finding .       
      - Built in C, it splits the board into segments for each worker to process.
      - Information is sent to and from the head node at the start and end of each generation.
      - Works by checking if the end position has already been reached, if not the water will expand
  ![chartgraph](https://github.com/rbrtknwls/Carleton-SCSI-2019/blob/master/waterChart.png) .
  
      - We found that the processing time for each generation was very small.
      - We found that the amount of time spent communicating became the bottle neck.
      - Adding more processors made the code compute much slower.
      
  ### 2. Parallel Intigration.       
      - Built in C, it splits the problem at the start into segments.
      - Information is only sent once, and received once.
  ![chartgraph](https://github.com/rbrtknwls/Carleton-SCSI-2019/blob/master/IntiChart.png) .
  
      - We found that the processing time was the major bottle neck.
      - Communication time was fast.
      - Adding more processors made the code compute much faster.
      
# Natural Language Processing . 
   - What I did week 2 of the program! (week 3 was computer music... I forgot to save the code).
   - Used python to do data abstraction on different text files.
   - Also created an AI to generate text.
