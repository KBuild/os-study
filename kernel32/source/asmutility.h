#ifndef __ASMUTILITY_H__
#define __ASMUTILITY_H__

#include "types.h"

BYTE kinport_byte(WORD port);
void koutport_byte(WORD port, BYTE data);
void kload_gdtr(QWORD gdtr_addr);
void kload_tr(WORD tss_segment_offset);
void kload_idtr(QWORD idtr_addr);

#endif
