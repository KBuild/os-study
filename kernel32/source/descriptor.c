#include "types.h"
#include "descriptor.h"

void kinit_gdt_and_tss()
{
    GDTR* gdtr_addr;
    GDTENTRY* entry_addr;
    TSSSEGMENT* tss_addr;
    int i;

    gdtr_addr = (GDTR*) GDTR_STARTADDRESS;
    entry_addr = (GDTENTRY*) (GDTR_STARTADDRESS + sizeof(GDTR));
    gdtr_addr->limit = GDT_TABLESIZE - 1;
    gdtr_addr->base = (DWORD) entry_addr;

    tss_addr = (TSSSEGMENT*) ( (DWORD) entry_addr + GDT_TABLESIZE );

    // NULL Descriptor
    kset_gdt_entry( &(entry_addr[0]), 0, 0, 0, 0, 0);
    // Code Descriptor
    kset_gdt_entry( &(entry_addr[1]), 0, 0xfffff, GDT_FLAGS_UPPER_CODE,
                    GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE); 
    // DATA Descriptor
    kset_gdt_entry( &(entry_addr[2]), 0, 0xfffff, GDT_FLAGS_UPPER_DATA,
                    GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA); 
    // TSS  Segment
    kset_gdt_entry( (GDTENTRY*)&(entry_addr[3]),
                    0, sizeof(TSSSEGMENT),
                    GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);
    kset_tss_segment(tss_addr);
}

void kset_tss_segment(TSSSEGMENT* tss_addr)
{
    kmemset(tss_addr, 0, sizeof(TSSSEGMENT));
    //tss_addr->iomap_base_addr = 0xffff;
}

void kset_gdt_entry(GDTENTRY* entry_addr, DWORD base_addr, DWORD limit,
                    BYTE upper_flags, BYTE lower_flags, BYTE type)
{
    entry_addr->lower_limit =  limit & 0xffff;
    entry_addr->lower_base  =  base_addr & 0xffff;
    entry_addr->upper_base  = (base_addr >> 16) & 0xff;
    entry_addr->type_lower_flag = lower_flags | type;
    entry_addr->upper_limit_upper_flag = ( ( limit >> 16 ) & 0xFF ) | 
                upper_flags;
    entry_addr->upper_base2 = (base_addr >> 24) & 0xff;
}

void kinit_idt_table()
{
    IDTR* idtr_addr;
    IDTENTRY* entry_addr;
    int i;

    idtr_addr  = (IDTR*) IDTR_STARTADDRESS;
    entry_addr = (IDTENTRY*) (IDTR_STARTADDRESS + sizeof(IDTR));
    idtr_addr->base = (DWORD) entry_addr;
    idtr_addr->limit = IDT_TABLESIZE - 1;

    for(i = 0 ; i < IDT_MAXENTRYCOUNT ; i++)
    {
        kset_idt_entry( &(entry_addr[i]), dummy_handler, 0x08,
                        IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
    }
}

void kset_idt_entry(IDTENTRY* entry_addr, void* handler, WORD selector, 
                    BYTE flags, BYTE type)
{
    entry_addr->offset1 = (DWORD) handler & 0xffff;
    entry_addr->selector = selector;
    entry_addr->zero = 0;
    entry_addr->type_attr = type | flags;
    entry_addr->offset2 = ((DWORD) handler >> 16) & 0xffff;
}

void dummy_handler()
{
    kprint_string(0, 0, "======================");
    kprint_string(0, 1, "Dummy Handler Occurred");
    kprint_string(0, 2, "======================");

    while(1);
}
