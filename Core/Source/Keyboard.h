//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Keyboard.h
//! \brief	Key
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		12-07-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef KEYBOARD_H
#define KEYBOARD_H

//Code From SDL :-)
#ifndef VK_0
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'
#endif /* VK_0 */

/* These keys haven't been defined, but were experimentally determined */
#define VK_SEMICOLON	0xBA
#define VK_EQUALS	0xBB
#define VK_COMMA	0xBC
#define VK_MINUS	0xBD
#define VK_PERIOD	0xBE
#define VK_SLASH	0xBF
#define VK_GRAVE	0xC0
#define VK_LBRACKET	0xDB
#define VK_BACKSLASH	0xDC
#define VK_RBRACKET	0xDD
#define VK_APOSTROPHE	0xDE
#define VK_BACKTICK	0xDF
#define VK_OEM_102	0xE2



typedef enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	NK_UNKNOWN		= 0,
	NK_FIRST		= 0,
	NK_BACKSPACE		= 8,
	NK_TAB		= 9,
	NK_CLEAR		= 12,
	NK_RETURN		= 13,
	NK_PAUSE		= 19,
	NK_ESCAPE		= 27,
	NK_SPACE		= 32,
	NK_EXCLAIM		= 33,
	NK_QUOTEDBL		= 34,
	NK_HASH		= 35,
	NK_DOLLAR		= 36,
	NK_AMPERSAND		= 38,
	NK_QUOTE		= 39,
	NK_LEFTPAREN		= 40,
	NK_RIGHTPAREN		= 41,
	NK_ASTERISK		= 42,
	NK_PLUS		= 43,
	NK_COMMA		= 44,
	NK_MINUS		= 45,
	NK_PERIOD		= 46,
	NK_SLASH		= 47,
	NK_0			= 48,
	NK_1			= 49,
	NK_2			= 50,
	NK_3			= 51,
	NK_4			= 52,
	NK_5			= 53,
	NK_6			= 54,
	NK_7			= 55,
	NK_8			= 56,
	NK_9			= 57,
	NK_COLON		= 58,
	NK_SEMICOLON		= 59,
	NK_LESS		= 60,
	NK_EQUALS		= 61,
	NK_GREATER		= 62,
	NK_QUESTION		= 63,
	NK_AT			= 64,
	/*
	   Skip uppercase letters
	 */
	NK_LEFTBRACKET	= 91,
	NK_BACKSLASH		= 92,
	NK_RIGHTBRACKET	= 93,
	NK_CARET		= 94,
	NK_UNDERSCORE		= 95,
	NK_BACKQUOTE		= 96,
	NK_a			= 97,
	NK_b			= 98,
	NK_c			= 99,
	NK_d			= 100,
	NK_e			= 101,
	NK_f			= 102,
	NK_g			= 103,
	NK_h			= 104,
	NK_i			= 105,
	NK_j			= 106,
	NK_k			= 107,
	NK_l			= 108,
	NK_m			= 109,
	NK_n			= 110,
	NK_o			= 111,
	NK_p			= 112,
	NK_q			= 113,
	NK_r			= 114,
	NK_s			= 115,
	NK_t			= 116,
	NK_u			= 117,
	NK_v			= 118,
	NK_w			= 119,
	NK_x			= 120,
	NK_y			= 121,
	NK_z			= 122,
	NK_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	NK_WORLD_0		= 160,		/* 0xA0 */
	NK_WORLD_1		= 161,
	NK_WORLD_2		= 162,
	NK_WORLD_3		= 163,
	NK_WORLD_4		= 164,
	NK_WORLD_5		= 165,
	NK_WORLD_6		= 166,
	NK_WORLD_7		= 167,
	NK_WORLD_8		= 168,
	NK_WORLD_9		= 169,
	NK_WORLD_10		= 170,
	NK_WORLD_11		= 171,
	NK_WORLD_12		= 172,
	NK_WORLD_13		= 173,
	NK_WORLD_14		= 174,
	NK_WORLD_15		= 175,
	NK_WORLD_16		= 176,
	NK_WORLD_17		= 177,
	NK_WORLD_18		= 178,
	NK_WORLD_19		= 179,
	NK_WORLD_20		= 180,
	NK_WORLD_21		= 181,
	NK_WORLD_22		= 182,
	NK_WORLD_23		= 183,
	NK_WORLD_24		= 184,
	NK_WORLD_25		= 185,
	NK_WORLD_26		= 186,
	NK_WORLD_27		= 187,
	NK_WORLD_28		= 188,
	NK_WORLD_29		= 189,
	NK_WORLD_30		= 190,
	NK_WORLD_31		= 191,
	NK_WORLD_32		= 192,
	NK_WORLD_33		= 193,
	NK_WORLD_34		= 194,
	NK_WORLD_35		= 195,
	NK_WORLD_36		= 196,
	NK_WORLD_37		= 197,
	NK_WORLD_38		= 198,
	NK_WORLD_39		= 199,
	NK_WORLD_40		= 200,
	NK_WORLD_41		= 201,
	NK_WORLD_42		= 202,
	NK_WORLD_43		= 203,
	NK_WORLD_44		= 204,
	NK_WORLD_45		= 205,
	NK_WORLD_46		= 206,
	NK_WORLD_47		= 207,
	NK_WORLD_48		= 208,
	NK_WORLD_49		= 209,
	NK_WORLD_50		= 210,
	NK_WORLD_51		= 211,
	NK_WORLD_52		= 212,
	NK_WORLD_53		= 213,
	NK_WORLD_54		= 214,
	NK_WORLD_55		= 215,
	NK_WORLD_56		= 216,
	NK_WORLD_57		= 217,
	NK_WORLD_58		= 218,
	NK_WORLD_59		= 219,
	NK_WORLD_60		= 220,
	NK_WORLD_61		= 221,
	NK_WORLD_62		= 222,
	NK_WORLD_63		= 223,
	NK_WORLD_64		= 224,
	NK_WORLD_65		= 225,
	NK_WORLD_66		= 226,
	NK_WORLD_67		= 227,
	NK_WORLD_68		= 228,
	NK_WORLD_69		= 229,
	NK_WORLD_70		= 230,
	NK_WORLD_71		= 231,
	NK_WORLD_72		= 232,
	NK_WORLD_73		= 233,
	NK_WORLD_74		= 234,
	NK_WORLD_75		= 235,
	NK_WORLD_76		= 236,
	NK_WORLD_77		= 237,
	NK_WORLD_78		= 238,
	NK_WORLD_79		= 239,
	NK_WORLD_80		= 240,
	NK_WORLD_81		= 241,
	NK_WORLD_82		= 242,
	NK_WORLD_83		= 243,
	NK_WORLD_84		= 244,
	NK_WORLD_85		= 245,
	NK_WORLD_86		= 246,
	NK_WORLD_87		= 247,
	NK_WORLD_88		= 248,
	NK_WORLD_89		= 249,
	NK_WORLD_90		= 250,
	NK_WORLD_91		= 251,
	NK_WORLD_92		= 252,
	NK_WORLD_93		= 253,
	NK_WORLD_94		= 254,
	NK_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	NK_KP0		= 256,
	NK_KP1		= 257,
	NK_KP2		= 258,
	NK_KP3		= 259,
	NK_KP4		= 260,
	NK_KP5		= 261,
	NK_KP6		= 262,
	NK_KP7		= 263,
	NK_KP8		= 264,
	NK_KP9		= 265,
	NK_KP_PERIOD		= 266,
	NK_KP_DIVIDE		= 267,
	NK_KP_MULTIPLY	= 268,
	NK_KP_MINUS		= 269,
	NK_KP_PLUS		= 270,
	NK_KP_ENTER		= 271,
	NK_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	NK_UP			= 273,
	NK_DOWN		= 274,
	NK_RIGHT		= 275,
	NK_LEFT		= 276,
	NK_INSERT		= 277,
	NK_HOME		= 278,
	NK_END		= 279,
	NK_PAGEUP		= 280,
	NK_PAGEDOWN		= 281,

	/* Function keys */
	NK_F1			= 282,
	NK_F2			= 283,
	NK_F3			= 284,
	NK_F4			= 285,
	NK_F5			= 286,
	NK_F6			= 287,
	NK_F7			= 288,
	NK_F8			= 289,
	NK_F9			= 290,
	NK_F10		= 291,
	NK_F11		= 292,
	NK_F12		= 293,
	NK_F13		= 294,
	NK_F14		= 295,
	NK_F15		= 296,

	/* Key state modifier keys */
	NK_NUMLOCK		= 300,
	NK_CAPSLOCK		= 301,
	NK_SCROLLOCK		= 302,
	NK_RSHIFT		= 303,
	NK_LSHIFT		= 304,
	NK_RCTRL		= 305,
	NK_LCTRL		= 306,
	NK_RALT		= 307,
	NK_LALT		= 308,
	NK_RMETA		= 309,
	NK_LMETA		= 310,
	NK_LSUPER		= 311,		/* Left "Windows" key */
	NK_RSUPER		= 312,		/* Right "Windows" key */
	NK_MODE		= 313,		/* "Alt Gr" key */
	NK_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	NK_HELP		= 315,
	NK_PRINT		= 316,
	NK_SYSREQ		= 317,
	NK_BREAK		= 318,
	NK_MENU		= 319,
	NK_POWER		= 320,		/* Power Macintosh power key */
	NK_EURO		= 321,		/* Some european keyboards */
	NK_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	NK_LAST
} NKey;

/* Enumeration of valid key mods (possibly OR'd together) */
/*typedef enum {
	KMOD_NONE  = 0x0000,
	KMOD_LSHIFT= 0x0001,
	KMOD_RSHIFT= 0x0002,
	KMOD_LCTRL = 0x0040,
	KMOD_RCTRL = 0x0080,
	KMOD_LALT  = 0x0100,
	KMOD_RALT  = 0x0200,
	KMOD_LMETA = 0x0400,
	KMOD_RMETA = 0x0800,
	KMOD_NUM   = 0x1000,
	KMOD_CAPS  = 0x2000,
	KMOD_MODE  = 0x4000,
	KMOD_RESERVED = 0x8000
} SDLMod;*/

#endif //KEYBOARD_H
