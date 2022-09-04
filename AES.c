#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NEWSIZE(x) ((x + 15)/16)*16 

static const unsigned char S_BOX[] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

  static const unsigned char REV_S_BOX[] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

  void printCharArr(unsigned char* arr, size_t size)
  {
    for (int i = 0; i < size/4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if(arr[i*4 + j] < 0x10)
          printf("%hhx",0);
        printf("%hhx",arr[i*4 + j]);
      }

      
      printf(" ");
    }
    printf("\n\n");
  }


void SubBytes(unsigned char* bytes, size_t size)
{
  for (int i = 0; i < size; i++)
    bytes[i] = S_BOX[bytes[i]];
}

void RevSubBytes(unsigned char* bytes, size_t size)
{
  for (int i = 0; i < size; i++)
    bytes[i] = REV_S_BOX[bytes[i]];
}

void ShiftRows(unsigned char* bytes)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < i; j++)
    {
      unsigned char temp = bytes[i];
      bytes[i] = bytes[i + 4];
      bytes[i + 4] = bytes[i + 8];
      bytes[i + 8] = bytes[i + 12];
      bytes[i + 12] = temp;
    }
    
  }
  
}

void RevShiftRows(unsigned char* bytes)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < i; j++)
    {
      unsigned char temp = bytes[i + 12];
      bytes[i + 12] = bytes[i + 8];
      bytes[i + 8] = bytes[i + 4];
      bytes[i + 4] = bytes[i];
      bytes[i] = temp;
    }
    
  }

}


unsigned char Gmul(unsigned char a, unsigned char b)
{

    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for (counter = 0; counter < 8; counter++)
    {
        if ((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void MixColumns(unsigned char* bytes)
{
  for (int i = 0; i < 4; i++)
  {
      unsigned char arr[4];
      for (int j = 0; j < 4; j++)
        arr[j] = bytes[4*i + j];

    bytes[4*i] = Gmul(arr[0], 2) ^ Gmul(arr[1], 3) ^ arr[2] ^ arr[3];
    bytes[4*i + 1] = arr[0] ^ Gmul(arr[1], 2) ^ Gmul(arr[2], 3) ^ arr[3];
    bytes[4*i + 2] = arr[0] ^ arr[1] ^ Gmul(arr[2], 2) ^ Gmul(arr[3], 3);
    bytes[4*i + 3] = Gmul(arr[0], 3) ^ arr[1] ^ arr[2] ^ Gmul(arr[3], 2);

  }

}
void RevMixColumns(unsigned char* bytes)
{
  for (int i = 0; i < 4; i++)
  {
      unsigned char arr[4];
      for (int j = 0; j < 4; j++)
        arr[j] = bytes[4*i + j];

    bytes[4*i] = Gmul(arr[0], 0x0E) ^ Gmul(arr[1], 0x0B) ^ Gmul(arr[2], 0x0D) ^ Gmul(arr[3], 0x09);
    bytes[4*i + 1] = Gmul(arr[0], 0x09) ^ Gmul(arr[1], 0x0E) ^ Gmul(arr[2], 0x0B) ^ Gmul(arr[3], 0x0D);
    bytes[4*i + 2] = Gmul(arr[0], 0x0D) ^ Gmul(arr[1], 0x09) ^ Gmul(arr[2], 0x0E) ^ Gmul(arr[3], 0x0B);
    bytes[4*i + 3] = Gmul(arr[0], 0x0B) ^ Gmul(arr[1], 0x0D) ^ Gmul(arr[2], 0x09) ^ Gmul(arr[3], 0x0E);

  }
}

void AddRoundKey(unsigned char* bytes, unsigned char* key)
{
    for (int i = 0; i < 16; i++)
      bytes[i] ^= key[i];
}


static unsigned char** SplitBlocks(unsigned char *bytes, size_t size)
{
  
  unsigned char** blocks = malloc(size*sizeof(char *)/16 + 1);
  for (int i = 0; i < size/16 + 1; i++)
  {
    unsigned char* arr = malloc(16);
    // printf("fag %p",arr);
    blocks[i] = arr;
  }

  
  for (int i = 0; i < size; i+=16)
  {
    for (int j = 0; j < 16; j++)
    {
      if(j + i >= size) 
      {
        blocks[i/16][j] = 0;
      }
      else
      {
      blocks[i/16][j] = bytes[j + i];
      }
    }
  }
  return blocks;
}

unsigned char* ConnectBlocks(unsigned char **blocks, size_t size)
{
  unsigned char *final = malloc(size);
  for (int i = 0; i < size; i+=16)
  {
    for (int j = 0; j < 16; j++)
    {
      final[j + i] = blocks[i/16][j];
    }
    
  }
  return final;

}

unsigned char* KeyExpansion(unsigned char *key, bool printKey)
{

    static const unsigned int ROUND_CONST[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 }; //change for big endian
    static unsigned char keys[176];

    for (int i = 0; i < 16; i++)
      keys[i] = key[i];

    for (int i = 16; i < 176; i+=4)
    {
      if(i % 16 == 0)
      {
        unsigned char *operand = malloc(4); 

        for (int j = 0; j < 4; j++)   
          operand[j] = keys[i - 4 + (j+1)%4]; //check if right
          
        SubBytes(operand, sizeof(operand));
        
        operand[0] = operand[0] ^ ROUND_CONST[i/16];
        for (int j = 0; j < 4; j++)        
          keys[i + j] = keys[i - 16 + j] ^ operand[j];
        
        free(operand);
        
      }
      else
      { 
        for (int j = 0; j < 4; j++)        
          keys[i + j] = keys[i - 16 + j] ^ keys[i - 4 + j];
        
      }
    }
    if(printKey) printCharArr(keys, sizeof(keys)); 
    return keys;

}


unsigned char* Encrypt(unsigned char *data, unsigned char *key, size_t dataSize, bool showSteps)
{
    static unsigned char* keys;

    if(showSteps) printf("Expanded Key:\n");
    keys = KeyExpansion(key, showSteps);

    unsigned char** blocks = SplitBlocks(data, dataSize);
    dataSize = NEWSIZE(dataSize);

    for (int i = 0; i < dataSize/16; i++)
    {        
      if(showSteps) printf("---------- BLOCK %d ----------\n", i);
      if(showSteps) { printf("input\n"); printCharArr(blocks[i], 16);}
      // if(showSteps) { printf("inputA\n"); printCharArr(blocks[0], 16);}
      // if(showSteps) { printf("inputA\n"); printCharArr(blocks[1], 16);}


      AddRoundKey(blocks[i], (unsigned char[]){keys[0], keys[1], keys[2], keys[3], 
                              keys[4], keys[5], keys[6], keys[7], keys[8], 
                              keys[9], keys[10], keys[11], keys[12], 
                              keys[13], keys[14], keys[15]});
      if(showSteps) { printf("initial xor w\\key\n"); printCharArr(blocks[i], 16);}
      
      for (int j = 1; j < 10; j++)
      {
        if(showSteps) {printf("-----ROUND %d-----\n", j);}

        SubBytes(blocks[i], 16);        
        if(showSteps) { printf("after s-box\n"); printCharArr(blocks[i], 16);}

        ShiftRows(blocks[i]);
        if(showSteps) { printf("after shiftrows\n"); printCharArr(blocks[i], 16);}

        MixColumns(blocks[i]);
        if(showSteps) { printf("after mixcolumns\n"); printCharArr(blocks[i], 16);}

        AddRoundKey(blocks[i], (unsigned char[]){keys[j*16], keys[j*16 + 1], keys[j*16 + 2], keys[j*16 + 3], 
                              keys[j*16 + 4], keys[j*16 + 5], keys[j*16 + 6], keys[j*16 + 7], keys[j*16 + 8], 
                              keys[j*16 + 9], keys[j*16 + 10], keys[j*16 + 11], keys[j*16 + 12], 
                              keys[j*16 + 13], keys[j*16 + 14], keys[j*16 + 15]});
        if(showSteps) { printf("after xor w\\key\n"); printCharArr(blocks[i], 16);}

      }
      if(showSteps) {printf("-----ROUND 10-----\n");}

      SubBytes(blocks[i], 16);
      if(showSteps) { printf("after s-box\n"); printCharArr(blocks[i], 16);}

      ShiftRows(blocks[i]);
      if(showSteps) { printf("after shiftrows\n"); printCharArr(blocks[i], 16);}

      AddRoundKey(blocks[i], (unsigned char[]){keys[10*16], keys[10*16 + 1], keys[10*16 + 2], keys[10*16 + 3], 
                              keys[10*16 + 4], keys[10*16 + 5], keys[10*16 + 6], keys[10*16 + 7], keys[10*16 + 8], 
                              keys[10*16 + 9], keys[10*16 + 10], keys[10*16 + 11], keys[10*16 + 12], 
                              keys[10*16 + 13], keys[10*16 + 14], keys[10*16 + 15]});
      if(showSteps) { printf("after xor w\\key\n"); printCharArr(blocks[i], 16);}

    }
    return ConnectBlocks(blocks, dataSize);

}

unsigned char* Decrypt(unsigned char *data,unsigned char *key, size_t dataSize, bool showSteps)
{
    static unsigned char* keys;

    if(showSteps) printf("Expanded Key:\n");
    keys = KeyExpansion(key, showSteps);

    unsigned char** blocks = SplitBlocks(data, dataSize);
    dataSize = NEWSIZE(dataSize);

    for (int i = 0; i < dataSize/16; i++)
    {
      if(showSteps) { printf("input\n"); printCharArr(blocks[i], 16);}

      AddRoundKey(blocks[i], (unsigned char[]){keys[10*16], keys[10*16 + 1], keys[10*16 + 2], keys[10*16 + 3], 
                              keys[10*16 + 4], keys[10*16 + 5], keys[10*16 + 6], keys[10*16 + 7], keys[10*16 + 8], 
                              keys[10*16 + 9], keys[10*16 + 10], keys[10*16 + 11], keys[10*16 + 12], 
                              keys[10*16 + 13], keys[10*16 + 14], keys[10*16 + 15]});
      if(showSteps) { printf("Reversed final xor w\\key\n"); printCharArr(blocks[i], 16);}



      for (int j = 9; j > 0; j--)
      {
        if(showSteps) {printf("----- Reversing ROUND %d-----\n", j);}
        RevShiftRows(blocks[i]);
        if(showSteps) { printf("Reversed shiftrows\n"); printCharArr(blocks[i], 16);}

        RevSubBytes(blocks[i], 16);
        if(showSteps) { printf("Reversed s-box\n"); printCharArr(blocks[i], 16);}

        AddRoundKey(blocks[i], (unsigned char[]){keys[j*16], keys[j*16 + 1], keys[j*16 + 2], keys[j*16 + 3], 
                              keys[j*16 + 4], keys[j*16 + 5], keys[j*16 + 6], keys[j*16 + 7], keys[j*16 + 8], 
                              keys[j*16 + 9], keys[j*16 + 10], keys[j*16 + 11], keys[j*16 + 12], 
                              keys[j*16 + 13], keys[j*16 + 14], keys[j*16 + 15]});
        if(showSteps) { printf("Reversed xor w\\key\n"); printCharArr(blocks[i], 16);}

        RevMixColumns(blocks[i]);
        if(showSteps) { printf("Reversed mixcolumns\n"); printCharArr(blocks[i], 16);}

      }      

      RevShiftRows(blocks[i]);
      if(showSteps) { printf("Reversed final shiftrows\n"); printCharArr(blocks[i], 16);}

      RevSubBytes(blocks[i], 16);
      if(showSteps) { printf("Reversed final S-box\n"); printCharArr(blocks[i], 16);}

      AddRoundKey(blocks[i], (unsigned char[]){keys[0], keys[1], keys[2], keys[3], 
                              keys[4], keys[5], keys[6], keys[7], keys[8], 
                              keys[9], keys[10], keys[11], keys[12], 
                              keys[13], keys[14], keys[15]});
      if(showSteps) { printf("Reversed initial xor w\\key\n"); printCharArr(blocks[i], 16);}
      
    }

    return ConnectBlocks(blocks, dataSize);
}

int main()
{
  // static unsigned char data[] = {0, 0, 1, 1, 3, 3, 7, 7, 0xf, 0xf, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f}; //should malloc this shit
  static unsigned char data[] = {0xbb, 0x0f, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x4f, 0x4, 0x8}; //should malloc this shit
  // static unsigned char data[] = {0x4, 0x8, 0x0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0};
  // static unsigned char key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
  // static unsigned char key[] = {0x3c , 0x4f, 0xcf, 0x09,  };
  // static unsigned char key[] = {0x16, 0x15, 0x7e, 0x2b, 0xa6, 0xd2, 0xae, 0x28, 0x88, 0x15, 0xf7, 0xab, 0x3c , 0x4f, 0xcf, 0x09};
  static unsigned char key[] = {0xbb, 0x0f, 0x22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00};


  // static unsigned char key[] = {0x54 , 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x67, 0x20, 0x46, 0x7e, 0x75};


  unsigned char* res = Encrypt(data, key, sizeof(data), false);

  // for (int i = 0; i < NEWSIZE(sizeof(data)) ; i++)
  //   printf("%x", res[i]);


  printCharArr(res, NEWSIZE(sizeof(data)));

  unsigned char* org = Decrypt(res, key, NEWSIZE(sizeof(data)), false);

  printCharArr(org, NEWSIZE(sizeof(data)));
  
  // unsigned char fag[17];
  // for (int i = 0; i < 17; i++)
  // {
  //   fag[i] = org[i];
  // }
  
  
  exit(EXIT_SUCCESS);
}
