/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Function to check if a number is prime */
int isPrime(int num) {
    if (num <= 1) {
        return 0; // 0 and 1 are not prime
    }
    
    int i;
    int sqrt_num = sqrt(num);
    for (i = 2; i <= sqrt_num; i++) {
        if (num % i == 0) {
            return 0; // Found a divisor, so it's not prime
        }
    }
    
    return 1; // It's prime
}

int main(int argc, const char** argv) {

    int lowerBound, upperBound;

    if (argc != 3) {
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if (lowerBound < 1 || upperBound < 1) {
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
                " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
    
    for (int num = lowerBound; num <= upperBound; num++) {
        if (isPrime(num) == 1) {
            printf("%d\n", num);
        }
    }

    return 0;
}