#include <stdio.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  // Alignment check for mmap below should be consistent with the reported page size
  // For now, just require it to be 4096
  assert(getpagesize() == 4096);
  assert(sysconf(_SC_PAGESIZE) == 4096);

  for (int i = 0; i < 10; i++) {
    int* map = (int*)mmap(0, 5000, PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANON, -1, 0);
    assert(map != MAP_FAILED);
    assert(((int)map) % 4096 == 0); // aligned
    assert(munmap(map, 5000) == 0);
  }

  const int NUM_BYTES = 8 * 1024 * 1024;
  const int NUM_INTS = NUM_BYTES / sizeof(int);

  int* map = (int*)mmap(0, NUM_BYTES, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANON, -1, 0);
  assert(map != MAP_FAILED);

  int i;

  for (i = 0; i < NUM_INTS; i++) {
    map[i] = i;
  }

  for (i = 0; i < NUM_INTS; i++) {
    assert(map[i] == i);
  }

  assert(munmap(map, NUM_BYTES) == 0);

  printf("hello,world");
  return 0;
}
