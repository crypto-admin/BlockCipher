#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sm3.h"


int main() {
  printf("enter sm3 test...\n");

	unsigned char digest[32] = {0x0};
  int i = 0;
	unsigned char message1[] =  "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";  // tested,ok!
  int len = strlen(message1);

  printf("\n********message hash result: *********\n");
	sm3_256(len, message1, digest);

	for (i = 0; i < 32; i++) {
		printf("0x%02x,",digest[i]);
	}

  return 0;
}