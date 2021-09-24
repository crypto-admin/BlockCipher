/* sch.h */

#ifndef HEADER_SCH_H
#define HEADER_SCH_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned int
#endif

typedef struct
{
    uint32 total[2];
    uint32 state[8];
    uint8 buffer[64];
}
sm3_context;

int sm3_256( unsigned int datalen_in, unsigned char *pdata_in, unsigned char digest[32]);
#ifdef  __cplusplus
}
#endif

#endif