#include <stdio.h>
#include <string.h>
#include <inttypes.h>


void encFile(FILE *inFile, int key);
void decFile(FILE *inFile, int key);

int encBuf(uint8_t * inBuf, uint64_t szInBuf, uint8_t * outBuf);
int decBuf(uint8_t * inBuf, uint64_t szInBuf, uint8_t * outBuf);


typedef enum  {

    ENC_NO_ERR = 0,
    ENC_ERR_IN_BUF_NUL = -1,
    ENC_ERR_OUT_BUF_NUL = -2,
    ENC_ERR_INTERRUPTED = -3,

} encErrors;

#define INT_ENC( in, key, out) \
        (out) = (in) ^ ( key )

#define CHAR_ENC( in, key, out) \
(out) = (in) ^ 'N'

