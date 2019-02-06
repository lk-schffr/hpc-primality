# High-performance computing with Message Passing Interface (MPI) for testing numbers primality

In this project there are three approaches to numbers primality testing.
Two of them will use high-performance computing with Message Passing Interface (MPI).

All tests were run on 64-bit [*PelicanHPC GNU/Linux*](https://pelicanhpc.org/) operating system with [*Open MPI*](https://www.open-mpi.org/) (version 1.6.5) installed. The cluster was made of three nodes running inside [*Oracle VM VirtualBox*](https://www.virtualbox.org/) virtual machines on a laptop computer with Intel® Core™ i7-6700HQ processor.

## Tested approaches to numbers primality testing
 * Single node, single thread approach
 * HPC MPI approach -- version I
 * HPC MPI approach -- version II
 
## Program outputs

### Single node, single thread approach
```
$ g++ primes-S.cpp -Wall -o out.bin && ./out.bin
---------------------------------
   HPC Primality Test (version SINGLE)
---------------------------------
   Running on SINGLE NODE.
   Checking 1025 number(s) starting from 22790428875364879 to 22790428875365903 for primality!
---------------------------------
   Computational node found prime:	22790428875364879
   Computational node found prime:	22790428875365903
---------------------------------
   Found 2 prime(s)! It took 2.066 seconds!
---------------------------------
```

### HPC MPI approach - version I
```
$ mpiCC primes-1.cpp -Wall -o out.bin && mpiexec -hostfile ~/tmp/bhosts -np 4 out.bin
---------------------------------
   HPC Primality Test (version I)
---------------------------------
   Running on 4 node(s) (3 computational).
   Checking 1025 number(s) starting from 22790428875364879 to 22790428875365903 for primality!
---------------------------------
   Available nodes:
      - Root node          - rank 00 - runs on:    pel33
      - Computational node - rank 01 - runs on:    pel35
      - Computational node - rank 02 - runs on:  pelican
      - Computational node - rank 03 - runs on:    pel33
---------------------------------
   Computational node #01 found prime:	22790428875364879
   Computational node #03 found prime:	22790428875365903
---------------------------------
   Found 2 prime(s)! It took 5.018 seconds!
---------------------------------
```

### HPC MPI approach - version II
```
$ mpiCC primes-2.cpp -Wall -o out.bin && mpiexec -hostfile ~/tmp/bhosts -np 4 out.bin
---------------------------------
   HPC Primality Test (version II)
---------------------------------
   Running on 4 node(s) (3 computational).
   Checking 1025 number(s) starting from 22790428875364879 to 22790428875365903 for primality!
---------------------------------
   Available nodes:
      - Root node          - rank 00 - runs on:    pel33
      - Computational node - rank 01 - runs on:    pel35
      - Computational node - rank 02 - runs on:  pelican
      - Computational node - rank 03 - runs on:    pel33
---------------------------------
   Computational node #01 found prime:	22790428875364879
   Computational node #03 found prime:	22790428875365903
---------------------------------
   Found 2 prime(s)! It took 1.569 seconds!
---------------------------------
```

### Running with debug mode enabled
```
$ mpiCC primes-2.cpp -Wall -o out.bin && mpiexec -hostfile ~/tmp/bhosts -np 10 out.bin
(DEBUG) T+  0.00s:                 3 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.00s:                                               9 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:                           5 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:                                6 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:                                          8 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:            2 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:                                     7 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:       1 # node: Ready. Waiting for segments for calculations...
(DEBUG) T+  0.01s:                      4 # node: Ready. Waiting for segments for calculations...
---------------------------------
   HPC Primality Test (version II)
---------------------------------
   Running on 10 node(s) (9 computational).
   Checking 1025 number(s) starting from 22790428875364879 to 22790428875365903 for primality!
---------------------------------
   Available nodes:
      - Root node          - rank 00 - runs on:    pel33
      - Computational node - rank 01 - runs on:    pel35
      - Computational node - rank 02 - runs on:  pelican
      - Computational node - rank 03 - runs on:    pel33
      - Computational node - rank 04 - runs on:    pel35
      - Computational node - rank 05 - runs on:  pelican
      - Computational node - rank 06 - runs on:    pel33
      - Computational node - rank 07 - runs on:    pel35
      - Computational node - rank 08 - runs on:  pelican
      - Computational node - rank 09 - runs on:    pel33
---------------------------------
(DEBUG) T+  0.01s: Sending segment <22790428875364879; 22790428875364991> to node `1`...
(DEBUG) T+  0.01s: Sending segment <22790428875364992; 22790428875365104> to node `2`...
(DEBUG) T+  0.01s: Sending segment <22790428875365105; 22790428875365217> to node `3`...
(DEBUG) T+  0.01s: Sending segment <22790428875365218; 22790428875365330> to node `4`...
(DEBUG) T+  0.02s:                 3 # node: Got segment <22790428875365105; 22790428875365217>!
(DEBUG) T+  0.02s:            2 # node: Got segment <22790428875364992; 22790428875365104>!
(DEBUG) T+  0.02s:            2 # node: Checking number `22790428875364993` for primality...
(DEBUG) T+  0.01s: Sending segment <22790428875365331; 22790428875365443> to node `5`...
(DEBUG) T+  0.02s:            2 # node: Calculated status `0` for number `22790428875364993`.
(DEBUG) T+  0.02s:                 3 # node: Checking number `22790428875365105` for primality...
(DEBUG) T+  0.02s:                      4 # node: Got segment <22790428875365218; 22790428875365330>!
(DEBUG) T+  0.02s:                 3 # node: Calculated status `0` for number `22790428875365105`.
(DEBUG) T+  0.02s:                      4 # node: Checking number `22790428875365219` for primality...
(DEBUG) T+  0.02s:                 3 # node: Checking number `22790428875365107` for primality...
(DEBUG) T+  0.01s:       1 # node: Got segment <22790428875364879; 22790428875364991>!
(DEBUG) T+  0.02s:                 3 # node: Calculated status `0` for number `22790428875365107`.

[...]

(DEBUG) T+  0.46s:                                               9 # node: Checking number `22790428875365903` for primality...
(DEBUG) T+  0.48s:       1 # node: Progress  70% - testing `22790428875364879` modulo `107106665`
(DEBUG) T+  0.53s:                                6 # node: Calculated status `0` for number `22790428875365507`.

[...]

(DEBUG) T+  0.91s:       1 # node: Calculated status `0` for number `22790428875364991`.
(DEBUG) T+  0.91s:       1 # node: Done. Sending results to root node...
(DEBUG) T+  0.91s:       1 # node: My job is done! Exiting...
(DEBUG) T+  0.90s: Node `1` found `1` primes!
(DEBUG) T+  0.90s: Received prime `22790428875364879` from node `1`...
   Computational node #01 found prime:	22790428875364879
(DEBUG) T+  0.90s: Node `2` found `0` primes!
(DEBUG) T+  0.90s: Node `3` found `0` primes!
(DEBUG) T+  0.90s: Node `4` found `0` primes!
(DEBUG) T+  0.90s: Node `5` found `0` primes!
(DEBUG) T+  0.90s: Node `6` found `0` primes!
(DEBUG) T+  0.90s: Node `7` found `0` primes!
(DEBUG) T+  0.90s: Node `8` found `0` primes!
(DEBUG) T+  1.09s:                                               9 # node: Progress  70% - testing `22790428875365903` modulo `107106665`
(DEBUG) T+  1.34s:                                               9 # node: Calculated status `1` for number `22790428875365903`.
(DEBUG) T+  1.34s:                                               9 # node: Done. Sending results to root node...
(DEBUG) T+  1.34s:                                               9 # node: My job is done! Exiting...
(DEBUG) T+  1.33s: Node `9` found `1` primes!
(DEBUG) T+  1.33s: Received prime `22790428875365903` from node `9`...
   Computational node #09 found prime:	22790428875365903
(DEBUG) T+  1.33s: The search is over!
---------------------------------
   Found 2 prime(s)! It took 1.326 seconds!
---------------------------------
```


