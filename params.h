#ifndef paramH
#define paramH

#include <math.h>

//#define u8 unsigned char
//#define u16 unsigned short
//#define u32 unsigned int

const unsigned int AVG_DATA_LEN=1024*8*4;
const unsigned int MAX_PKT_DATA_LEN=1024;

const double Fd=2400000000.0f;
const double Td=(1.0/Fd);

const double Ti=10000*1e-6f;
const double Di=5*1.0e-6f;
const unsigned LenN=floor(Di/Td);

const double LFM_dev=300*1e6f;
const double LFM_step=(LFM_dev/LenN / 2);

const double Noise=0;

#endif
