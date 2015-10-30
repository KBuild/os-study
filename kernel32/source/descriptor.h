#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "types.h"

//IDT

#pragma pack(push, 1)

typedef struct k_gdtr {
    WORD limit;
    DWORD base;
} GDTR, IDTR;

typedef struct k_descriptor {
    WORD lower_limit;
    WORD lower_base;
    BYTE upper_base;
    BYTE type_lower_flag; // TYPE, S, DPL, P
    BYTE upper_limit_upper_flag; // segment limit, avl, l, d/b, g
    BYTE upper_base2;
} GDTENTRY;

typedef struct k_idt_entry {
    WORD offset1; 
    WORD selector;
    BYTE zero;
    BYTE type_attr;
    WORD offset2;
} IDTENTRY;

typedef struct k_tss_data {
    WORD prev_task_link;
    WORD reserved0;
    WORD reg_ss0;
    WORD reserved1;
    WORD reg_ss1;
    WORD reserved2;
    WORD reg_ss2;
    WORD reserved3;
    DWORD reg_cr3;
    DWORD reg_eip;
    DWORD reg_eflags;
    DWORD reg_eax;
    DWORD reg_ecx;
    DWORD reg_edx;
    DWORD reg_ebx;
    DWORD reg_esp;
    DWORD reg_ebp;
    DWORD reg_esi;
    DWORD reg_edi;
    WORD reg_es;
    WORD reserved4;
    WORD reg_cs;
    WORD reserved5;
    WORD reg_ss;
    WORD reserved6;
    WORD reg_ds;
    WORD reserved7;
    WORD reg_fs;
    WORD reserved8;
    WORD reg_gs;
    WORD reserved9;
    WORD ldt_seg_selector;
    WORD reserved10;
    WORD reserved11;
    WORD iomap_base_addr;
} TSSSEGMENT;

#pragma unpack(pop)

#endif
