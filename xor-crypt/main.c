#include "xor-crypt.h"

//#define AS_LIB
#define AS_BUF

int fixKey()
{
    uint8_t char_key[] = {'N', 'E', 'I', 'L'}; //Can be any chars, and any size array
    int key = 0;
    
    if(sizeof(key) == 4)
    {
        key = (char_key[0]<< 24 ) | (char_key[1]<< 16 ) | (char_key[2]<< 8 ) | (char_key[3]<< 0 );
    }
    else
    {
        key = (char_key[0]<< 24 ) | (char_key[1]<< 16 ) | (char_key[2]<< 8 ) | (char_key[3]<< 0 );
        key = key | (key << 32);
    }
    
    return key;

}

#ifndef AS_LIB
#define ENC_XTRA_PADDING (1024)

int main (int argc, char *argv[]) {
    FILE *inFile = NULL;
    int key = 0;
    char inSel = 'v';
    
    
    if(argc != 3)
    {
        printf("Undefined Usage. Correct usage: %s e/u fileName \n", argv[0]);
        return -1;
    }
    
    key = fixKey();
    
    /* Third argument is input file name  */
    inFile = fopen(argv[2], "rb") ;
    if(!inFile)
    {
        printf("Filed to open file %s \n", argv[2]);
        return -3;
    }
    
    inSel = *argv[1];
   switch(inSel)
    {
        case 'e':
        case 'd':
            encFile(inFile,  key);
            break;
        default:
        {
            printf("Undefined Usage. Correct usage: %s e/u fileName \n", argv[0]);
            return -2;
        }
            
    }

    fclose(inFile);
    return 0;
    
}

#endif

void encFile(FILE * inFile, int key)
{
    int inInt=0, outInt = 0, intSz = 0;
     long lSize, cnt = 0;
    FILE * encFile = NULL;
    
    // obtain file size:
    fseek (inFile , 0 , SEEK_END);
    lSize = ftell (inFile);
    rewind (inFile);
    
    intSz = sizeof(inInt);
    
    encFile = fopen("outFile.txt", "wb");
    
    if(!encFile)
    {
        printf("Filed to open output file \n");
        exit(0);
    }

    #ifndef AS_BUF
    
    while(cnt < lSize)
    {
        if((cnt + intSz) < lSize )
        {
            fread(&inInt, intSz, 1, inFile);
            INT_ENC(inInt, key, outInt);
            fwrite(&outInt, intSz, 1, encFile);
            cnt = cnt + intSz;
        }
        else
        {
            uint loop = lSize - cnt;
            
            cnt = cnt + loop;
            
            while(loop)
            {
                char in,out;
                fread(&in, 1, 1, inFile);
                CHAR_ENC(in, key, out);
                fwrite(&out, 1, 1, encFile);
                loop = loop - 1;
            }
            
        }
        
    }

    fclose(encFile);

    #else /*ifndef AS_BUF*/
    {
        uint8_t * crypt_buf = malloc(lSize + ENC_XTRA_PADDING);
        uint8_t * inBuf = malloc(lSize);

        if(inBuf != NULL)
        {
            fread(inBuf, 1, lSize, inFile);
        }

        int err = encBuf(inBuf,(uint64_t) lSize, crypt_buf);

        {
               fwrite(crypt_buf, 1, lSize, encFile);
                free(crypt_buf);
               fclose(encFile);
        }
    }
    #endif
}

void decFile(FILE * inFile, int key)
{

}

int encBuf(uint8_t * inBuf,uint64_t szInBuf,uint8_t * outBuf)
{
    uint64_t cnt = 0;
    int key = 0, inInt, outInt;
    int intSz = sizeof(key);
    uint64_t offset = 0;

    if(inBuf == NULL)
    {
        return (int) ENC_ERR_IN_BUF_NUL;
    }

    if(outBuf == NULL)
    {
        return (int) ENC_ERR_OUT_BUF_NUL;
    }

    key = fixKey();

    while(cnt <szInBuf)
    {
        if((cnt + intSz) < szInBuf )
        {
            memcpy(&inInt, (inBuf + offset), sizeof(inInt) );
            INT_ENC(inInt, key, outInt);
            memcpy((outBuf + offset), &outInt, sizeof(outInt));
            cnt = cnt + intSz;
            offset = offset + intSz;
        }
        else
        {
            uint loop = szInBuf- cnt;
            uint rem = sizeof(int) - loop;
            uint8_t * byteIn = (uint8_t *) &inInt;
            uint tmpCnt = 0;
            uint8_t zero = 0;
            
            cnt = cnt + loop;
            
            while(loop)
            {
                memcpy((byteIn + tmpCnt), (inBuf + offset + tmpCnt), 1);
                loop = loop - 1;
                tmpCnt = tmpCnt + 1;
            }
            
            //offset = offset + tmpCnt;
            //tmpCnt = 0;
            
            while(rem)
            {
                memcpy((byteIn + tmpCnt ), &zero, 1);
                rem  = rem - 1;
                tmpCnt = tmpCnt + 1;
            }
            
            INT_ENC(inInt, key, outInt);
            memcpy((outBuf + offset), &outInt, sizeof(outInt));
            offset = offset + tmpCnt;
            tmpCnt = 0;

        }
        
    }


    return ENC_NO_ERR; 
}