// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_mm.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 // Only 1KB for testins

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void test_mm()
{

  printfColor("\nStartin test... \n", white);

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  rq = 0;
  total = 0;

  // Request as many blocks as we can
  while (rq < MAX_BLOCKS && total < MAX_MEMORY)
  {
    mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
    mm_rqs[rq].address = malloc(mm_rqs[rq].size);

    if (mm_rqs[rq].address)
    {
      total += mm_rqs[rq].size;
      printfColor("Block %d:  %d\n", white, rq, mm_rqs[rq].size);
      rq++;
    }
  }

  // Set
  uint32_t i;
  for (i = 0; i < rq; i++)
    if (mm_rqs[i].address)
      memset(mm_rqs[i].address, i, mm_rqs[i].size);

  // Check
  for (i = 0; i < rq; i++)
    if (mm_rqs[i].address)
      if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
      {
        printfColor("test_mm ERROR: Problems with Block %d\n", white, i);
        return;
      }

  // Free
  for (i = 0; i < rq; i++)
  {
    if (mm_rqs[i].address)
    {
      free(mm_rqs[i].address);
    }
  }

  printfColor("\nTest succesfully passed! :)\n", pink);
}
