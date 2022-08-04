/* Copyright 2021 Kyle McCreery
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include <stdio.h>/*snprintf*/


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-const-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
/** @defgroup jr_natr_murphpad_keymap */

/** @addtogroup jr_natr_murphpad_keymap
  * @brief Keymap
  *
  * @details
  *
  *  Build :
  *     qmk compile -kb  mechwild/murphpad -km natr-bigoled-cat
  *  Clean :
  *     qmk compile -kb  mechwild/murphpad -km clean
  *
  *  Programming :
  *     Must press 'reset' button 2x to enable Caterina bootloader
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    06/18/22 |   JR     | Initial File - from jimr-bigoled
  *    07/24/22
  * @{
*/

/** @ingroup jr_natr_murphpad_keymap
*   @brief  enumerate list of layers
*/
typedef enum {
  kMainLayer = 0,
  kRgbLayer,
  kNumLayers
} kLayers;

/** @ingroup jr_natr_murphpad_keymap
*   @brief  create link between a layer and its ascii name
*/
typedef struct {
	kLayers    layer;
	const char *oled;
} sLayers;

/** @ingroup jr_natr_murphpad_keymap
*   @brief  Custom keycodes
*/
typedef enum {
	kCUSTOME_KEYCODES = SAFE_RANGE,
	kTOGGLE_OLED,
	kGOTO_NEXT_LAYER,
	kMAX_CUSTOM_KEYCODES
} kCustomKeycodes;

/** @ingroup jr_natr_murphpad_keymap
*   @brief  associate custom keycodes with ascii content to push out
*/
typedef struct {
	kCustomKeycodes    kc;
	const char 		   *kbstring;
} sKeyCode;

/** @ingroup jimr_murphpad_keymap
*   @brief  enumerate list of layers
*/
sKeyCode g_key_codes[ kMAX_CUSTOM_KEYCODES -  SAFE_RANGE ] = {
		{ kCUSTOME_KEYCODES ,   ""                },
		{ kTOGGLE_OLED ,        ""                },
		{ kGOTO_NEXT_LAYER ,    ""                }
};

/** @ingroup jr_natr_murphpad_keymap
*   @brief  table of layers to ascii names
*/
sLayers g_layer_info[kNumLayers] = {
	{ kMainLayer , "MAIN" },
	{ kRgbLayer  , "RGB"  }
};

#ifdef OLED_ENABLE
	typedef enum {
		kOled_Standard,
		kOled_BongoCat
	} kOled_screens;

	kOled_screens g_oled_screen = kOled_BongoCat;
#endif

/** @ingroup jr_natr_murphpad_keymap
*   @brief  retrieve ascii layer name from layer value
*
*   @details
*   		decouple implicit dependacy on layer order and our local structure
*
*   @return  ascii layer name
*/
const char * get_layer_name( uint16_t layer ){
	uint8_t ldx;
	
	for( ldx = 0 ; ldx < kNumLayers; ++ldx ){
		if(  layer == g_layer_info[ ldx ].layer  )	
			return  g_layer_info[ ldx ].oled;
	}

	return "Unknown!";
}

/** @ingroup jr_natr_murphpad_keymap
*   @brief  retrieve ascii string to send associated with keycode
*
*   @return  ascii content to be sent on keycode
*/
const char * get_kc_string( uint16_t keycode ){
	
	if( keycode < SAFE_RANGE ) 
		return "";
		
	return 	g_key_codes[ keycode - SAFE_RANGE ].kbstring;
	
}

#ifdef ENCODER_ENABLE
	bool encoder_update_user(uint8_t index, bool clockwise) {

		kLayers layer = (kLayers) get_highest_layer(layer_state);

		switch (index) {
			case 0://encoder
				if ( kMainLayer == layer ) {
					if (clockwise) {
						tap_code(KC_VOLU);
					} else {
						tap_code(KC_VOLD);
					}
				} else if ( kRgbLayer == layer ){
					if (clockwise) {
						tap_code16( RGB_HUI );
					} else {
						tap_code16( RGB_HUD );
					}
				}
			break;

			case 1:
				if ( kMainLayer == layer ) {
					if (clockwise) {
						tap_code(KC_BRIU);
					} else {
						tap_code(KC_BRID);
					}
				} else if ( kRgbLayer == layer ){
					if (clockwise) {
						tap_code16(RGB_SAI);
					} else {
						tap_code16(RGB_SAD);
					}
				}
			break;
		}
		return true;
	}
#endif

/** @ingroup jr_natr_murphpad_keymap
*   @brief  Manage simple state machine for advancing to next layer while allowing for rollover
*
*   @param  curr_layer  [in] :  current layer
*
*   @return  --
*/
void move_to_next_layer( kLayers curr_layer ){

   kLayers next_layer = ++curr_layer;

   if( next_layer == kNumLayers ){
       next_layer = (kLayers)0;
   }

   layer_move( next_layer );
};

/** @ingroup jr_natr_murphpad_keymap
*   @brief
*
*   @param  keycode  [in] :  keycode
*   @param  record   [in] :
*
*   @return  true if ??
*/
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	
	switch( keycode ){

		case kTOGGLE_OLED:
			if ( record->event.pressed ) {
#ifdef OLED_ENABLE
				if( kOled_Standard == g_oled_screen ){
					g_oled_screen = kOled_BongoCat;
					oled_on();
				}else if( kOled_BongoCat == g_oled_screen ){
					g_oled_screen = kOled_Standard;
					oled_on();
				}
#endif
			}
			break;
	}
	
	return true;
};

#ifdef OLED_ENABLE

void render_anim(void);
char wpm_str[10];

/** @ingroup jr_natr_murphpad_keymap
*   @brief handle rotation of oled
*
*   @param  rotation  [in] :  current rotation
*
*   @return desired rotation
*/
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;       // flips the display 180 degrees
}


/////////////////////////////////////////////////////////////////////////


#define MAX_RGB_STAT_LINE	( 20 )
//keep off stack
char g_rgbStatusLine1[ MAX_RGB_STAT_LINE ] = {0};
char g_rgbStatusLine2[ MAX_RGB_STAT_LINE ] = {0};

/** @ingroup jr_natr_murphpad_keymap
*   @brief periodic activity for user driven oled task management
*
*   @return true if ??
*/
bool oled_task_user(void) {
	
	if( kOled_Standard == g_oled_screen ){
		  // Host Keyboard Layer Status
		  oled_write_P(PSTR("LAYER:   "), false);
		  oled_write_ln( get_layer_name( get_highest_layer(layer_state) ), false);

		  // Host Keyboard LED Status
		  led_t led_state = host_keyboard_led_state();
		  oled_write_P(PSTR("NUM LCK: "), false);
		  oled_write_ln_P(led_state.num_lock     ? PSTR("On") : PSTR("Off"), false);

		  oled_write_P(PSTR("CAP LCK: "), false);
		  oled_write_ln_P(led_state.caps_lock    ? PSTR("On") : PSTR("Off"), false);

		  oled_write_P(PSTR("SRC LCK: "), false);
		  oled_write_ln_P(led_state.scroll_lock  ? PSTR("On") : PSTR("Off"), false);

		#ifdef BACKLIGHT_ENABLE
		  //locsl , not host
		  oled_write_P(PSTR("BACKLIT: "), false);
		  oled_write_ln_P(is_backlight_enabled() ? PSTR("On") : PSTR("Off"), false);
		#endif
		#ifdef RGBLIGHT_ENABLE
														 //      "BACKLIT: "
		  snprintf( g_rgbStatusLine1, sizeof( g_rgbStatusLine1), "RGB:     %02d", rgblight_get_mode());
		  oled_write_ln( g_rgbStatusLine1, false);

		  snprintf( g_rgbStatusLine2, sizeof( g_rgbStatusLine2), "H:%03d S:%03d V:%03d", rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
		  oled_write_ln( g_rgbStatusLine2, false);

		  //blank
		  snprintf( g_rgbStatusLine2, sizeof( g_rgbStatusLine2), "                 ");
	      oled_write_ln( g_rgbStatusLine2, false);
		#endif
	} else if( kOled_BongoCat == g_oled_screen ){
        render_anim();
        //oled_set_cursor(0,6);

        oled_set_cursor(0,5);
		//blank
		snprintf( g_rgbStatusLine2, sizeof( g_rgbStatusLine2), "                 ");
	    oled_write_ln( g_rgbStatusLine2, false);

        sprintf(wpm_str, "       WPM: %03d", get_current_wpm());
        oled_write(wpm_str, false);
	}

	return true;
}


#endif//OLED_ENABLE

/** @ingroup jr_natr_murphpad_keymap
*   @brief  keymap file
*/
const uint16_t PROGMEM keymaps[kNumLayers][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [kMainLayer] = LAYOUT(
                        KC_F1,   KC_F2,   KC_F3,   KC_F4,
				        KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS,
                        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
		KC_MUTE,        KC_P4,   KC_P5,   KC_P6,   _______,
		kTOGGLE_OLED,   KC_P1,   KC_P2,   KC_P3,   KC_PENT,
		MO(kRgbLayer),  KC_P0,   KC_P0,   KC_PDOT, _______,
						/* Bottom side of PCB*/
                        KC_F5,   KC_F6,   KC_F7

    ),
    [kRgbLayer] = LAYOUT(
    		            RGB_TOG,  _______, EE_CLR, QK_BOOT,
                        _______,  _______, _______, _______,
                        RGB_HUD,  RGB_SPI, RGB_HUI, _______,
        _______,        RGB_RMOD, RGB_TOG, RGB_MOD, _______,
        _______,        RGB_VAD,  RGB_SPD, RGB_VAI, _______,
        _______,        RGB_SAD,  RGB_SAD, RGB_SAI, _______,
                       /* Bottom side of PCB*/
						_______, _______, _______

    )
	#if 0
	  [_FN3] = LAYOUT(
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,

                  _______, _______, _______

    )
	#endif
};

///////////////////////////////////////////////
///		Bongo Cat Animation
///////////////////////////////////////////////
#ifdef OLED_ENABLE
	//custom timeout ( default is 60? )322323232121321321231231
	#ifdef OLED_TIMEOUT
    #undef OLED_TIMEOUT
	#define OLED_TIMEOUT 300000
    #endif

	// WPM-responsive animation stuff here
	#define IDLE_FRAMES 5
	#define IDLE_SPEED /*40*/ 5 // below this wpm value your animation will idle

	// #define PREP_FRAMES 1 // uncomment if >1

	#define TAP_FRAMES 2
	#define TAP_SPEED /*60*/ 20 // above this wpm value typing animation to triggere

	#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
	// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
	#define ANIM_SIZE 636 // number of bytes in array, minimize for adequate firmware size, max is 1024

	uint32_t anim_timer = 0;
	uint32_t anim_sleep = 0;
	uint8_t current_idle_frame = 0;
	// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
	uint8_t current_tap_frame = 0;

	//override - allow any keys including numpad
	bool wpm_keycode_user(uint16_t keycode) {
	    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) || (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
	        keycode = keycode & 0xFF;
	    } else if (keycode > 0xFF) {
	        keycode = 0;
	    }

#if 0
	    if ((keycode >= KC_A && keycode <= KC_0) || (keycode >= KC_TAB && keycode <= KC_SLASH)) {
	        return true;

	    return false;
#else
    	return true;
#endif
	}


	// Images credit j-inc(/James Incandenza) and pixelbenny. Credit to obosob for initial animation approach.
	void render_anim(void) {
		static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
			{
//          1   2  3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32 33 34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50   1   2  3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32 33 34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50 1   2  3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
			{
			0,  0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 16,  8,  8,  4,  4,  4,  8, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24,100,130,  2,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
			{
			0,  0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16,  8,  4,  2,  2,  4, 24, 96,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,194,  1,  1,  2,  2,  4,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96, 96,  0,129,130,130,132,  8, 16, 32, 64,128,  0,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 25,  6,  0,  0,  0,  0,  0,  0,  0, 24, 24, 24, 27,  3,  0, 64,160, 34, 36, 20, 18, 18, 18, 11,  8,  8,  8,  8,  5,  5,  9,  9, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
			{
			0,  0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
			{
			0,  0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 ,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  4,  2,  2,  2,  4, 56, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,226,  1,  1,  2,  2,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,192,193,193,194,  4,  8, 16, 32, 64,128,  0,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 13,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  2,  2,  4,  4,  8,  8,  8,  8,  8,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			}
		};
		static const char PROGMEM prep[][ANIM_SIZE] = {
			{
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132,195,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64,  64, 32, 32, 32, 32,  16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			}
		};
		static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE] = {
			{
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,129,128,128,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,  1,  2,  4,  8, 16, 32, 67,135,  7,  1,  0,184,188,190,159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0, 24,  6,  5,152,153,132, 67,124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  8,  8, 16, 16, 16, 16, 16, 17, 15,  1, 61,124,252,252,252,252,252, 60, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64,  64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,  2,  2,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
			{
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,128, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  4,  2,  1,  1,  2, 12, 48, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30,225,  0,  0,  1,  1,  2,  2,  1,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0, 48, 48,  0,  0,  1,225, 26,  6,  9, 49, 53,  1,138,124,  0,  0,128,128,128,128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,196,  4,196,  4,196,  2,194,  2,194,  1,  1,  1,  1,  0,  0,  0,
			0,  0, 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,112, 12,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0, 64,160, 33, 34, 18, 17, 17, 17,  9,  8,  8,  8,  8,  4,  4,  4,  4,  4,  4,  2,  2,  2,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,170,170,255,255,195,191,127,  3,127,191,195,255,255,170,170,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128, 64, 64, 64,  64, 32, 32, 32, 32, 16, 16, 16, 16,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  3,122,122,121,121,121,121, 57, 49,  2,  2,  4,  4,  8,  8,  8,136,136,135,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,130,135, 31,  7,159,  7, 28,  7,159,  7,159,  7,  2,130,  0,  0,  0,  0,
		   16, 16, 16,  16, 8,  8,  8,  8,  4,  4,  4,  4,  4,  2,  2,  2,  2,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48,120,124,254,255, 63,  7,  0,  0,  0,  0,255,255,127,127, 63, 62, 28, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  8, 23,  0, 15,  1,  2,  1, 15,  0, 15,  2,  5,  8
			},
		};

		//assumes 1 frame prep stage
		void animation_phase(void) {
			if(get_current_wpm() <=IDLE_SPEED){
				current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
				oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
			 }

			 if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED){
				 // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
				 oled_write_raw_P(prep[0], ANIM_SIZE);  // remove if IDLE_FRAMES >1
			 }
			 if(get_current_wpm() >=TAP_SPEED){
				 current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
				 oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE);
			 }
		}
		if(get_current_wpm() != 000) {
			oled_on(); // not essential but turns on animation OLED with any alpha keypress
			if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
				anim_timer = timer_read32();
				animation_phase();
			}
			anim_sleep = timer_read32();
		} else {
			if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
				oled_off();
			} else {
				if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
					anim_timer = timer_read32();
					animation_phase();
				}
			}
		}
	}
#endif //OLED_ENABLE
#pragma GCC diagnostic pop
/** @} */
