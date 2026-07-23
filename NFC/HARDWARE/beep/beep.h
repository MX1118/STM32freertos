#ifndef __beep_H
#define __beep_H
#include "stm32f4xx.h"


#define BEEP PFout(8)
void BEEP_Init(void);

#endif



