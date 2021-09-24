//Test with FIPS180-2, PASSED!

#include <stdio.h>
#include "sha1.h"


int main() {
	unsigned int i;
	unsigned char *msg="abc"; // test data;
  unsigned char output[20];

	SHA1_Init();
	SHA1_Process(msg, output);
	
	for(i = 0; i < 20; i++)
		 printf("0x%02x,", output[i]);

  return 0;
}
