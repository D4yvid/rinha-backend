#ifndef UTIL_ERROR_H
#define UTIL_ERROR_H

typedef struct Error {
  const char *error;
  const char *description;
} *Error;

#define FAIL(n) (n != NULL)
#define SUCCESS() NULL
#define TRY(op, err)                                                           \
  struct Error *err;                                                           \
  if (FAIL((err = (op))))
#define ERR(err, desc) (&(struct Error){.error = err, .description = desc})

#endif /** UTIL_ERROR_H */
