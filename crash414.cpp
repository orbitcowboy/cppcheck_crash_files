typedef union {
  char c[1024];
  unsigned short n[(1024 / sizeof(unsigned short))];
} BLK;
extern BLK hdr;

static unsigned char bitmap[512];

void garbage() {
  for (int i = 1; i < (1024 / sizeof(unsigned short)); i++) {
      (bitmap[hdr.n[i] >> 1]) &= 1;
  }
}
// from http://trac.cppcheck.net/ticket/8197
