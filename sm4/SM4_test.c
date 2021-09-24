#include <stdio.h>
#include "sm4.h"

int main() {
	unsigned int i;
	unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
  unsigned char IV[16]={0x01};

	unsigned char output[100];
  unsigned int plain_len=16;
	unsigned int output_len;
  unsigned char plain[100];

	//output_len = SM4_ECB_ENC(input, plain_len, key, output);
	output_len = SM4_CBC_ENC(IV, input, plain_len, key, output);
  //数据测试段
	for(i = 0; i < output_len; i++)
		printf("0x%02x,", output[i]);
	printf("\n");

	//plain_len  = SM4_ECB_DEC(output, output_len, key, plain);
	plain_len  = SM4_CBC_DEC(IV, output, output_len, key, plain);
    
    //数据测试段
	for(i = 0; i < plain_len; i++)
		printf("0x%02x,", plain[i]);
	printf("\n");

  return 0;
}