#pragma once
#include <stdint.h>

#ifndef ID_TYPE
typedef uint16_t IDType;
#endif
#ifndef ID_TYPE_SIZE
static const unsigned int ID_TYPE_SIZE_BITS = 16;
#endif
#ifndef MAX_DATA_SIZE
static const unsigned int MAX_DATA_SIZE = 1024;
#endif
#ifndef DEFAULT_ERROR_MODE
static const int DEFAULT_ERROR_MODE = 1;
#endif

static const unsigned int ID_TYPE_SIZE_BYTES = ID_TYPE_SIZE_BITS / 8;
static const char UNDEFINED_LOC = 'Z';
static const int THROW_ON_ERROR = 0;
static const int RETURN_ON_ERROR = 1;
static const int RECALL_ON_ERROR = 2;
static const int CONTINUE_ON_ERROR = 3;