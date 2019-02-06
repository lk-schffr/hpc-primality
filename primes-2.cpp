#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <vector>

// Debug mode. Comment next line to disable it. Uncomment to enable.
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PADDING 5
double START_TIME;
int RANK;
#endif

//                        ~1.8 * 10 ^ 19 === 2^64 - 1
// Number can be from 0 to 18,446,744,073,709,551,615
//                    0    18446744073709551615
#define MAXIMUM_NUM 18446744073709551615LLU

////////////////////////////////////////////////
// Test cases

// Change test case below.
#define TEST_CASE 3

// Test #1: Primes between 0 and 1000  -> pi(1000)  = 168  primes (Time <1 seconds)
#if TEST_CASE == 1
#define NUM_START 0LLU
#define NUM_END   1000LLU

// Test #2: Primes between 0 and 10000 -> pi(10000) = 1229 primes (Time ~2 seconds)
#elif TEST_CASE == 2
#define NUM_START 0LLU
#define NUM_END   10000LLU

// Test #3: Prime gap between 22790428875364879 and 22790428875365903 is 1024 -> 2 primes (Time ~5-6   seconds)
#elif TEST_CASE == 3
#define NUM_START 22790428875364879LLU
#define NUM_END   22790428875365903LLU

// Test #4: Checking 3334 numbers starting from 22790428875364444 to 22790428875367777 for primality -> 56 primes (Time ~29-37 seconds)
#elif TEST_CASE == 4
#define NUM_START 22790428875364444LLU
#define NUM_END   22790428875367777LLU

// Test #5: Primes among last 100 numbers that can be stored in unsigned long long int -> 3 primes (Time ~68-77 seconds)
#elif TEST_CASE == 5
#define NUM_START 18446744073709551516LLU
#define NUM_END   18446744073709551615LLU

// Test #6: Prime gap between 9586724781371233277 and 9586724781371234779 is 1502 -> 2 primes (Time ~145-159 seconds)
#elif TEST_CASE == 6
#define NUM_START 9586724781371233277LLU
#define NUM_END   9586724781371234779LLU

// Custom test case.
#else
#define NUM_START 100LLU
#define NUM_END   200LLU

#endif

////////////////////////////////////////////////
// Tested edge cases.

//#define NUM_START 18446744073709551614LLU
//#define NUM_END   18446744073709551614LLU

//#define NUM_START MAXIMUM_NUM
//#define NUM_END   MAXIMUM_NUM

//#define NUM_START 15LLU
//#define NUM_END   13LLU

//
////////////////////////////////////////////////

// Num data type.
typedef unsigned long long int num;

// Primality test.
bool isPrime(num n) {
 if (n < 2) return false;
 if (n < 4) return true;
 if ((n & 1) == 0) return false;
 if ((n % 3) == 0) return false;

 num sqrtN = (num) (sqrt(n)) + 1;

 for (num i = 5; i <= sqrtN; i += 6) {

#ifdef DEBUG
  if (i % 21421333 == 0) {
   char prog = (char)((i*100)/sqrtN);
   printf("(DEBUG) T+%6.2fs: %*d # node: Progress %3d%% - testing `%llu` modulo `%llu`\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*RANK+2, RANK, prog, n, i);
  }
#endif

  if ((n % i) == 0 || (n % (i + 2)) == 0) return false;
 }
 return true;
}

// Main function.
int main(int argc, char **argv) {
 int size, rank, nodes;
 num found;
 double time;

 MPI_Init(&argc, &argv);

 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 nodes = size - 1;

#if NUM_START > NUM_END
 if (rank == 0) printf("   Error: NUM_START can't be bigger than NUM_END!\n"); MPI_Finalize(); return 0;
#endif

#ifdef DEBUG
 RANK = rank;
 START_TIME = MPI_Wtime();
#endif

 char processorName[MPI_MAX_PROCESSOR_NAME];
 int processorNameLen;
 MPI_Get_processor_name(processorName, &processorNameLen);

 // Processors names.
 int maxProcessorNameLen;
 MPI_Reduce(&processorNameLen, &maxProcessorNameLen, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
 maxProcessorNameLen = maxProcessorNameLen + 1;
 if (rank == 0) {
  printf("---------------------------------\n   HPC Primality Test (version II)\n---------------------------------\n   Running on %d node(s) (%d computational).\n   Checking %llu number(s) starting from %llu to %llu for primality!\n---------------------------------\n", size, nodes, NUM_END - NUM_START + 1, NUM_START, NUM_END);
  printf("   Available nodes:\n");
  printf("      - Root node          - rank %02d - runs on: %*s\n", rank, maxProcessorNameLen, processorName);

  for (int i = 1; i < size; ++i) {
   char name[MPI_MAX_PROCESSOR_NAME] = {};
   MPI_Recv(name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   printf("      - Computational node - rank %02d - runs on: %*s\n", i, maxProcessorNameLen, name);
  }

  printf("---------------------------------\n");
 } else {
  MPI_Send(processorName, processorNameLen, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
 }

 // Root node.
 if (rank == 0) {

  if (nodes < 1) {
   printf("   Error: No nodes for computation! Program requires at least 2 nodes!\n");
   MPI_Finalize();
   return 0;
  }

  num segmentSize = (NUM_END - NUM_START) / ((num) nodes);
  num segmentStart;
  num segmentEnd;

  found = 0LLU;

  // Start measuring time.
  time = MPI_Wtime();

  // Sending segments to workers...
  for (int segment = 0; segment < nodes; ++segment) {
   segmentStart = NUM_START + segmentSize * segment;

   // Last segment
   if (segment == nodes - 1) {
    segmentEnd = NUM_END;
   } else {
    segmentEnd = segmentStart + segmentSize - 1;
   }

#ifdef DEBUG
   printf("(DEBUG) T+%6.2fs: Sending segment <%llu; %llu> to node `%d`...\n", (MPI_Wtime()-START_TIME), segmentStart, segmentEnd, (segment + 1));
#endif

   MPI_Send(&segmentStart, 1, MPI_UNSIGNED_LONG_LONG, (segment + 1), 0, MPI_COMM_WORLD);
   MPI_Send(&segmentEnd, 1, MPI_UNSIGNED_LONG_LONG, (segment + 1), 0, MPI_COMM_WORLD);
  }

  // Receiving results...

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: Waiting for results...\n", (MPI_Wtime()-START_TIME));
#endif

  for (int segment = 0; segment < nodes; ++segment) {
   // Found primes number.
   num count;
   MPI_Recv(&count, 1, MPI_UNSIGNED_LONG_LONG, (segment + 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

#ifdef DEBUG
   printf("(DEBUG) T+%6.2fs: Node `%d` found `%llu` primes!\n", (MPI_Wtime()-START_TIME), (segment + 1), count);
#endif

   for (num p = 0LLU; p < count; ++p) {
    // Receive prime.
    num number;
    MPI_Recv(&number, 1, MPI_UNSIGNED_LONG_LONG, (segment + 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

#ifdef DEBUG
    printf("(DEBUG) T+%6.2fs: Received prime `%llu` from node `%d`...\n", (MPI_Wtime()-START_TIME), number, (segment + 1));
#endif

    ++found;
    printf("   Computational node #%02d found prime:\t%llu\n", (segment + 1), number);
   }
  }

  // Stop measuring time.
  time = MPI_Wtime() - time;

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: The search is over!\n", (MPI_Wtime()-START_TIME));
#endif

 } else {
  // Computational nodes.

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: %*d # node: Ready. Waiting for segments for calculations...\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank);
#endif

  num segmentStart;
  num segmentEnd;

  found = 0LLU;
  std::vector <num> primesList;

  MPI_Recv(&segmentStart, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv(&segmentEnd, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: %*d # node: Got segment <%llu; %llu>!\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank, segmentStart, segmentEnd);
#endif

  num n = segmentStart;

  if (n < 2) n = 2;
  if ((n > 2) && ((n & 1) == 0)) ++n;

  while (n <= segmentEnd) {

#ifdef DEBUG
   printf("(DEBUG) T+%6.2fs: %*d # node: Checking number `%llu` for primality...\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank, n);
#endif

   char prime;
   prime = isPrime(n) ? 1 : 0;

#ifdef DEBUG
   printf("(DEBUG) T+%6.2fs: %*d # node: Calculated status `%d` for number `%llu`.\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank, prime, n);
#endif

   if (prime == 1) {
    ++found;
    primesList.push_back(n);
   }

   if (n == MAXIMUM_NUM) break; // Largest possible LLU.
   if (n < 3) n += 1; else n += 2;
  }

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: %*d # node: Done. Sending results to root node...\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank);
#endif

  // Send number of primes found.
  MPI_Send(&found, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);

  // Send all found prime numbers.
  for (std::vector<num>::iterator it = primesList.begin(); it != primesList.end(); ++it) {
   MPI_Send(&(*it), 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
  }

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: %*d # node: My job is done! Exiting...\n", (MPI_Wtime()-START_TIME), DEBUG_PADDING*rank+2, rank);
#endif
 }

 MPI_Finalize();

 if (rank == 0) {
  printf("---------------------------------\n   Found %llu prime(s)! It took %.3f seconds!\n---------------------------------\n", found, time);
 }
 return 0;
}