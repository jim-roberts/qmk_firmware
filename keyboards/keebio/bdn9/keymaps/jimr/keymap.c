/* Copyright 2019 Danny Nguyen <danny@keeb.io>
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


/** @defgroup jr_bdn9_keymap */

/** @addtogroup jr_bdn9_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  keebio/bdn9/rev2 -km jimr
  *  Clean :
  *     qmk compile -kb  keebio/bdn9/rev2 -km clean
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    xx/xx/xx |   JR     | Initial File 
  * @{
*/

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

//(re)define some useful items
#ifndef _______
#define _______         KC_TRNS             /*transparent to layer*/
#endif
#define ___X___         KC_NO               /* do nothing */
#define CTL_SHFT( kc )  LCTL( LSFT(kc) )
#define ALT_SHFT( kc )  LALT( LSFT(kc) )

#define PSCR_PASTE_KEY      KC_F20  /*Pass to autohotkey script : prntsrc, open irfanview and paste*/

enum custom_keycodes {
    kCYGWIN_ROOT_TO_REPO = SAFE_RANGE,
    kCYGWIN_TREE,
    kWINCMD_ROOT_TO_REPO,
	kHEX_PREFIX
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      /* start: custom keycodes for UNIX layer */
      case kCYGWIN_ROOT_TO_REPO:
          if (record->event.pressed) {
            SEND_STRING("cd /cygdrive/c/Users/robertsj/Code/Repositories/MKS/\r\n");
          } /*else handle key released */
          break;
      case kCYGWIN_TREE:
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
      case kHEX_PREFIX:
    	  if (record->event.pressed) {
			 SEND_STRING("0x");
		   } /*else handle key released */
		   break;
      /* end: custom keycodes for UNIX layer */
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#if 0
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Mute       | Home | Press: Play/Pause  |
        | Hold: Layer 2     | Up   | RGB Mode           |
        | Left              | Down | Right              |
     */
    [0] = LAYOUT(
        KC_MUTE, KC_HOME, KC_MPLY,
        MO(1)  , KC_UP  , RGB_MOD,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    /*
        | RESET          | N/A  | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
     */
    [1] = LAYOUT(
        RESET  , BL_STEP, KC_STOP,
        _______, KC_HOME, RGB_MOD,
        KC_MPRV, KC_END , KC_MNXT
    ),
#endif
#if 0
    [0] = LAYOUT(
        KC_1    , KC_2 , KC_3,
        MO(1)   , KC_4 , KC_5,
        KC_6    , KC_7 , KC_8
    ),
    [1] = LAYOUT(
        RESET   , KC_9 , KC_A,
        _______ , KC_B , KC_C,
        KC_D    , KC_E , KC_F
    ),
#endif

    [0] = LAYOUT(
        RGB_MOD , MO(1)       , KC_MUTE  ,
		KC_CALC , KC_COPY     , KC_PASTE ,
		KC_MPLY , KC_MPRV     , KC_MNXT
    ),
    [1] = LAYOUT(
        RESET         , ___X___              , ___X___,
		PSCR_PASTE_KEY, kHEX_PREFIX          , ___X___,
		kCYGWIN_TREE  , kWINCMD_ROOT_TO_REPO , kCYGWIN_ROOT_TO_REPO
    ),
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
	    if (clockwise) {
		  tap_code16( LCTL( LSFT(KC_EQL)  ) );//zoom / scale 
	    } else {
		  tap_code16( LCTL( KC_MINS ) );
	    }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_PGUP);
        } else {
            tap_code(KC_PGDN);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}

 /** @} */
