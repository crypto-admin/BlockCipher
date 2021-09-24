// sm4算法
// 编程依据：SMS4国密算法: 明文长度128, 密文长度128， sbox为8->8

#include <string.h>
#include "sm4.h"

#define ROTL(x,n) ((unsigned int)x << n | ((unsigned int)x >> (32 - n)))

void int2char(unsigned int* in, unsigned char* out, int num) {
	int i = 0;
	for(i=0; i<num; i++) {
		out[4*i+0] = (unsigned char)(in[i]>>24);
		out[4*i+1] = (unsigned char)(in[i]>>16);
		out[4*i+2] = (unsigned char)(in[i]>>8);
		out[4*i+3] = (unsigned char)in[i];
	}
}



void char2int(unsigned char*in, unsigned int* out, int num) {
	int i=0; 
	if(num%4!=0)return;
	for(i=0; i<num/4; i++){
		out[i] = (unsigned int)in[4*i]<<24 ^ (unsigned int)in[4*i+1]<<16 ^ (unsigned int)in[4*i+2]<<8 ^(unsigned int)in[4*i+3];	
	}
}


static const unsigned char Sbox[256] = {
  0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05,
  0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
  0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62,
  0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6,
  0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8,
  0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35,
  0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87,
  0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e,
  0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1,
  0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3,
  0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f,
  0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51,
  0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8,
  0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0,
  0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84,
  0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
};

/* System parameter */
static const unsigned int FK[4] = {0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};

/* fixed parameter */
static const unsigned int CK[32] = {
  0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
  0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
  0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
  0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
  0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
  0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
  0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
  0x10171e25,0x2c333a41,0x484f565d,0x646b7279
};

unsigned int rk[32]={0x0};

/*
 *  T function: T() = L(t());
 * in: a 32 bits unsigned int ;
 * return: a 32 bits unsigned int
 */
static unsigned int sm4_T(unsigned int in) {
  unsigned int temp = 0;
	unsigned char temp1[4];

  temp1[0] = Sbox[(unsigned char)(in>>24)&0xff];
  temp1[1] = Sbox[(unsigned char)(in>>16)&0xff];
  temp1[2] = Sbox[(unsigned char)(in>> 8)&0xff];
  temp1[3] = Sbox[(unsigned char)(in&0xff)];
    
	temp  = (unsigned int)temp1[0]<<24 |(unsigned int)temp1[1]<<16 | (unsigned int)temp1[2]<<8 | (unsigned int)temp1[3];
		
  return temp^(ROTL(temp, 2))^(ROTL(temp, 10))^(ROTL(temp, 18))^(ROTL(temp, 24));
}

unsigned int T1(unsigned int in) {
  unsigned int temp = 0;
	unsigned char temp1[4];

  temp1[0] = Sbox[(unsigned char)(in>>24)&0xff];
  temp1[1] = Sbox[(unsigned char)(in>>16)&0xff];
  temp1[2] = Sbox[(unsigned char)(in>>8)&0xff ];
  temp1[3] = Sbox[(unsigned char)(in&0xff)];
    
	temp  = (unsigned int)temp1[0]<<24 |(unsigned int)temp1[1]<<16 | (unsigned int)temp1[2]<<8 | (unsigned int)temp1[3];
		
  return temp^(ROTL(temp, 13))^(ROTL(temp, 23));
}

/* main round F:F(x0,x1,x2,x3,rk) =  x0^sm4_T(x1 ^ x2 ^ x3 ^ rk)
*/
static unsigned int sm4_F(unsigned int x0, unsigned int x1, unsigned int x2, unsigned int x3, unsigned int rk) {
    return (x0^sm4_T(x1^x2^x3^rk));
}

void keyexpand(unsigned int *key, unsigned int rk[32]) {
	int i = 0;
	unsigned int K[36] = {0};

	for(i=0; i<4; i++)
		K[i]  = key[i] ^ FK[i];
	for(i=4; i<36; i++)
		K[i] = K[i-4]^T1(K[i-3]^K[i-2]^K[i-1]^CK[i-4]);
   
	for(i=0; i<32; i++)
		rk[i]  = K[i] ^ T1(K[i+1]^K[i+2]^K[i+3]^CK[i]);
}

//单体加密
void sm4_enc(unsigned int *input,  unsigned int *output) {
	int i=0;
	unsigned int state[36] = {0}; 
	
	for(i=0; i<4; i++)
		state[i] = input[i];
	for(i=4;i<36;i++)
		state[i] = sm4_F(state[i-4], state[i-3], state[i-2], state[i-1], rk[i-4]);
   
	output[0] = state[35]; output[1] = state[34]; output[2] = state[33]; output[3] = state[32];
}

//单体解密
void sm4_dec(unsigned int *input,  unsigned int *output) {
	int i=0;
	unsigned int state[36] = {0}; 

	for(i=0; i<4; i++)
		state[i] = input[i];
	for(i=4;i<36;i++)
		state[i] = sm4_F(state[i-4], state[i-3], state[i-2], state[i-1], rk[35-i]);
   
	output[0] = state[35]; output[1] = state[34]; output[2] = state[33]; output[3] = state[32];

}

//SM4_ECB加密模式
unsigned int SM4_ECB_ENC(unsigned char *input, 
				 unsigned int input_length, 
				 unsigned char *key, 
				 unsigned char *output) {
	unsigned int i, block_num;
  unsigned int left;
	unsigned int plain[4]={0x00};
	unsigned int cipher[4]={0x00};
	unsigned int  key_int[4] = {0x0};
	unsigned char temp[16];

  if(input_length<0) return 0;
	block_num = input_length/16;
	left = input_length%16;

  char2int(key, key_int, 16);
	keyexpand(key_int, rk);

	for(i=0; i<block_num; i++) {
		char2int(input, plain, 16);
		sm4_enc(plain,cipher);
		int2char(cipher, output, 4);
		input  +=16;
		output +=16;
	}

	for(i=0; i<left; i++)
		   temp[i] = input[i];
	for(i=left; i<16; i++)
		   temp[i] = (16-left);
	char2int(temp, plain, 16);
	sm4_enc(plain, cipher);
	int2char(cipher, output, 4);

	return 16*block_num+16;
}

unsigned int SM4_ECB_DEC(unsigned char *input, 
				 unsigned int input_length, 
				 unsigned char* key, 
				 unsigned char *output) {
	unsigned int i, block_num;
  unsigned int left;
	unsigned int plain[4]={0x00};
	unsigned int cipher[4]={0x00};
	unsigned int  key_int[4] = {0x0};
	unsigned char temp[16];

	block_num = input_length/16;
	left = input_length%16;
	if(left)return 0;

  char2int(key, key_int, 16);
	keyexpand(key_int, rk);

	for(i=0; i<(block_num-1); i++) {
		char2int(input, cipher, 16);
		sm4_dec(cipher, plain);
		int2char(plain, output, 4);
		input  +=16;
		output +=16;
	}

	char2int(input, cipher, 16);
	sm4_dec(cipher, plain);
  int2char(plain, temp, 4);
  for(i=0; i<(16-temp[15]); i++)
    output[i] = temp[i];

	return (16*(block_num-1)+16-temp[15]);
}


//SM4_CBC 加密模式
unsigned int SM4_CBC_ENC(unsigned char *IV, 
				 unsigned char *input, 
				 unsigned int  input_length, 
				 unsigned char *key, 
				 unsigned char *output)
{
	unsigned int i, j, block_num;
    unsigned int left;
	unsigned int plain[4]={0x00};
	unsigned int cipher[4]={0x00};
	unsigned int  key_int[4] = {0x0};
	unsigned char temp[16];
	unsigned char IV_copy[16];

	memcpy(IV_copy, IV, 16);

    if(input_length<0)return 0;
	block_num = input_length/16;
	left = input_length%16;

    char2int(key, key_int, 16);
	keyexpand(key_int, rk);

	for(i=0; i<block_num; i++)
	{	
        for(j=0; j<16; j++)
			input[j]^=IV_copy[j];
		char2int(input, plain, 16);
		sm4_enc(plain,cipher);
		int2char(cipher, output, 4);
		memcpy(IV_copy, output, 16);
		input  +=16;
		output +=16;
	}

	for(i=0; i<left; i++)
		   temp[i] = input[i];
	for(i=left; i<16; i++)
		   temp[i] = (16-left);
	for(j=0; j<16; j++)
		temp[j]^=IV_copy[j];

	char2int(temp, plain, 16);
	sm4_enc(plain, cipher);
	int2char(cipher, output, 4);

	return 16*block_num+16;

}

unsigned int SM4_CBC_DEC(unsigned char *IV,
				 unsigned char *input, 
				 unsigned int  input_length,
				 unsigned char *key, 
				 unsigned char *output)
{
	unsigned int i, j,block_num;
    unsigned int left;
	unsigned int plain[4]={0x00};
	unsigned int cipher[4]={0x00};
	unsigned int  key_int[4] = {0x0};
	unsigned char temp[16];
	unsigned char IV_copy[16];

	memcpy(IV_copy, IV, 16);

	block_num = input_length/16;
	left = input_length%16;
	if(left)return 0;

    char2int(key, key_int, 16);
	keyexpand(key_int, rk);

	for(i=0; i<(block_num-1); i++)
	{
		char2int(input, cipher, 16);
		sm4_dec(cipher, plain);
		int2char(plain, output, 4);
		for(j=0; j<16; j++)
		{
			output[j]^=IV_copy[j];
			IV_copy[j]=input[j];
		}
		input  +=16;
		output +=16;
	
	}

	char2int(input, cipher, 16);
	sm4_dec(cipher, plain);
	int2char(plain, temp, 4);
	for(j=0; j<16; j++)
	{
		temp[j]^=IV_copy[j];
	}
    for(i=0; i<(16-temp[15]); i++)
		output[i] = temp[i];

	return (16*(block_num-1)+16-temp[15]);
}


