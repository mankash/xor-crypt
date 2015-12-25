#include <stdio.h>
#include <string.h>
//#include "inttypes.h"

void encFile(FILE *inFile, int key);
void decFile(FILE *inFile, int key);

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

#define INT_ENC( in, key, out) \
        (out) = (in) ^ ( key )

#define CHAR_ENC( in, key, out) \
(out) = (in) ^ 'N'


int main (int argc, char *argv[]) {
    FILE *inFile = NULL;
    int key = 0;
    char inSel = 'v';
    
     //printf("\ncmdline args count=%d", argc);
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
}

void decFile(FILE * inFile, int key)
{

}