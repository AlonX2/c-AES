#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
union uKey
{
  unsigned int int32;
  unsigned char bytes[4];
};
  void printCharArr(unsigned char* arr, size_t size)
  {
    for (int i = 0; i < size/4; i++)
    {
      // if(i%4 == 0 && i>0)
      //   printf(" ");
      union uKey data;
      for (int j = 0; j < 4; j++)
        data.bytes[j] = arr[i*(size/4) + j];

      
      printf("%x\n", data.int32);
    }
    printf("\n");
  }
int main(int argc, char ** argv){
    unsigned char fag[]= {0x15, 0x54, 0x98, 0x45, 0x15, 0x22, 0x78,0xf1,0x22, 0x78,0xf1,0x22, 0x78,0xf1, 0x8b, 0x45};
    printCharArr(fag, 16);
}