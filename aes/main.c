#include <stdio.h>
#include <time.h>
#include <string.h>
#include "aes.h"


//Tested  2021; AES-128 is Tested with AES Stardard;

int main() {
	int i;
	//Test Data;
	//const unsigned char in[16] = { 0x1 };
  unsigned char in[16] = "123";
	unsigned char out[16] = { 0x1 };
	unsigned char usrkey[16] = "1";

	//File test
	clock_t start, end;
	char file_path[200]; //
	char file_out_path[200];
	FILE *fp = NULL;
	FILE *fp_out = NULL;

	char buf[100];
	memset(buf, 0, 100);

	printf("\nPlease input the file path:\n");
	scanf("%s", file_path);
	printf("file_path = %s\n", file_path);


	fp = fopen(file_path, "rb+");
	if (NULL == fp)
	{
		printf("file open failed!\n");
		return 1;
	}

	int file_path_len = strlen(file_path);
	memcpy(file_out_path, file_path, file_path_len);
	file_out_path[file_path_len] = '.';
	file_out_path[file_path_len + 1] = 'm';
	file_out_path[file_path_len + 2] = '\0';
	printf("file_out_path = %s\n", file_out_path);
	fp_out = fopen(file_out_path, "w+");
	if (NULL == fp_out)
	{
		printf("file_out create failed!\n");
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	long long file_len = ftell(fp);
	printf("file_len = %u,\n", file_len);
	fseek(fp, 0, SEEK_SET);
	long long file_block64 = file_len / 64;
	long file_left = file_len % 64;
	
	long long file_block16 = file_len / 16;
	if (file_block16 < 1)printf("msg len<16, please use single block enc");
	long long file_left16 = file_len % 16;


	AES_KEY key;

	for (i = 0; i < 16; i++)
	{
		in[i] = (16 * i + i);
		usrkey[i] = i;
	}

	//AES_set_encrypt_key(usrkey, 128, &key);
	//AES_encrypt(in, out, &key);  // a single block enc;
	//AES_set_decrypt_key(usrkey, 128, &key); 
	//AES_decrypt(out, in, &key);

	//File test
	start = clock();

	AES_set_encrypt_key(usrkey, 128, &key);
	for (i = 0; i < file_block16; i++)
	{
		fread(buf, 1, 16, fp);
		AES_encrypt(buf, out, &key);
		fwrite(buf, 1, 16, fp_out);
	}
	end = clock();

  double spend = (end - start) / CLOCKS_PER_SEC;
	printf("enc time is %f seconds\n", spend);

	for (i = 0; i < 16; i++)
		printf("0x%02x,", out[i]);

	fclose(fp);
	fclose(fp_out);

  return 0;
}