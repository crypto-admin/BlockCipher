//
// Created by crypto-lib on 2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sha1.h"

unsigned int state[5]={0x0};

static const unsigned char padding[64] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned int K[4]={0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define Ch(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define Parity(x, y, z) (x^y^z)


void SHA1_OneRound(unsigned char *msg) {
  unsigned int W[80];
  int i;
  unsigned int T,a,b,c,d,e;

  for(i = 0; i < 16; i++)
    W[i] = (unsigned int)(msg[4*i]<<24) | (unsigned int)(msg[4*i+1]<<16) | (unsigned int)(msg[4*i+2]<<8) | (unsigned int)msg[4*i+3];
  for(i = 16; i < 80; i++)
    W[i] = ROTL(W[i-3]^W[i-8]^W[i-14]^W[i-16], 1);

  a=state[0];
  b=state[1];
  c=state[2];
  d=state[3];
  e=state[4];

  for(i = 0; i < 80; i++) {
    if(i < 20)    T = ROTL(a,5)+Ch(b,c,d)+e+K[0]+W[i];

    if(19<i&&i<40) T = ROTL(a,5)+Parity(b,c,d)+e+K[1]+W[i];

    if(39<i&&i<60) T = ROTL(a,5)+Maj(b,c,d)+e+K[2]+W[i];

    if(59<i&&i<80) T = ROTL(a,5)+Parity(b,c,d)+e+K[3]+W[i];

    e=d;
    d=c;
    c=ROTL(b,30);
    b=a;
    a=T;
  }
  state[0]+=a;
  state[1]+=b;
  state[2]+=c;
  state[3]+=d;
  state[4]+=e;

}
////////////////////////////////////////////////////////////
//          SHA1_Init+SHA1_Process: User API
//          One-Time Hash, msg_len<2^29
//          Imple:
//                 1. SHA1_INIT
//                 2. SHA1_Process
//          Output: state[5] = 160bit
////////////////////////////////////////////////////////////
void SHA1_Init() {
  state[0]=0x67452301;
  state[1]=0xefcdab89;
  state[2]=0x98badcfe;
  state[3]=0x10325476;
  state[4]=0xc3d2e1f0;
}

void  SHA1_Process(unsigned char *msg, unsigned  char *output) {
  //msg_len<2^29
  unsigned int i;
  unsigned char last[64]={0x0};
  unsigned int msg_len=strlen(msg);

  unsigned int sha1blocknum = msg_len/64;
  unsigned int sha1lastlen=msg_len%64;

  for (i = 0; i < sha1blocknum; i++)
    SHA1_OneRound(msg+64*i);

  for (i = 0; i < sha1lastlen; i++)
    last[i]=msg[msg_len-sha1lastlen+i];
  for(i = sha1lastlen; i < 64; i++)
    last[i]=padding[i-sha1lastlen];
  if(sha1lastlen > 55) {
    SHA1_OneRound(last);
    for(i = 0; i < 64; i++) last[i]=0;
  }
  last[63] = (msg_len<<3)&0xff;
  last[62] = ((msg_len<<3)>>8) &0xff;
  last[61] = ((msg_len<<3)>>16)&0xff;
  last[60] = ((msg_len<<3)>>24)&0xff;
  SHA1_OneRound(last);
  for (int i = 0; i < 5; i++) {
    output[4*i] = (unsigned  char)(state[i] & 0xff);
    output[4*i + 1] = (unsigned  char)((state[i] >> 8) & 0xff);
    output[4*i + 2] = (unsigned  char)((state[i] >> 16) & 0xff);
    output[4*i + 3] = (unsigned  char)((state[i] >> 24) & 0xff);
  }
}