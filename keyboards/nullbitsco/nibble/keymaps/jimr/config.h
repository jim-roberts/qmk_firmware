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
#pragma once

#include "config_common.h"

#ifdef OLED_ENABLE
	#define OLED_DISPLAY_128X64 /* This informs QMK OLED driver size of oled screen*/

	#ifdef OLED_DISPLAY_128X64
		//0.96in
		//#define OLED_IC		OLED_IC_SSD1306 /*default*/
		//1.30in
		#define OLED_IC		OLED_IC_SH1106
		
		#if ( OLED_IC_SH1106 == OLED_IC )
			#define OLED_COLUMN_OFFSET ( 2 ) 
		#endif
		
		#define OLED_FONT_H "keyboards/nullbitsco/nibble/keymaps/jimr/glcdfont.c"
	#endif
#endif

#ifdef RGBLIGHT_SLEEP
#undef RGBLIGHT_SLEEP
#endif
#define RGBLIGHT_ANIMATIONS
//#define RGBLIGHT_EFFECT_SNAKE
//#define RGBLIGHT_EFFECT_BREATHING
