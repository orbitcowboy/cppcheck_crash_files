typedef struct {
  short n[42];
} BLK;
BLK hdr;

char bitmap[512];

void garbage() {
  for (int i = 1; i < 42; i++)
      (bitmap[hdr.n[i] >> 1]) &= 1;
}
// from http://trac.cppcheck.net/ticket/8197
