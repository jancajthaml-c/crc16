int crc16(const void* data, const unsigned int polynomial, const int init, const int xor_out);

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

double diff(double start, double end) {
  if ((end - start) < 0) {
  return 1000000000 + end - start;
  } else {
  return end - start;
  }
  return 0;
}

static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size) {
    char *s = malloc(size + 1);
    if (s) {
      rand_string(s, size);
    }
    return s;
}

void expectCrc16(const char* label, const void* data, const unsigned int polynomial, const int init, const int xor_out, const int checksum) {
  const int d = crc16(data, polynomial, init, xor_out);

  if (d != checksum) {
    printf("%s : `%s` expected checksum 0x%2.2x but got 0x%2.2x instead\n\n", label, data, checksum, d);
    exit(EXIT_FAILURE);
  }
}

#define CCITT

int main() {
  //printf("\nRunning unit tests\n");

  //expectCrc32("test-hand", "a", 0xEDB88320, 0x11);

  // expectations
  // zero data
  expectCrc16("CCIT",         "", 0x1021, 0xFFFF, 0x0000, 0xFFFF);

  // single char
  expectCrc16("CCIT",         "a", 0x1021, 0xFFFF, 0x0000, 0x29B1);

  // medium string
  expectCrc16("CCIT",         "00123014764700968325", 0x1021, 0xFFFF, 0x0000, 0xD01B);


  /*
  expectCrc32("test1", "a", 0x04C11DB7, 0xE8B7BE43);

  expectCrc32("test2", "00123014764700968325", 0xA833982B, 0xD303B3F3);
  expectCrc32("test3", "00123014764700968325", 0x04C11DB7, 0x57d311bb);
  expectCrc32("test4", "00123014764700968325", 0x1EDC6F41, 0x660BC80D);
  expectCrc32("test5", "00123014764700968325", 0xA833982B, 0xD303B3F3);
  expectCrc32("test6", "1234567812345678", 0x11c4dfb5, 0xed52dcf8);
  */

  //printf("passed");

  // micro benchmark
  int i;
  unsigned long long times = 1000;

  size_t big_string_size = 1000000;
  char* big_string = rand_string_alloc(big_string_size);

  size_t small_string_size = 10;
  char* small_string = rand_string_alloc(small_string_size);

  //printf("\nRunning benchmark %lu times \n", (unsigned long) times);

  double total_time_big = 0;
  double total_time_small = 0;
  double clock_cost = 0;

  struct timespec t1;
  struct timespec t2;

  for (i = 0; i < times; ++i) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    clock_cost += diff(t1.tv_nsec, t2.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    crc16(big_string, 0x1021, 0xFFFF, 0xFFFF);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    total_time_big += diff(t1.tv_nsec, t2.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    crc16(small_string, 0x1021, 0xFFFF, 0xFFFF);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    total_time_small += diff(t1.tv_nsec, t2.tv_nsec);
  }

  total_time_big = ((total_time_big / times) / big_string_size);
  total_time_small = (((total_time_small - clock_cost) / times) / small_string_size);

  printf("((N * %f) + %f) ns/op\n", total_time_big, total_time_small);

  return 0;
}
