#ifndef LPRINTF_H_
#define LPRINTF_H_

#include <stdint.h>

void lprintf(uint32_t moduleInstance, char *, ...);
void setupPrint(void);

#endif /* LPRINTF_H_ */
