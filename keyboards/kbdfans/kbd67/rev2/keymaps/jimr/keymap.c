/* Copyright 2020 MechMerlin 
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

#define __X__  KC_NO


/** @addtogroup jr_kbd67v2_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  kbdfans/kbd67/rev2 -km jimr
  *  Clean :
  *     qmk compile -kb  kbdfans/kbd67/rev2 -km clean
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    06/17/22 |   JR     | Initial File
  * @{
*/

#if 0
//ref for quantum keycodes
	QK_BOOT	Put the keyboard into bootloader mode for flashing
	DB_TOGG	Toggle debug mode
	EE_CLR	Reinitializes the keyboards EEPROM (persistent memory)
	QK_RBT	Resets the keyboard. Does not load the bootloader
//ref for backlighting
	BL_TOGG	Turn the backlight on or off
	BL_STEP	Cycle through backlight levels
	BL_ON	Set the backlight to max brightness
	BL_OFF	Turn the backlight off
	BL_INC	Increase the backlight level
	BL_DEC	Decrease the backlight level
	BL_BRTG	Toggle backlight breathing	
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* Keymap (Base Layer) Default Layer
	   * ,----------------------------------------------------------------------.
	   * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  Backsp   | In  |
	   * |----------------------------------------------------------------------|
	   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [ |  ] |   \   | Del  |
	   * |----------------------------------------------------------------------|
	   * |Caps   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '| Return    | PgUp |
	   * |----------------------------------------------------------------------|
	   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  / | Shift | Up  | Pgdn |
	   * |----------------------------------------------------------------------|
	   * | Ctrl | Win | Alt | space | fn | space| fn |ALT|Ctrl|Lef | Dow | Rig  |
	   * `----------------------------------------------------------------------'
	   */
	[0] = LAYOUT_all(
	  KC_ESC,   KC_1,    KC_2,    KC_3,   KC_4,    KC_5,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   __X__, KC_BSPC, KC_INS,
	  KC_TAB,   KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, 
	  KC_CAPS,  KC_A,    KC_S,    KC_D,   KC_F,    KC_G,   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, 
	  KC_LSFT,  __X__  , KC_Z,    KC_X,    KC_C,   KC_V,    KC_B,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN, 
	  KC_LCTL,  KC_LGUI, KC_LALT, KC_SPC, MO(1),   KC_SPC, MO(2),   MO(3), KC_PSCR, KC_LEFT, KC_DOWN, KC_RGHT  ),

	   	/* Keymap (FN Layer) 
	   * ,----------------------------------------------------------------------.
	   * |~ `| F1| F2| F3| F4| F5| F6| F7| F8| F9| F10| F11| F12|        | PrtS |
	   * |----------------------------------------------------------------------|
	   * |     |   |   |   |   |   |   |   |   |   |   |    |    |       |      |
	   * |----------------------------------------------------------------------|
	   * |       |   |   |   |   |   |   |   |   |   |   |   | Return    | Home |
	   * |----------------------------------------------------------------------|
	   * |        |   |   |   |   |   |   |   |   |   |    | Shift |     |  End |
	   * |----------------------------------------------------------------------|
	   * |      |     |     | space | fn | space | fn | Ctrl |     |     |      |
	   * `----------------------------------------------------------------------'
	   */
	[1] = LAYOUT_all(
	   KC_GRV ,  KC_F1 ,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, _______, KC_PSCR,__X__,
	   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_HOME,
	   _______, __X__  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_END,
	   EE_CLR,  QK_BOOT, _______, _______, _______, _______, _______, _______, _______, BL_DEC, _______, BL_INC ), 


	   	/* Keymap (FN Layer) 
	   * ,----------------------------------------------------------------------.
	   * |~ `|F13|F14|F15|F16|F17|F18|F19|F20|F21| F22| F23| F24|        |      |
	   * |----------------------------------------------------------------------|
	   * |     |   |   |   |   |   |   |   |   |   |   |    |    |       |      |
	   * |----------------------------------------------------------------------|
	   * |       |   |   |   |   |   |   |   |   |   |   |   | Return    |      |
	   * |----------------------------------------------------------------------|
	   * |        |   |   |   |   |   |   |   |   |   |    | Shift |     |      |
	   * |----------------------------------------------------------------------|
	   * |      |     |     | space | fn | space | fn | Ctrl |     |     |      |
	   * `----------------------------------------------------------------------'
	   */

	[2] = LAYOUT_all(
	   _______, KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  _______, _______,__X__,
	   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
	   _______, __X__  , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
	   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ ), 
};
	


void keyboard_post_init_user(void) {

    backlight_enable();

    //attempt an EEPROM corruption recovery
    /*
    if ( kLayer_EXT_KEYS != get_highest_layer(layer_state) ){
        eeconfig_init();
        set_single_persistent_default_layer( kLayer_EXT_KEYS );
    }
    */
};