#include <stdio.h>
#include <inttypes.h>

#define TARGET 1000001

int sumofdiv[TARGET];

void init(){
    sumofdiv[0] = sumofdiv[1] = 1;
    int64_t p, sum, ppow;
    for(int64_t n = 2; n < TARGET; n++){
        p = sumofdiv[n];
        if(!p){ //prime
            sumofdiv[n] = n+1;
            for(int64_t j = n*n; j < TARGET; j+=n){
                sumofdiv[j] = n;
            }
        } else{
            //p is a prime factor
            sum = 1 + p;
            ppow = p*p;
            while((n%ppow) == 0){
                sum += ppow;
                ppow*=p;
            }
            sumofdiv[n] = sum*sumofdiv[n*p/ppow];
        }
    }

    //proper divisors
    for(int64_t n = 2; n < TARGET; n++){
        sumofdiv[n]-=n;
    }
    sumofdiv[1] = TARGET + 1; // if a chain has a prime, then is no cicle: this forces to break
}

//binary array access macros
#define baisset(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) != 0
#define baisclear(ba, i) ((ba)[(i)>>6] & (1ull << ((i) & 63))) == 0
#define baset(ba, i) (ba)[(i)>>6] |= (1ull << ((i) & 63))

uint64_t visited[1+(TARGET>>6)];

void hareturtle(int i, int n, int *plen, int *pciclemin) {
	int turtle = i;
	int hare = i;

	int cicle = 1;

	do {
		turtle = sumofdiv[turtle];
		for (int step = 0; cicle && step < 2; step++) {
			hare = sumofdiv[hare];
			if (hare >= n || (baisset(visited, hare))) {
				cicle = 0;				
			}
		}
	} while (cicle && turtle != hare);
	
    //mark all elements to avoid future revisit
	for (turtle = i; turtle < n && !(baisset(visited, turtle)); turtle = sumofdiv[turtle]) {
        baset(visited, turtle);
	}

	if (!cicle) {
		*plen = 0;
		return;
	}

    
	int len;
	turtle = sumofdiv[hare];//turtle and hare in the cicle, turtle in the next position 
    *pciclemin = turtle;
	for(len = 1; turtle != hare; len++){
		turtle = sumofdiv[turtle];
        if(turtle < *pciclemin)
            *pciclemin = turtle;
    }

	
	*plen = len;
}

int main()
{
	init();
	
    int n = TARGET;

	int maxlen = 0;
	int minfirst = 0;

	for (int i = 1; i < n; i++) {
		if (baisset(visited, i)) continue;
		int ciclelen, ciclemin;
		hareturtle(i, n, &ciclelen, &ciclemin);
		if (ciclelen > maxlen || (ciclelen == maxlen && ciclemin < minfirst)) {
			maxlen = ciclelen;
			minfirst = ciclemin;
		}
	}

	printf("%d\n", minfirst);

    return 0;
}