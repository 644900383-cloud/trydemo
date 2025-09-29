
//
// strdup.c
//
// Copyright (c) 2014 Stephen Mathieson
// MIT licensed
//

#ifndef HAVE_STRDUP

#include <stdlib.h>
#include <string.h>
#include "strdup.h"

#ifndef strdup

char *
strdup(const char *str) {
  if (str == NULL) {
    return NULL;
  }

  size_t len = strlen(str) + 1;
  char *buf = (char *)malloc(len);
  if (buf == NULL) {
    return NULL;
  }
  memcpy(buf, str, len); // Copy including the null terminator
  return buf;
}

#endif

#endif /* HAVE_STRDUP */
