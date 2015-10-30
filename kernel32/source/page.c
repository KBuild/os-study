#include "page.h"

void kinit_page_tables(void)
{
	PDENTRY* pdentry_addr;
	PTENTRY* ptentry_addr;
	PENTRY * pentry_addr;
	DWORD mapping_addr;
	int i;

	pdentry_addr = (PDENTRY*) 0x100000;
	kset_page_entry_data( &(pdentry_addr[0]), 0x101000, PAGE_FLAGS_DEFAULT);
	for(i = 1 ; i < PAGE_MAXENTRYCOUNT ; i++)
	{
		kset_page_entry_data( &(pdentry_addr[i]), 0, 0); 
	}

	ptentry_addr = (PTENTRY*) 0x101000;
	mapping_addr = 0;
	for(i = 0 ; i < PAGE_MAXENTRYCOUNT ; i++)
	{
		kset_page_entry_data( &(ptentry_addr[i]), mapping_addr,
				PAGE_FLAGS_DEFAULT);
		mapping_addr += PAGE_DEFAULT_SIZE;
	}
}

void kset_page_entry_data(PENTRY* entry, DWORD address, DWORD flags)
{
	entry->base_address = address | flags;
}
