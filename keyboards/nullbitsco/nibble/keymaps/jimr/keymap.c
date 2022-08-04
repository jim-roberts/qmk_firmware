/* Copyright 2021 Jose Luis Adelantado Torres
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

/** @defgroup jr_nibble_keymap */

/** @addtogroup jr_nibble_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  nullbitsco/nibble -km jimr
  *  Clean :
  *     qmk compile -kb  nullbitsco/nibble -km clean
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    xx/xx/xx |   JR     | Initial File - based off oled_status keymap. 
  *             |          | 128x64 0.96" oled + encoder ( non-standard nibble options )
  *             |          | Ported some layer + RGB feature(s) from noodle pad code. Work specific macros.
  * @{
*/

//JR
#define ___X___             KC_NO               /* do nothing */
#define MIC_MUTE_KEY        KC_F24
#define MIC_UNMUTE_KEY      KC_F23
#define PSCR_PASTE_KEY      KC_F20  /*Pass to autohotkey script : prntsrc, open irfanview and paste*/

#ifdef OLED_ENABLE
//#    include "../jr-oled-status/oled_display.h"
#include "oled_display.h"
#endif

enum layer_names { _MA, _FN };

/*   @brief Our custom keycode enumeration.*/
enum custom_keycodes {
    KC_CUST = SAFE_RANGE,
    kNIX_ROOT_TO_REPO,
    kNIX_TREE,
    kWINCMD_ROOT_TO_REPO
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#if 0
	[_MA] = LAYOUT_ansi(
			KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
	KC_F1 , KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,
	KC_F2 , KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
	KC_F3 , KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
	KC_F4 , KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    MO(_FN), KC_RALT, KC_RCTL, KC_LEFT,          KC_DOWN, KC_RGHT
	),

#else //JR - assuming:
	// NOTE: Default keymap layers were designed for ANSI split-space layout  http://www.keyboard-layout-editor.com/#/gists/f28bd5ff4e62f69e89896df3a59671c6
	// where ( it seems ) SPACE is implicitly doubled on PCB: i.e  actual layout ==     LALT  SPC   MO(FN)  SP  RALT .....
	//                             CTRL  GUI   LALT  SPC   FN    SPC   RALT RCTRL  L  D  R
	//NOTE R0 spacing is then ...  1.25  1.25  1.25  2.75  1.25  2.25  1.5  1.5    1  1  1
	[_MA] = LAYOUT_ansi(
			         KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
  KC_MUTE          , KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
 PSCR_PASTE_KEY    , KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
 kNIX_ROOT_TO_REPO , KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
   KC_F11          , KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    MO(_FN), KC_RALT, KC_RCTL, KC_LEFT,          KC_DOWN, KC_RGHT
	),
#endif
    [_FN] = LAYOUT_ansi(
                 RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,  KC_END,
        RGB_TOG, _______, RGB_SAI, RGB_SAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MIC_MUTE_KEY,
		RGB_TOG, _______, RGB_HUI, RGB_HUD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, MIC_UNMUTE_KEY,
		RGB_HUI, _______, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
		RGB_MOD, _______, _______, _______,                   _______,                   _______, _______, _______, _______,          _______, _______
    ),
};

// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_timer = timer_read32();
    set_oled_mode(OLED_MODE_IDLE);
    return OLED_ROTATION_0;
    //return OLED_ROTATION_90;
    //return OLED_ROTATION_180;
    //return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (timer_elapsed(oled_timer) >= 3000) {
        set_oled_mode(OLED_MODE_IDLE);
    }
    render_frame();

    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// Send keystrokes to host keyboard, if connected (see readme)
	process_record_remote_kb(keycode, record);
	switch (keycode) {
		case RGB_TOG:
			if (record->event.pressed) {
	#ifdef OLED_ENABLE
				process_record_keymap_oled(keycode);
	#endif
		    }
		  break;
		/* start: custom keycodes for UNIX layer */
		case kNIX_ROOT_TO_REPO:
		  if (record->event.pressed) {
			SEND_STRING("cd /cygdrive/c/Users/robertsj/Code/Repositories/MKS/\r\n");
		  } /*else handle key released */
		  break;
		case kNIX_TREE:
		  if (record->event.pressed) {
			SEND_STRING("tree -d\r\n");
		  } /*else handle key released */
		  break;
		/* end: custom keycodes for UNIX layer */

		/* start: custom keycodes for WIN layer */
		case kWINCMD_ROOT_TO_REPO:
		  if (record->event.pressed) {
			SEND_STRING("cd /d c:\\Users\\robertsj\\Code\\Repositories\\MKS\r\n");
		  } /*else handle key released */
		  break;
		/* end: custom keycodes for UNIX layer */
		}
	return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
#ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLU);
#endif
    } else {
        tap_code(KC_VOLD);
#ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLD);
#endif
    }
    return true;
}

void matrix_init_user(void) {
    // Initialize remote keyboard, if connected (see readme)
    matrix_init_remote_kb();
}

void matrix_scan_user(void) {
    // Scan and parse keystrokes from remote keyboard, if connected (see readme)
    matrix_scan_remote_kb();
}
 /** @} */

