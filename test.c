#include <stdlib.h>
#include <stdio.h>
#include "freqlist.h"

int main (int argc, char * argv[]) {
	freqlist f = newFreqList();
	int i, err[3];

	for(i = 0; i < 15; i++) {
		add(f, (byte) 'A');
		add(f, (byte) 'B');
	}

	for(i = 0; i < 10; i++) {
		add(f, (byte) 'A');
		add(f, (byte) 'C');
	}	

	for(i = 0; i < 5; i++) {
		add(f, (byte) 'A');
		add(f, (byte) 'D');
	}

	flist_dump(f);

	for( initIterator(f); !endOfList(f); next(f, &(err[0])) ) {
		printf("%x : %d (%d, %d, %d)\n", getValue(f, &(err[1])), getOccurrences(f, &(err[2])), err[0], err[1], err[2]);
	}

	return 0;
}