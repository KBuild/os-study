#include "types.h"
#include "asmutility.h"
#include "keyboard.h"

//output buffer(0x60 chck)
//if full return true
BOOL kcheck_output_buffer(void)
{
	if(kinport_byte(0x64) & 0x01)
	{
		return TRUE;
	}
	return FALSE;
}

//input buffer(0x60 check)
BOOL kcheck_input_buffer(void)
{
	if(kinport_byte(0x64) & 0x02)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL kactivate_keyboard(void)
{
	int i;
	int j;

	koutport_byte(0x64, 0xAE);

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	koutport_byte(0x60, 0xF4);

	for(j = 0 ; j < 100 ; j++)
	{
		for(i = 0 ; i < 0xFFFF ; i++)
		{
			if(kcheck_output_buffer() == TRUE)
			{
				break;
			}
		}

		if(kinport_byte(0x60) == 0xFA)
		{
			return TRUE;
		
		}
	}

	return FALSE;
}

BYTE kget_keyboard_code(void)
{
	while(kcheck_output_buffer() == FALSE)
	{
		;
	}
	return kinport_byte(0x60);
}

BOOL kchange_keyboard_led(BOOL capslockon, BOOL numlockon, BOOL scrolllockon)
{
	int i, j;

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	koutport_byte(0x60, 0xED);

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	for(j = 0 ; j < 100 ; j++)
	{
		for(i = 0 ; i < 0xFFFF ; i++)
		{
			if(kcheck_output_buffer() == TRUE)
			{
				break;
			}
		}

		if(kinport_byte(0x60) == 0xFA)
		{
			break;
		}
	}

	if(j >= 100)
	{
		return FALSE;
	}

	koutport_byte(0x60,
		       	(capslockon << 2) | (numlockon << 1) | scrolllockon);

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	for(j = 0 ; j < 100 ; j++)
	{
		for(i = 0 ; i < 0xFFFF ; i++)
		{
			if(kcheck_output_buffer() == TRUE)
			{
				break;
			}
		}


		if(kinport_byte(0x60) == 0xFA)
		{
			break;
		}
	}

	if(j >= 100)
	{
		return FALSE;
	}

	return TRUE;
}

void kenable_A20(void)
{
	BYTE outportdata;
	int i;

	koutport_byte(0x64, 0xD0);

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_output_buffer() == TRUE)
		{
			break;
		}
	}

	outportdata = kinport_byte(0x60);
	outportdata |= 0x01; // set A20 bit

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	koutport_byte(0x64, 0xD1);
	koutport_byte(0x60, outportdata);
}

void kreboot(void) // reset process
{
	int i;

	for(i = 0 ; i < 0xFFFF ; i++)
	{
		if(kcheck_input_buffer() == FALSE)
		{
			break;
		}
	}

	koutport_byte(0x64, 0xD1);
	koutport_byte(0x60, 0x00);

	while(1)
	{
		;
	}
}

static KEYBOARDMANAGER keyboard_manager = { 0, };

static KEYMAPPINGENTRY keymapping_table[KEY_MAPPINGTABLEMAXCOUNT] =
{
/*  0   */ { KEY_NONE       , KEY_NONE       },
/*  1   */ { KEY_ESC        , KEY_ESC        },
/*  2   */ { '1'            , '!'            },
/*  3   */ { '2'            , '@'            },
/*  4   */ { '3'            , '#'            },
/*  5   */ { '4'            , '$'            },
/*  6   */ { '5'            , '%'            },
/*  7   */ { '6'            , '^'            },
/*  8   */ { '7'            , '&'            },
/*  9   */ { '8'            , '*'            },
/*  10  */ { '9'            , '('            },
/*  11  */ { '0'            , ')'            },
/*  12  */ { '-'            , '_'            },
/*  13  */ { '='            , '+'            },
/*  14  */ { KEY_BACKSPACE  , KEY_BACKSPACE  },
/*  15  */ { KEY_TAB        , KEY_TAB        },
/*  16  */ { 'q'            , 'Q'            },
/*  17  */ { 'w'            , 'W'            },
/*  18  */ { 'e'            , 'E'            },
/*  19  */ { 'r'            , 'R'            },
/*  20  */ { 't'            , 'T'            },
/*  21  */ { 'y'            , 'Y'            },
/*  22  */ { 'u'            , 'U'            },
/*  23  */ { 'i'            , 'I'            },
/*  24  */ { 'o'            , 'O'            },
/*  25  */ { 'p'            , 'P'            },
/*  26  */ { '['            , '{'            },
/*  27  */ { ']'            , '}'            },
/*  28  */ { '\n'           , '\n'           },
/*  29  */ { KEY_CTRL       , KEY_CTRL       },
/*  30  */ { 'a'            , 'A'            },
/*  31  */ { 's'            , 'S'            },
/*  32  */ { 'd'            , 'D'            },
/*  33  */ { 'f'            , 'F'            },
/*  34  */ { 'g'            , 'G'            },
/*  35  */ { 'h'            , 'H'            },
/*  36  */ { 'j'            , 'J'            },
/*  37  */ { 'k'            , 'K'            },
/*  38  */ { 'l'            , 'L'            },
/*  39  */ { ';'            , ':'            },
/*  40  */ { '\''           , '\"'           },
/*  41  */ { '`'            , '~'            },
/*  42  */ { KEY_LSHIFT     , KEY_LSHIFT     },
/*  43  */ { '\\'           , '|'            },
/*  44  */ { 'z'            , 'Z'            },
/*  45  */ { 'x'            , 'X'            },
/*  46  */ { 'c'            , 'C'            },
/*  47  */ { 'v'            , 'V'            },
/*  48  */ { 'b'            , 'B'            },
/*  49  */ { 'n'            , 'N'            },
/*  50  */ { 'm'            , 'M'            },
/*  51  */ { ','            , '<'            },
/*  52  */ { '.'            , '>'            },
/*  53  */ { '/'            , '?'            },
/*  54  */ { KEY_RSHIFT     , KEY_RSHIFT     },
/*  55  */ { '*'            , '*'            },
/*  56  */ { KEY_LALT       , KEY_LALT       },
/*  57  */ { ' '            , ' '            },
/*  58  */ { KEY_CAPSLOCK   , KEY_CAPSLOCK   },
/*  59  */ { KEY_F1         , KEY_F1         },
/*  60  */ { KEY_F2         , KEY_F2         },
/*  61  */ { KEY_F3         , KEY_F3         },
/*  62  */ { KEY_F4         , KEY_F4         },
/*  63  */ { KEY_F5         , KEY_F5         },
/*  64  */ { KEY_F6         , KEY_F6         },
/*  65  */ { KEY_F7         , KEY_F7         },
/*  66  */ { KEY_F8         , KEY_F8         },
/*  67  */ { KEY_F9         , KEY_F9         },
/*  68  */ { KEY_F10        , KEY_F10        },
/*  69  */ { KEY_NUMLOCK    , KEY_NUMLOCK    },
/*  70  */ { KEY_SCROLLLOCK , KEY_SCROLLLOCK },

/*  71  */ { KEY_HOME       , '7'            },
/*  72  */ { KEY_UP         , '8'            },
/*  73  */ { KEY_PAGEUP     , '9'            },
/*  74  */ { '-'            , '-'            },
/*  75  */ { KEY_LEFT       , '4'            },
/*  76  */ { KEY_CENTER     , '5'            },
/*  77  */ { KEY_RIGHT      , '6'            },
/*  78  */ { '+'            , '+'            },
/*  79  */ { KEY_END        , '1'            },
/*  80  */ { KEY_DOWN       , '2'            },
/*  81  */ { KEY_PAGEDOWN   , '3'            },
/*  82  */ { KEY_INS        , '0'            },
/*  83  */ { KEY_DEL        , '.'            },
/*  84  */ { KEY_NONE       , KEY_NONE       },
/*  85  */ { KEY_NONE       , KEY_NONE       },
/*  86  */ { KEY_NONE       , KEY_NONE       },
/*  87  */ { KEY_F11        , KEY_F11        },
/*  88  */ { KEY_F12        , KEY_F12        }
};

BOOL kcheck_alphabet_scancode(BYTE scancode)
{
	if( ('a' <= keymapping_table[scancode].normal_code) &&
		(keymapping_table[scancode].normal_code <= 'z') )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL kcheck_numberorsymbol_scancode(BYTE scancode)
{
	if( (2 <= scancode) && (scancode <= 53) &&
		(kcheck_alphabet_scancode(scancode) == FALSE) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL kcheck_numberpad_scancode(BYTE scancode)
{
	if( (71 <= scancode) && (scancode <= 83) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL kcheck_usecombined_code(BYTE scancode)
{
	BYTE down_scancode;
	BOOL usecombined_key;

	down_scancode = scancode & 0x7F;

	if(kcheck_alphabet_scancode(down_scancode) == TRUE)
	{
		if(keyboard_manager.shift_down ^ keyboard_manager.capslockon)
		{
			usecombined_key = TRUE;
		}
		else
		{
			usecombined_key = FALSE;
		}
	}
	else if(kcheck_numberorsymbol_scancode(down_scancode) == TRUE)
	{
		if(keyboard_manager.shift_down == TRUE)
		{
			usecombined_key = TRUE;
		}
		else
		{
			usecombined_key = FALSE;
		}
	}
	else if( (kcheck_numberpad_scancode(down_scancode) == TRUE) &&
			(keyboard_manager.extended_codein == FALSE) )
	{
		if(keyboard_manager.numlockon == TRUE)
		{
			usecombined_key = TRUE;
		}
		else
		{
			usecombined_key = FALSE;
		}
	}

	return usecombined_key;
}

void update_combination_keystatusandled(BYTE scancode)
{
	BOOL down;
	BYTE down_scancode;
	BOOL led_status_changed = FALSE;

	if(scancode & 0x80) // check 7th bit 10000000
	{
		down = FALSE;
		down_scancode = scancode & 0x7F; // 01111111
	}
	else
	{
		down = TRUE;
		down_scancode = scancode;
	}

	//if scancode is shift(42 or 54)
	if( (down_scancode == 42) || (down_scancode == 54) )
	{
		keyboard_manager.shift_down = down;
	}
	// check capslock
	else if( (down_scancode == 58) && (down == TRUE) )
	{
		keyboard_manager.capslockon ^= TRUE;
		led_status_changed = TRUE;
	}
	// check numlock
	else if( (down_scancode == 69) && (down == TRUE) )
	{
		keyboard_manager.numlockon ^= TRUE;
		led_status_changed = TRUE;
	}
	// check scroll lock
	else if( (down_scancode == 70) && (down == TRUE) )
	{
		keyboard_manager.scrolllockon ^= TRUE;
		led_status_changed = TRUE;
	}

	if(led_status_changed == TRUE)
	{
		kchange_keyboard_led(keyboard_manager.capslockon, 
		keyboard_manager.numlockon, keyboard_manager.scrolllockon);
	}
}

BOOL kconvert_scancode_to_asciicode(BYTE scancode, BYTE* asciicode, BOOL* flags)
{
	BOOL usecombined_key;

	//check pause key
	if(keyboard_manager.skipcountforpause > 0)
	{
		keyboard_manager.skipcountforpause--;
		return FALSE;
	}

	//for pause key
	if(scancode == 0xE1)
	{
		*asciicode = KEY_PAUSE;
		*flags = KEY_FLAGS_DOWN;
		keyboard_manager.skipcountforpause = KEY_SKIPCOUNTFORPAUSE;
		return TRUE;
	}
	//for extended key code, set only flags
	else if(scancode == 0xE0)
	{
		keyboard_manager.extended_codein = TRUE;
		return FALSE;
	}

	usecombined_key = kcheck_usecombined_code(scancode);

	if(usecombined_key == TRUE)
	{
		*asciicode = keymapping_table[scancode & 0x7F].combined_code;
	}
	else
	{
		*asciicode = keymapping_table[scancode & 0x7F].normal_code;
	}

	//set using extended code
	if(keyboard_manager.extended_codein == TRUE)
	{
		*flags = KEY_FLAGS_EXTENDEDKEY;
		keyboard_manager.extended_codein = FALSE;
	}
	else
	{
		*flags = 0;
	}

	//set key down or key press
	if( (scancode & 0x80) == 0 )
	{
		*flags |= KEY_FLAGS_DOWN;
	}

	update_combination_keystatusandled(scancode);
	return TRUE;
}
