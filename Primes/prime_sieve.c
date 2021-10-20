#include <stdio.h>
#include <stdlib.h>

#define SEARCH_LIMIT 1E6
#define TRUE 1
#define FALSE 0

int is_prime(int investigating, int *found_primes, int count);

int
main(int argc, char **argv){
	int* found_primes; int count = 1; int investigating = 3;
	found_primes = (int *) malloc(SEARCH_LIMIT*sizeof(int));
	
	found_primes[0] = 2; //first prime is 2
	while(count < SEARCH_LIMIT){//search for primes until limit
		if(is_prime(investigating, found_primes, count)){
			found_primes[count++] = investigating;
		}
		investigating += 2; //only searching for odd numbers
	}
	printf("%d\n",found_primes[count-1]);//print last prime found
}

/*A number is prime is all the primes found are not divisors of the 
 * number under investigation upto primes less than or equal to the 
 * root of the number under investigation. Technically, could start 
 * the search from found_primes[1] as not investigating even numbers.*/
int is_prime(int investigating, int *found_primes, int count){
	for(int i=0; i<count && (found_primes[i]*found_primes[i] <= investigating); i++){
		if(investigating % found_primes[i] == 0) return FALSE;
	}
	return TRUE;
}


