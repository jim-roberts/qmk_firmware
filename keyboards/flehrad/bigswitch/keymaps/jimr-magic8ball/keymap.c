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

//#define BUILD_FOR_RELEASE

#ifndef BUILD_FOR_RELEASE
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-const-variable"
	#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include QMK_KEYBOARD_H

#ifndef RGBLIGHT_ENABLE
#error "Magic 8ball Project requires RGB light!"
#endif

#ifndef OLED_ENABLE
#error "Magic 8ball Project requires OLED"
#endif

#ifdef AUDIO_ENABLE
  #include "audio.h"

//TODO - replace with custom songs for events
	float my_pressed_song[][2] = SONG(NOCTURNE_OP_9_NO_1);
	float my_result_song[][2] = SONG(CAMPANELLA);
#endif

/** @defgroup jr_bigswitch_magic8ball_keymap */

/** @addtogroup jr_bigswitch_magic8ball_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  flehrad/bigswitch -km jimr-magic8ball
  *  Clean :
  *     qmk compile -kb  flehrad/bigswitch -km clean
  *
  *  Flashing:
  *
  *  		1. Press and release reset button on bottom ( small hole, largwe hole is speaker )
  *  		2. QMK toolbox: Select Atmega 32u4. Select file and Flash.
  *
  *
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    02/25/22 |   JR     | Initial File
  * @{
*/

/** @ingroup jr_bigswitch_magic8ball_keymap
*   @brief Our custom keycode enumeration.*/
enum custom_keycodes {
    kCUSTOME_KEYCODES = SAFE_RANGE,
    kMAGIC
};

typedef enum {
	kIDLE_STATE = 0,
	kPRESSED_STATE,
	kRESULT_STATE,
	kMAX_NUM_STATES
} kBall_state;

/** @ingroup jr_bigswitch_magic8ball_keymap
*   @brief define storage type for led color info */
typedef struct {
    uint8_t hue;    /**< HUE           */
    uint8_t sat;    /**< Saturation    */
    uint8_t val;    /**< Value         */
} sHSV;

/** @ingroup jr_bigswitch_magic8ball_keymap
*   @brief define storage type to hold layer led color and modes
*
*   @details We include an explicit layer member to avoid implicit association of enumerated order to any table ordering
*
*/
typedef struct {
	kBall_state  state;			/**< ball state */
	sHSV		 color; 		/**< color for ball state */
	uint16_t	 rgb_mode; 		/**< RGB mode for ball state, see RGBLIGHT_ANIMATIONS  */
} sTable_entry;

static kBall_state  g_ball_state = kIDLE_STATE;
static uint32_t g_state_timeout = 0;
static uint32_t g_oled_off_timer;


#define MY_BLUE     HSV_BLUE
#define MY_GREEN    HSV_GREEN
// 17.99  , 100% , 100%
#define MY_ORANGE   0x07 , 0xFF,  0xFF /*HSV_ORANGE*/
//185 , 100% , 100%
//#define MY_PURPLE   185 , PERC_2_BYTE( 100 ) , PERC_2_BYTE( 100 )
#define MY_PURPLE   0xB9 , 0xFF , 0xFF /*HSV_PURPLE*/

#ifdef RGBLIGHT_ENABLE
	#define SET_COLOR( h,s,v )      rgblight_sethsv_noeeprom( h,s,v );
	#define SET_LED_MODE( mod )     rgblight_mode_noeeprom( (uint8_t)mod ) ;
#else
	#define SET_COLOR( h,s,v )
	#define SET_LED_MODE( mod )
#endif
/*
 choices for rgb animations , See: rgblight.h
 RGBLIGHT_MODE_STATIC_LIGHT     : All leds on w/current rgb color(s)
 RGBLIGHT_MODE_BREATHING        : Slowly brighten and dim all leds at same time w/current rgb color(s)
 RGBLIGHT_MODE_RAINBOW_MOOD     : All leds on and all slowly cycle through colors
 RGBLIGHT_MODE_RAINBOW_SWIRL    : Same effect as RGBLIGHT_MODE_STATIC_GRADIENT ( might be due to limted # of leds )
 RGBLIGHT_MODE_SNAKE            : One led on at a time, moving in a circular pattern
 RGBLIGHT_MODE_KNIGHT           : Knight rider /cylon light effect w/current rgb color(s)
 RGBLIGHT_MODE_STATIC_GRADIENT  : Each led on but assigned a different color from the spectrum
 RGBLIGHT_MODE_TWINKLE          : Slow brighten and dim one led at a time, in sequence w/current rgb color(s)
 RGBLIGHT_MODE_ALTERNATING      : Alternate halves ( on/off ) w/current rgb color(s)
 RGBLIGHT_MODE_CHRISTMAS        : Alternate halves with Red and Green colors
 RGBLIGHT_MODE_RGB_TEST
 */

#ifdef RGBLIGHT_ENABLE
	#define IDLE_STATE_RGB_MODE		RGBLIGHT_MODE_BREATHING
	#define	PRESSED_STATE_RGB_MODE	RGBLIGHT_MODE_SNAKE
	#define	RESULT_STATE_RGB_MODE	RGBLIGHT_MODE_RAINBOW_SWIRL

	// How long (in milliseconds) to wait between animation steps for each of the "Solid color breathing" animations
	//const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = {30, 20, 10, 5};

	// How long (in milliseconds) to wait between animation steps for each of the "Cycling rainbow" animations
	//const uint8_t RGBLED_RAINBOW_MOOD_INTERVALS[] PROGMEM = {120, 60, 30};

	// How long (in milliseconds) to wait between animation steps for each of the "Swirling rainbow" animations
	const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = /*{100, 50, 20}*/ { 5 , 2 , 1 };

	// How long (in milliseconds) to wait between animation steps for each of the "Snake" animations
	const uint8_t RGBLED_SNAKE_INTERVALS[] PROGMEM = /*{100, 50, 20}*/{ 10 , 10 , 2 };

	// How long (in milliseconds) to wait between animation steps for each of the "Knight" animations
	//const uint8_t RGBLED_KNIGHT_INTERVALS[] PROGMEM = {127, 63, 31};

	// How long (in milliseconds) to wait between animation steps for each of the "Twinkle" animations
	//const uint8_t RGBLED_TWINKLE_INTERVALS[] PROGMEM = {50, 25, 10};

	// These control which hues are selected for each of the "Static gradient" modes
	//const uint8_t RGBLED_GRADIENT_RANGES[] PROGMEM = {255, 170, 127, 85, 64};
#else
	#define IDLE_STATE_RGB_MODE		0
	#define	PRESSED_STATE_RGB_MODE	0
	#define	RESULT_STATE_RGB_MODE	0
#endif

/** @ingroup jr_bigswitch_magic8ball_keymap
*   @brief Our primary container for associating a ball state to its color and mode
*/
static sTable_entry g_led_mode_table[ kMAX_NUM_STATES ] = {
		{ .state =  kIDLE_STATE     , .color = { MY_ORANGE } , .rgb_mode  = IDLE_STATE_RGB_MODE  },
		{ .state =  kPRESSED_STATE  , .color = { MY_PURPLE }  , .rgb_mode = PRESSED_STATE_RGB_MODE },
		{ .state =  kRESULT_STATE   , .color = { MY_PURPLE }  , .rgb_mode = RESULT_STATE_RGB_MODE }
};
#define DEFAULT_LED_MODE    IDLE_STATE_RGB_MODE


#define BIGSWITCH_OLED_TIMEOUT_MS	20000
#define PRESSED_STATE_TIMEOUT_MS    5000   /* Duration of PRESSED state before transitioning to RESULT */
#define RESULT_STATE_TIMEOUT_MS     15000  /* Duration of RESET state before transitioning to IDLE */

#ifdef OLED_ENABLE
#define OLED_CLR_SCREEN()			oled_clear(); oled_render()
#endif

#define MAX_NUM_RESPONSES		20  /*5x4*/
const char *g_ball_responses[ MAX_NUM_RESPONSES  ] = { //https://en.wikipedia.org/wiki/Magic_8-ball
		//Affirmative                                 Non-Committal                   Negative
		"It is certain."     , "As I see it, yes"   , "Reply hazy, try again."     , "Don't count on it."   ,
		"It is decidedly so.", "Most likely."       , "Ask again later."           , "My reply is no."      ,
		"Without a doubt."   , "Outlook good."      , "Better not tell you now."   , "My sources say no."   ,
		"Yes definitely."    , "Yes."               , "Cannot predict now."        , "Outlook not so good." ,
		"You may rely on it.", "Signs point to yes.", "Concentrate and ask again." , "Very doubtful."
};
static int g_choosen_response;

//Fwd decls
bool set_rgb_animation_state( kBall_state ball_state );
bool do_oled_update( kBall_state ball_state );
bool magic8ball_task( void );
bool onswitchpress_magic8ball_state(void);
void kick_oled_off_watchdog(void);

/** @ingroup jr_bigswitch_magic8ball_keymap
*
*   @brief set state of ws2812 leds
*
*   @param  ball_state : ball state
*
*   @return true if successful, false otherwise
*   */
bool set_rgb_animation_state( kBall_state ball_state )
{
	static kBall_state prev_ball_state = kMAX_NUM_STATES;

	sHSV *pcolor;

	if ( ( ball_state < kIDLE_STATE ) ||
		 ( ball_state >= kMAX_NUM_STATES )
	   )
		return false;

	if( prev_ball_state != ball_state ){

		SET_LED_MODE( g_led_mode_table[ ball_state ].rgb_mode ) ; //no return value avail
		pcolor = &g_led_mode_table[ ball_state ].color;
		SET_COLOR( pcolor->hue , pcolor->sat , pcolor->val );
	}

	prev_ball_state = ball_state;

    return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_0;
}

static void render_8ball_graphic(void) {
  static const char PROGMEM magic8ball_oled[] = {
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f,
		  0x0f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f,
		  0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
		  0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0x1f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0xe0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x07, 0x03, 0x33, 0x73,
		  0x23, 0x03, 0x87, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1f, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x81, 0x80, 0x3c, 0x3c, 0x38,
		  0x10, 0x80, 0x81, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		  0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xfc,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xfc, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xfc, 0xf8,
		  0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
		  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8,
		  0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

  oled_write_raw_P(magic8ball_oled, sizeof(magic8ball_oled));
};
#endif

void kick_oled_off_watchdog(void){
	g_oled_off_timer = timer_read32();
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
	if (timer_elapsed(g_oled_off_timer) >= BIGSWITCH_OLED_TIMEOUT_MS ) {
		oled_off();
	}

    return true;
}
#endif

/** @ingroup jr_bigswitch_magic8ball_keymap
*
*   @brief  update display
*
*   @param  ball_state : ball state
*
*   @return true if successful, false otherwise
*   */
bool do_oled_update( kBall_state ball_state ){

	static kBall_state prev_ball_state = kMAX_NUM_STATES;

	if( prev_ball_state !=  ball_state ){
#ifdef OLED_ENABLE
		OLED_CLR_SCREEN();

		switch( ball_state ){
			case kIDLE_STATE:
				render_8ball_graphic();
				break;
			case kPRESSED_STATE:
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n     CONSULTING"), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n        THE"), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n      SPIRITS"), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				break;
			case kRESULT_STATE:
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write( g_ball_responses[ g_choosen_response ] , false );
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				oled_write_P(PSTR("\n "), false);
				break;
			default:
				break;
		}
#endif
	}

	prev_ball_state =  ball_state;
	return true;
}

/** @ingroup jr_bigswitch_magic8ball_keymap
*
*   @brief  primary feature
*
*   @param  ball_state : ball state
*
*   @return true if successful, false otherwise
*   */
bool magic8ball_task( void){

	bool rc;
	static kBall_state prev_ball_state = kMAX_NUM_STATES;

	//Look for state changes via big switch
	if( prev_ball_state !=  g_ball_state  ){
	     kick_oled_off_watchdog();

		 switch( g_ball_state ){
		 	 //from Idle to Pressed
			 case kPRESSED_STATE:
#ifdef AUDIO_ENABLE
				 PLAY_SONG(my_pressed_song);
#endif
				 //start timer to transition to Result state
				 g_state_timeout = timer_read32() + PRESSED_STATE_TIMEOUT_MS;
				 srand(timer_read32());
				 break;
			 case kRESULT_STATE:
#ifdef AUDIO_ENABLE
				 PLAY_SONG(my_result_song);
#endif
				 //start timer to return to Idle state
				 g_state_timeout = timer_read32() + RESULT_STATE_TIMEOUT_MS;
				 break;
			 default:
				 break;
		 }
	}

	prev_ball_state = g_ball_state;

    if ( false == do_oled_update( g_ball_state ) ){
    	return false;
    }

	rc = set_rgb_animation_state( g_ball_state );

	//Monitor for autostate changes due to timeouts
	if ( timer_expired32( timer_read32(), g_state_timeout ) ){
		 switch( g_ball_state ){
			 case kPRESSED_STATE:
				 g_ball_state = kRESULT_STATE;
				 g_choosen_response =  rand() % MAX_NUM_RESPONSES ;
				 break;
			 case kRESULT_STATE:
				 g_ball_state = kIDLE_STATE;
#ifdef AUDIO_ENABLE
				 audio_stop_all();
#endif
				 break;
			 default:
				 break;
		 }
	}

	return rc;

}

/** @ingroup jr_bigswitch_magic8ball_keymap
*
*   @brief  state change handler
*
*   @param  none
*
*   @return true if successful, false otherwise
*   */
bool onswitchpress_magic8ball_state(void){

	switch( g_ball_state ){
		case kIDLE_STATE:
			g_ball_state = kPRESSED_STATE;
			break;
		case kPRESSED_STATE:
		case kRESULT_STATE:
			g_ball_state = kIDLE_STATE;
			break;
		default:
			break;
	}

	return true;
}

/** @ingroup jr_bigswitch_magic8ball_keymap
*
*   @brief Our custom keycode handler
*
*   @param  keycode : keycode value as represented in our "custome keycodes" enumeration
*   @param  record  : ??
*
*   @return true to indicate finished handling keypress event
*   */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case kMAGIC:
          if (record->event.pressed) {
#ifdef OLED_ENABLE
        	  if( is_oled_on() ){
        		  onswitchpress_magic8ball_state();

        	  }else{
        		  kick_oled_off_watchdog();//reset our screen timer
        		  oled_on();//wake up screen
        	  }
#else
        	  onswitchpress_magic8ball_state();
#endif
          }
          //return true;
          return false;
          break;
      default:
    	  return true;
    	  break;
    }

    return true;
};

void matrix_scan_user(void) {
	magic8ball_task();
}


void keyboard_post_init_user(void) {

#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
#endif

    g_ball_state = kIDLE_STATE;
    kick_oled_off_watchdog();
    magic8ball_task();//get IDLE into effect now rather waiting for QMK to fire up the task

#ifdef AUDIO_ENABLE
    if(! audio_is_on() )
    	audio_on();
#endif
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
		[0] = LAYOUT_bigswitch( kMAGIC )
};

#ifndef BUILD_FOR_RELEASE
	#pragma GCC diagnostic pop
#endif
