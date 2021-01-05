#ifndef LPRINTF_H_
#define LPRINTF_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

void lprintf(uint32_t moduleInstance, char *, ...);
void setupPrint(void);

#endif /* LPRINTF_H_ */
