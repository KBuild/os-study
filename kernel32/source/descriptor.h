#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "types.h"

//==============================================================================
// GDT
//==============================================================================
#define GDT_TYPE_CODE           0x0A
#define GDT_TYPE_DATA           0x02
#define GDT_TYPE_TSS            0x09
#define GDT_FLAGS_LOWER_S       0x10
#define GDT_FLAGS_LOWER_DPL0    0x00
#define GDT_FLAGS_LOWER_DPL1    0x20
#define GDT_FLAGS_LOWER_DPL2    0x40
#define GDT_FLAGS_LOWER_DPL3    0x60
#define GDT_FLAGS_LOWER_P       0x80
#define GDT_FLAGS_UPPER_L       0x20
#define GDT_FLAGS_UPPER_D       0x40
#define GDT_FLAGS_UPPER_G       0x80

#define GDT_FLAGS_LOWER_KERNELCODE ( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | \
        GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_KERNELDATA ( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
        GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERCODE ( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | \
        GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERDATA ( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
        GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_TSS ( GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P | \
        GDT_TYPE_TSS )

#define GDT_FLAGS_UPPER_CODE ( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_D )
#define GDT_FLAGS_UPPER_DATA ( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_D )
#define GDT_FLAGS_UPPER_TSS ( GDT_FLAGS_UPPER_G )

#define GDT_KERNELCODESEGMENT   0x08
#define GDT_KERNELDATASEGMENT   0x10
//#define GDT_USERDATASEGMENT     0x18
//#define GDT_USERCODESEGMENT     0x20
#define GDT_TSSSEGMENT          0x18

#define SELECTOR_RPL_0          0x00
#define SELECTOR_RPL_3          0x03

#define GDTR_STARTADDRESS   0x102000
#define GDT_MAXENTRYCOUNT   4
#define GDT_TABLESIZE       ( sizeof(GDTENTRY) * GDT_MAXENTRYCOUNT )
#define TSS_SEGMENTSIZE     (sizeof( TSSSEGMENT ) )

//==============================================================================
// IDT
//==============================================================================
#define IDT_TYPE_INTERRUPT      0x0E
#define IDT_TYPE_TRAP           0x0F
#define IDT_FLAGS_DPL0          0x00
#define IDT_FLAGS_DPL1          0x20
#define IDT_FLAGS_DPL2          0x40
#define IDT_FLAGS_DPL3          0x60
#define IDT_FLAGS_P             0x80

#define IDT_FLAGS_KERNEL        ( IDT_FLAGS_DPL0 | IDT_FLAGS_P )
#define IDT_FLAGS_USER          ( IDT_FLAGS_DPL3 | IDT_FLAGS_P )

#define IDT_MAXENTRYCOUNT       100
#define IDTR_STARTADDRESS       ( GDTR_STARTADDRESS + sizeof( GDTR ) + \
        GDT_TABLESIZE + TSS_SEGMENTSIZE )
#define IDT_STARTADDRESS        ( IDTR_STARTADDRESS + sizeof( IDTR ) )
#define IDT_TABLESIZE           ( IDT_MAXENTRYCOUNT * sizeof( IDTENTRY ) )
#define IST_STARTADDRESS        0x700000
#define IST_SIZE                0x100000

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
    DWORD esp0;
    WORD reg_ss0;
    WORD reserved1;
    DWORD esp1;
    WORD reg_ss1;
    WORD reserved2;
    DWORD esp2;
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

#pragma pack(pop)

void kinit_gdt_and_tss();
void kset_tss_segment(TSSSEGMENT* tss_addr);
void kset_gdt_entry(GDTENTRY* entry_addr, DWORD base_addr, DWORD limit,
                    BYTE upper_flags, BYTE lower_flags, BYTE type);
void kset_idt_table();
void kset_idt_entry(IDTENTRY* entry_addr, void* handler, WORD selector,
                    BYTE flags, BYTE type);
void dummy_handler();

#endif
