    extern int __getrpcbyname_r(const char *name, struct rpcent *resbuf,
                                char *buffer, size_t buflen,
                                struct rpcent **result) attribute_hidden;


struct rpcent *getrpcbyname(const char *name) {
  static size_t buffer_size;
  static struct rpcent resbuf;
  struct rpcent *result;


  if (buffer == NULL) {
    buffer_size = 1024;
    buffer = (char *)malloc(buffer_size);
  }

  while (buffer != NULL && (__getrpcbyname_r(name, &resbuf, buffer, buffer_size,
                                             &result) == ERANGE)) {  }

  return result;
}

// from https://trac.cppcheck.net/ticket/8714
