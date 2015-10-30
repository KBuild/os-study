#include "types.h"
#include "page.h"
#include "switch.h"
#include "keyboard.h"

void kprint_string(int iX, int iY, const char* string_addr);
BOOL kinit_kernel_area(void);
BOOL kcheck_memory_enough(void);

void main(void)
{
	//DWORD i;

	char vctemp[2] = {0, };
	BYTE temp;
	BYTE flags;
	int i = 0;

	kprint_string(0, 3, "C Lang Kernel Start.................[Pass]");

	kprint_string(0, 4, "Minimum Memory Size Check...........[    ]");
	if(kcheck_memory_enough() == FALSE)
	{
		kprint_string(37, 4, "Fail");
		kprint_string(0, 5, "Not Enough Memory! OS DEAD");
		while(1);
	}
	else
	{
		kprint_string(37, 4, "Pass");
	}

	kprint_string(0, 5, "32bit Kernel Area Initialize........[    ]");
	if(kinit_kernel_area() == FALSE)
	{
		kprint_string(37, 5, "Fail");
		kprint_string(0, 6, "Kernal Area Initialize Fail");
		while(1);
	}
	kprint_string(37, 5, "Pass");

	kprint_string(0, 6, "32bit Page Tables Initialize........[    ]");
	kinit_page_tables();
	kswitch_and_execute_kernel();
	kprint_string(37, 6, "Pass");

	/*
	 * KEYBOARD ACTIVATE ROUTINE
	 */
	kprint_string(0, 7, "Keyboard Activate...................[    ]");
	if(kactivate_keyboard() == TRUE)
	{
		kprint_string(37, 7, "Pass");
		kchange_keyboard_led(FALSE, FALSE, FALSE);
	}
	else
	{
		kprint_string(37, 7, "Fail");
		while(1);
	}

	while(1)
	{
		if(kcheck_output_buffer() == TRUE)
		{
			temp = kget_keyboard_code();

			if(kconvert_scancode_to_asciicode
				(temp, &(vctemp[0]), &flags) == TRUE)
			{
				if(flags & KEY_FLAGS_DOWN)
				{
					kprint_string(i++, 13, vctemp);
				}
			}
		}
	}
	/*
	 * ROUTINE END
	 */
	while(1);
}

void kprint_string(int iX, int iY, const char* string_addr)
{
	KCHARACTER* screen_addr = (KCHARACTER*) 0xB8000;
	int i;

	screen_addr += (iY * 80) + iX;
	for(i = 0 ; string_addr[i] != 0 ; i++)
	{
		screen_addr[i].character = string_addr[i];
	}
}

BOOL kinit_kernel_area(void)
{
	DWORD* current_addr;

	current_addr =  (DWORD*) 0x100000;

	while( (DWORD)current_addr < 0x600000 )
	{
		*current_addr = 0x00;
		if(*current_addr != 0)
		{
			return FALSE;
		}

		current_addr++;
	}

	return TRUE;
}

BOOL kcheck_memory_enough(void)
{
	DWORD* current_addr;

	current_addr = (DWORD*) 0x100000;

	while( (DWORD) current_addr < 0x4000000 )
	{
		*current_addr = 0x12345678;

		if(*current_addr != 0x12345678)
		{
			return FALSE;
		}

		current_addr += (0x100000 / 4);
	}

	return TRUE;
}
