
#ifndef _SM4_H_
#define _SM4_H_


void int2char(unsigned int* in, unsigned char* out, int num);
void char2int(unsigned char*in, unsigned int* out, int num);

unsigned int SM4_ECB_ENC(unsigned char *input, 
				 unsigned int input_length, 
				 unsigned char *key, 
				 unsigned char *output);

unsigned int SM4_ECB_DEC(unsigned char *input, 
				 unsigned int input_length, 
				 unsigned char* key, 
				 unsigned char *output);

unsigned int SM4_CBC_ENC(unsigned char *IV, 
				 unsigned char *input, 
				 unsigned int  input_length, 
				 unsigned char *key, 
				 unsigned char *output);

unsigned int SM4_CBC_DEC(unsigned char *IV,
				 unsigned char *input, 
				 unsigned int  input_length,
				 unsigned char *key, 
				 unsigned char *output);


#endif