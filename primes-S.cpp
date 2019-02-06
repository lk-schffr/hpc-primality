#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

// Time measurements.
struct timeval TIMEVAL;
#define GET_TIME ((gettimeofday(&TIMEVAL, NULL), (TIMEVAL.tv_sec + TIMEVAL.tv_usec * 1.e-6)))

// Debug mode. Comment next line to disable it. Uncomment to enable.
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PADDING 5
double START_TIME;
int RANK = 0;
#define DEBUG_TIME (GET_TIME-START_TIME)
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
   printf("(DEBUG) T+%6.2fs: %*d # node: Progress %3d%% - testing `%llu` modulo `%llu`\n", DEBUG_TIME, DEBUG_PADDING*RANK+2, RANK, prog, n, i);
  }
#endif

  if ((n % i) == 0 || (n % (i + 2)) == 0) return false;
 }
 return true;
}

// Main function.
int main(int argc, char **argv) {
 int found;
 double runTime;

#if NUM_START > NUM_END
 printf("   Error: NUM_START can't be bigger than NUM_END!\n"); return 0;
#endif

#ifdef DEBUG
 START_TIME = GET_TIME;
#endif

 printf("---------------------------------\n   HPC Primality Test (version SINGLE)\n---------------------------------\n   Running on SINGLE NODE.\n   Checking %llu number(s) starting from %llu to %llu for primality!\n---------------------------------\n", NUM_END - NUM_START + 1, NUM_START, NUM_END);

 num n = NUM_START;
 found = 0;

 if (n < 2) n = 2;
 if ((n > 2) && ((n & 1) == 0)) ++n;

 // Start measuring time.
 runTime = GET_TIME;

 while (n <= NUM_END) {
  char prime;

#ifdef DEBUG
  if (n != 0) {
   printf("(DEBUG) T+%6.2fs: %*d # node: Number `%llu` for primality test...\n", DEBUG_TIME, DEBUG_PADDING*RANK+2, RANK, n);
  }
#endif

  prime = isPrime(n) ? 1 : 0;

#ifdef DEBUG
  printf("(DEBUG) T+%6.2fs: %*d # node: Calculated status `%d` for number `%llu`...\n", DEBUG_TIME, DEBUG_PADDING*RANK+2, RANK, prime, n);
#endif

  if (prime == 1) {
   ++found;
   printf("   Computational node found prime:\t%llu\n", n);
  }
  if (n == MAXIMUM_NUM) break; // Largest possible LLU.
  if (n < 3) n += 1; else n += 2;
 }

 // Stop measuring time.
 runTime = GET_TIME - runTime;

#ifdef DEBUG
 printf("(DEBUG) T+%6.2fs: The search is over!\n", DEBUG_TIME);
#endif

 printf("---------------------------------\n   Found %d prime(s)! It took %.3f seconds!\n---------------------------------\n", found, runTime);
 return 0;
}