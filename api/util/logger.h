#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <stdio.h>

#define LOG(fp, level, fn, line, file, prefix, ...)                            \
  {                                                                            \
    fprintf(fp, "[%s:%s(%d)] [%s/%s]: ", file, fn, line, level, prefix);       \
    fprintf(fp, __VA_ARGS__);                                                  \
  }

#define LOGI(...)                                                              \
  LOG(stdout, "INFO", __FUNCTION__, __LINE__, __FILE_NAME__, __VA_ARGS__)

#define LOGW(...)                                                              \
  LOG(stdout, "WARN", __FUNCTION__, __LINE__, __FILE_NAME__, __VA_ARGS__)

#define LOGE(...)                                                              \
  LOG(stdout, "ERR ", __FUNCTION__, __LINE__, __FILE_NAME__, __VA_ARGS__)

#if defined(DEBUG)
#define LOGD(...)                                                              \
  LOG(stdout, "DBG ", __FUNCTION__, __LINE__, __FILE_NAME__, __VA_ARGS__)
#else
#define LOGD(...)
#endif /** defined(DEBUG) */

#endif /** UTIL_LOGGER_H */
