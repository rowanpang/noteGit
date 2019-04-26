#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
/*
 *ref
 *    https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html
 */

#define CST 11246805480246805480ULL

static uint64_t load64_le(uint8_t const* V)
{
#if __BYTE_ORDER !=__ORDER_LITTLE_ENDIAN__
    #error This code only works with little endian systems
#endif
  uint64_t Ret = *((uint64_t const*)V);
  return Ret;
}

static uint64_t hash(const uint8_t* Data, const size_t Len)
{
   uint64_t Ret = 0;
   int blockSize=sizeof(uint64_t);
   const size_t NBlocks = Len/blockSize;
   for (size_t I = 0; I < NBlocks; ++I) {
	const uint64_t V = load64_le(&Data[I*blockSize]);
	Ret = (Ret ^ V)*CST;
   }
   uint64_t LastV = 0;
   for (size_t I = 0; I < (Len-NBlocks*blockSize); ++I) {
	//*8 meaning: sizeof(uint8_t)=8
	LastV |= ((uint64_t)Data[NBlocks*blockSize+I]) << (I*8);
   }
   Ret = (Ret^LastV)*CST;
   return Ret;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s string\n", argv[0]);
    return 1;
  }
  const char* Str = argv[1];
  const uint64_t H = hash((const uint8_t*) Str, strlen(Str));
  printf("%016" PRIX64 "\n", H);
  return 0;
}
