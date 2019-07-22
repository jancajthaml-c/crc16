/*
static inline unsigned reverse(unsigned x) {
   x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
   x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) << 4) | ((x >> 4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}*/

// https://people.cs.umu.se/isak/snippets/crc-16.c
int crc16(const void* data, const unsigned polynomial, const int init, const int xorout) {
  const unsigned char* current = (const unsigned char*) data;
  //unsigned crc = ((unsigned)init), byte, c, v, tcrc, i;

  unsigned short crc = init;
  unsigned byte;
  unsigned short p0 = polynomial;
  //unsigned short i, v, tcrc = 0;

  while ((byte = *current++) != 0) {
    p0 = ((crc>>8) ^ byte) & 0xff;
    p0 ^= p0>>4;
    crc = (crc << 8) ^ (p0 << 12) ^ (p0 <<5) ^ p0;
    crc &= 0xffff;
   }
   return crc ^ xorout;
}

