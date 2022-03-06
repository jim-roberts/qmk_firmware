/*
Copyright 2018 QMK Contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x1209
#define PRODUCT_ID      0xB195
#define DEVICE_VER      0x0001
#define MANUFACTURER    flehrad
#define PRODUCT         BigSwitch PCB

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 1

/* key matrix pins */
#define MATRIX_ROW_PINS { B5 }
#define MATRIX_COL_PINS { B6 }
#define UNUSED_PINS { }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Set 0 if debouncing isn't needed */
//#define DEBOUNCE 50

/* key combination for command */
#define IS_COMMAND() ( \
    false \
)

// Referenced custom font
#ifdef OLED_ENABLE
    //JR
	#define OLED_DISPLAY_128X64

	#ifdef OLED_DISPLAY_128X64
		#if 0
			//0.96in
			#define OLED_IC		OLED_IC_SSD1306 /*default*/
        #else
		   //1.30in
		   #define OLED_IC					OLED_IC_SH1106
		   #define OLED_COLUMN_OFFSET		0 /* # pixels to adjust to the right */
        #endif
	#endif

	#define OLED_TIMEOUT           0 /* ms */ /* We are going to implement our own soln */
	//#define OLED_SCROLL_TIMEOUT    30000/* ms */
#endif

#ifdef AUDIO_ENABLE
	#define AUDIO_PIN 	C6 /* B5 and B6 not avail. used by switch */

    //Adjust Max volume
	//#define AUDIO_DAC_SAMPLE_MAX 4095U

    #define MIDI_BASIC

    /* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
	//#define MIDI_TONE_KEYCODE_OCTAVES 2

	//Enable 300ms delay ( see quantum.c , matrix_scan_quantum() )
	#define AUDIO_INIT_DELAY

#endif

#ifdef RGBLIGHT_ENABLE

	#define RGB_DI_PIN 					E6 /*D3*/

	//#define RGBLIGHT_ANIMATIONS  /* See : https://github.com/qmk/qmk_firmware/blob/master/docs/feature_rgblight.md */
	//RGBLIGHT_ANIMATIONS is bringing in too much code. Explicitly list the ones we want so we get back some codespace
	#define RGBLIGHT_EFFECT_SNAKE
	#define RGBLIGHT_EFFECT_BREATHING
	#define RGBLIGHT_EFFECT_RAINBOW_SWIRL

    #undef RGBLED_NUM
	#define RGBLED_NUM 19 /* magic-8ball */
	#ifdef RGBLIGHT_SLEEP
		#undef RGBLIGHT_SLEEP
#endif

	#ifdef AUDIO_ENABLE
		//#define NO_MUSIC_MODE
	#endif
#endif

//// More codespace scavenging //////
#define DISABLE_LEADER
