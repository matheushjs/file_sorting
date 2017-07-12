# pthreads_ParallelHeapsort
Implementation of parallel heapsort for sorting a file containing records.

# Results

Results are available on the file [results.txt](./results.txt). A summary is shown below.

Compiled using gcc 5.4.0.

Ran on processor: Intel(R) Core(TM) i7-5500U CPU @ 2.40GHz

Ran on disk: TOSHIBA MQ01ABD100, 5400RPM, 1TB capacity

- No Optimization

Normal | Parallel
---|---
26.85s | 23.91s
26.26s | 23.83s
27.83s | 24.03s

- -O3 Optimization

Normal | Parallel
---|---
14.79s | 13.44s
14.71s | 14.26s
14.74s | 13.34s

# File organization

- File contains records of Person objects
- Each Person has: Name (string) & ID (integer)
- ID is stored first, and uses up the space of a *long int*
- Name is written up to, and including, the `\0` character.

# Test file

 - Randomly generated.
 - Names have between 1 and 100 randomly generated characters between ascii 'A' and 'Z'.
 - 10,000,000 records. 344 MB.

# Thread scheme

Phase 1:
  
- Thread 1: Read records from the file
  
- Thread 2: Add records to the heap
  
Phase 2:
  
- Thread 1: Remove sorted elements from the heap
  
- Thread 2: Write records to the file
