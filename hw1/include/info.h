#ifndef INFO_H
#define INFO_H
#include <stdlib.h>
#include <stdio.h>

#ifdef INFO
	#define info(fmt, ...) do{fprintf(stderr, fmt, ##__VA_ARGS__);}while(0)
#else
	#define info(fmt, ...)
#endif

#endif /* DEBUG_H */