/* Copyright 2022 Kyle McCreery 
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

#define FN1_BSPC    LT(1, KC_BSPC)
#define FN2_SPC     LT(2, KC_SPC)
#define FN3_B       LT(3, KC_B)
#define CTL_Z       CTL_T(KC_Z)
#define ALT_X       ALT_T(KC_X)
#define ALT_N       ALGR_T(KC_N)
#define CTL_M       RCTL_T(KC_M)
#define SFT_ENT     RSFT_T(KC_ENT)
#define WIN_C		LGUI_T(KC_C)

enum layer_names {
  _BASE,
  _FN1,
  _FN2,
  _FN3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,     KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,    KC_P7,   KC_P8,   KC_P9,   KC_MUTE,       
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,     KC_H,    KC_J,     KC_K,    KC_L,    KC_ESC,  KC_P4,   KC_P5,   KC_P6,   KC_P0,
    CTL_Z,   ALT_X,   WIN_C,   KC_V,    FN1_BSPC, FN2_SPC, FN3_B,    ALT_N,   CTL_M,   SFT_ENT, KC_P1,   KC_P2,   KC_P3,   KC_MINS
  ),                                                                                                     
  [_FN1] = LAYOUT(                                                                                       
    _______, _______, _______, RESET,   _______,  _______,  KC_PSCR, KC_LBRC, KC_RBRC, KC_BSLS, KC_7,    KC_8,    KC_9,    KC_NLCK,       
    _______, _______, _______, _______, _______,  _______,  _______, KC_GRV,  KC_SCLN, KC_QUOT, KC_4,    KC_5,    KC_6,    KC_0, 
    _______, _______, _______, _______, _______,  KC_TAB,   KC_LGUI, KC_COMM, KC_DOT,  KC_SLSH, KC_1,    KC_2,    KC_3,    KC_EQL
  ),
  [_FN2] = LAYOUT(
    _______, _______, _______, _______, _______,  _______,  _______, KC_LCBR, KC_RCBR, KC_PIPE, KC_AMPR, KC_ASTR, KC_LPRN, KC_CLCK,       
    _______, _______, _______, _______, _______,  _______,  _______, KC_TILD, KC_COLN, KC_DQUO, KC_DLR,  KC_PERC, KC_CIRC, KC_RPRN, 
    _______, _______, _______, _______, KC_DEL,   _______,  _______, KC_LABK, KC_RABK, KC_QUES, KC_EXLM, KC_AT,   KC_HASH, KC_PLUS
  ),                                                        
  [_FN3] = LAYOUT(                                          
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, RGB_TOG,       
    _______, _______, _______, _______, _______,  _______,  _______, _______, _______, _______, RGB_HUI, RGB_SAI, RGB_VAI, RGB_MOD,  
    _______, _______, _______, _______, _______,  _______,  _______, _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, RGB_RMOD
  )

};


#if 0
	#ifdef ENCODER_ENABLE       // Encoder Functionality
		uint8_t selected_layer = 0;
		bool encoder_update_user(uint8_t index, bool clockwise) {
			#ifdef OLED_ENABLE
				oled_clear();
				oled_render();
			#endif
			switch (index) {
				case 0:         // This is the only encoder right now, keeping for consistency
					if ( clockwise ) {
						if ( selected_layer  < 3 && keyboard_report->mods & MOD_BIT(KC_LSFT) ) { // If you are holding L shift, encoder changes layers
							selected_layer ++;
							layer_move(selected_layer);
						} else {
							tap_code(KC_VOLU);                                                   // Otherwise it just changes volume
						}
					} else if ( !clockwise ) {
						if ( selected_layer  > 0 && keyboard_report->mods & MOD_BIT(KC_LSFT) ){
							selected_layer --;
							layer_move(selected_layer);
						} else {
							tap_code(KC_VOLD);
						}
					}
			}
		return true;
		}
	#endif

	#ifdef OLED_ENABLE   // OLED Functionality
		oled_rotation_t oled_init_user(oled_rotation_t rotation) {
			return OLED_ROTATION_270;
		}

		bool clear_screen = false;          // used to manage singular screen clears to prevent display glitch
		static void render_name(void) {     // Render Mercutio Script Text
			static const char PROGMEM mercutio_name[] = {
				0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0x95, 0xB5, 0x96, 0xD5, 0xB6, 0xB6,
				0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
				0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
				0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
			};
			oled_write_P(mercutio_name, false);
		}

		static void render_logo(void) {     // Render MechWild "MW" Logo
			static const char PROGMEM logo_1[] = {0x97, 0x98, 0x99, 0x9A,0x00};
			static const char PROGMEM logo_2[] = {0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x00};
			static const char PROGMEM logo_3[] = {0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xB6, 0x00};
			static const char PROGMEM logo_4[] = {0xB6, 0xB6, 0xB6, 0x9B, 0x9C, 0x9D, 0x9E, 0x00};
			oled_set_cursor(0,0);
			oled_write_P(logo_1, false);
			oled_set_cursor(0,1);
			oled_write_P(logo_2, false);
			oled_set_cursor(0,2);
			oled_write_P(logo_3, false);
			oled_set_cursor(0,3);
			oled_write_P(logo_4, false);
		}

		bool oled_task_user(void) {

			if ( IS_HOST_LED_OFF(USB_LED_NUM_LOCK) && IS_HOST_LED_OFF(USB_LED_CAPS_LOCK) && selected_layer == 0 && get_highest_layer(layer_state) == 0 ) {
				render_name();
				clear_screen = true;
			} else {
				if (clear_screen == true) {
					oled_clear();
					oled_render();
					clear_screen = false;
				}
				render_logo();
				oled_set_cursor(8,2);
				switch(selected_layer){
					case 0:
						oled_write_P(PSTR("Lock Layer 0"), false);
						break;
					case 1:
						oled_write_P(PSTR("Lock Layer 1"), false);
						break;
					case 2:
						oled_write_P(PSTR("Lock Layer 2"), false);
						break;
					case 3:
						oled_write_P(PSTR("Lock Layer 3"), false);
						break;
					default:
						oled_write_P(PSTR("Lock Layer ?"), false);    // Should never display, here as a catchall
				}
				oled_set_cursor(8,3);
				if (get_highest_layer(layer_state) == selected_layer) {
					oled_write_P(PSTR("            "), false);
				} else {
					switch (get_highest_layer(layer_state)) {
						case 0:
							oled_write_P(PSTR("Temp Layer 0"), false);
							break;
						case 1:
							oled_write_P(PSTR("Temp Layer 1"), false);
							break;
						case 2:
							oled_write_P(PSTR("Temp Layer 2"), false);
							break;
						case 3:
							oled_write_P(PSTR("Temp Layer 3"), false);
							break;
						default:
							oled_write_P(PSTR("Temp Layer ?"), false);    // Should never display, here as a catchall
					}
				}
				led_t led_state = host_keyboard_led_state();
				oled_set_cursor(8,0);
				oled_write_P(led_state.scroll_lock ? PSTR("SCRLK") : PSTR("     "), false);
				oled_set_cursor(8,1);
				oled_write_P(led_state.num_lock ? PSTR("NLCK ") : PSTR("     "), false);
				oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
			}
		return false;
		}
	#endif
#endif

void keyboard_post_init_user(void) {

#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    //SET_COLOR(g_layer_colors[ kLayer_EXT_KEYS ].hue , g_layer_colors[ kLayer_EXT_KEYS ].sat , g_layer_colors[ kLayer_EXT_KEYS ].val);
    //rgblight_sethsv_noeeprom( MY_ORANGE );
    //rgblight_mode_noeeprom( DEFAULT_LED_MODE );
#endif

    //attempt an EEPROM corruption recovery
    /*
    if ( kLayer_EXT_KEYS != get_highest_layer(layer_state) ){
        eeconfig_init();
        set_single_persistent_default_layer( kLayer_EXT_KEYS );
    }
    */
};