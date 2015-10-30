#ifndef __ASMUTILITY_H__
#define __ASMUTILITY_H__

#include "types.h"

BYTE kinport_byte(WORD port);
void koutport_byte(WORD port, BYTE data);

#endif /* __ASMUTILITY_H__ */
