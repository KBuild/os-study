#ifndef __PAGE_H__
#define __PAGE_H__

#include "types.h"

#define PAGE_FLAGS_P        0x00000001
#define PAGE_FLAGS_RW       0x00000002
#define PAGE_FLAGS_US       0x00000004
#define PAGE_FLAGS_PWT      0x00000008
#define PAGE_FLAGS_PCD      0x00000010
#define PAGE_FLAGS_A        0x00000020
#define PAGE_FLAGS_D        0x00000040
#define PAGE_FLAGS_PAT      0x00000080
#define PAGE_FLAGS_G        0x00000100
#define PAGE_FLAGS_DEFAULT  ( PAGE_FLAGS_P | PAGE_FLAGS_RW )
#define PAGE_TABLESIZE      0x0400
#define PAGE_MAXENTRYCOUNT  1024
#define PAGE_DEFAULT_SIZE   0x1000

#pragma pack(push, 1)

typedef struct k_page_table_entry_struct
{
	DWORD base_address;
} PDENTRY, PTENTRY, PENTRY;

#pragma pack(pop)

void kinit_page_tables(void);
void kset_page_entry_data(PENTRY* entry, DWORD address, DWORD flags);

#endif /*__PAGE_H__*/
