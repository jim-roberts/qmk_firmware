#include QMK_KEYBOARD_H

/** @defgroup jr_sinc_keymap */

/** @addtogroup jr_sinc_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  keebio/sinc/rev2 -km jimr
  *  Clean :
  *     qmk compile -kb  keebio/sinc/rev2 -km clean
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *
  * @{
*/


#include "quantum.h"
#include "action_layer.h"
#include "action_util.h"

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#define ___X___         KC_NO
#define LEFT_ENCODER    0
#define RIGHT_ENCODER   1
#define KC_SCLCK        KC_SCROLLLOCK
#define CTL_SHFT( kc )  LCTL( LSFT(kc) )
#define ALT_SHFT( kc )  LALT( LSFT(kc) )
#define CTL_ALT_DEL     LCTL(LALT(KC_DEL))

//Text macros
#define SEL_WORD        CTL_SHFT( KC_RGHT )
#define SEL_LINE        LSFT( KC_DOWN )
#define COPY_IT         LCTL( KC_C )
#define PASTE_IT        LCTL( KC_V )
#define CNTR_MOUSE		LCTL( LALT ( KC_M ) ) /*works with powerscript to center mouse on main screen/monitor */

#define TOGGLE(x)           if( TRUE == x ){x = FALSE;}else{x = TRUE;}


/** @ingroup jr_sinc_keymap
*   @brief Our layer enumeration. Changing order will change layer order */
typedef enum {
    kDefault_Layer, //NOTE: Advise not to move relative to list order. Should always associate default to standard keyboard behaviour
    kRGB_Layer,
    kDevLayer,
    kLayer_NUM_LAYERS
} kLayers;

/** @ingroup jr_sinc_keymap
*   @brief Our custom keycode enumeration.*/
enum custom_keycodes {
    kCUSTOME_KEYCODES = SAFE_RANGE,
    kCYG_REPO,
    kCYG_TREE,
    kWIN_REPO,
	kKC_CODE,
	kKC_MK_DEF,
	kKC_MK_LCOV,
};

/** @ingroup jr_sinc_keymap
*
*   @brief Our custom keycode handler
*
*   @param  keycode : keycode value as reprented in our "custome keycodes" enumeration
*   @param  record  : ??
*
*   @return true to indicate finished handling keypress event
*   */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      /* start: custom keycodes for UNIX layer */
      case kCYG_REPO:
          if (record->event.pressed) {
            SEND_STRING("cd /cygdrive/c/Users/robertsj/Code/Repositories/MKS/\r\n");
          } /*else handle key released */
          return false;
          break;
      case kCYG_TREE:
          if (record->event.pressed) {
            SEND_STRING("tree -d\r\n");
          } /*else handle key released */
          return false;
          break;
      /* end: custom keycodes for UNIX layer */

      /* start: custom keycodes for WIN layer */
      case kWIN_REPO:
          if (record->event.pressed) {
            SEND_STRING("cd /d c:\\Users\\robertsj\\Code\\Repositories\\MKS\r\n");
          } /*else handle key released */
          return false;
          break;
      case kKC_CODE:
    	  if (record->event.pressed) {
    		  SEND_STRING("clear ; make def\r\n");
    	  }
          return false;
          break;
      case kKC_MK_DEF:
     	  if (record->event.pressed) {
     		  SEND_STRING("clear ; make cleanall def\r\n");
     	  }
    	  return false;
    	  break;
      case kKC_MK_LCOV:
     	  if (record->event.pressed) {
     		  SEND_STRING("clear ; make cleanall lcov\r\n");
     	  }
    	  return false;
    	  break;
      /* end: custom keycodes for UNIX layer */
    }

    return true;
};

#ifdef RGBLIGHT_ENABLE

    #define RGB_INCR_HUE()    rgblight_increase_hue_noeeprom()
    #define RGB_DECR_HUE()    rgblight_decrease_hue_noeeprom()

    //color explore/debug tool. consume percent value and calc equiv byte value
    //#define PERC_2_BYTE(  per  )  (uint8_t)( 255.0f * ( per / 100.0f ) )

    #define MY_BLUE     HSV_BLUE
    #define MY_GREEN    HSV_GREEN
    // 17.99  , 100% , 100%
    #define MY_ORANGE   0x07 , 0xFF,  0xFF /*HSV_ORANGE*/
    //185 , 100% , 100%
    //#define MY_PURPLE   185 , PERC_2_BYTE( 100 ) , PERC_2_BYTE( 100 )
    #define MY_PURPLE   0xB9 , 0xFF , 0xFF /*HSV_PURPLE*/

    #define SET_COLOR( h,s,v )      rgblight_sethsv_noeeprom( h,s,v );
    #define SET_LED_MODE( mod )     rgblight_mode_noeeprom( (uint16_t)mod ) ;

    /* You can recover some resources by ONLY defining support for specific animations in the config.h
    *  but if doing so, refactor this def check accordingly
    *
    *  Example:
    *
    *  config.h
    *    //#define RGBLIGHT_ANIMATIONS
    *    #define RGBLIGHT_EFFECT_SNAKE
    */
    #if defined RGBLIGHT_ANIMATIONS             || \
        defined RGBLIGHT_EFFECT_ALTERNATING     || \
        defined RGBLIGHT_EFFECT_BREATHING       || \
        defined RGBLIGHT_EFFECT_CHRISTMAS       || \
        defined RGBLIGHT_EFFECT_KNIGHT          || \
        defined RGBLIGHT_EFFECT_RAINBOW_MOOD    || \
        defined RGBLIGHT_EFFECT_RAINBOW_SWIRL   || \
        defined RGBLIGHT_EFFECT_RGB_TEST        || \
        defined RGBLIGHT_EFFECT_SNAKE           || \
        defined RGBLIGHT_EFFECT_STATIC_GRADIENT || \
        defined RGBLIGHT_EFFECT_TWINKLE
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
        #define RGBLIGHT_ANIMATION_SUPPORT_ENABLED
        #define DEFAULT_LED_MODE    RGBLIGHT_MODE_STATIC_LIGHT

        //How long (in ms) to wait between animation steps
        //param 1 - on time
        //param 2 - off time
        //param 3 - delay between off to on
        const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM =        {30, 20, 10, 5};
        //const uint8_t RGBLED_RAINBOW_MOOD_INTERVALS[] PROGMEM =     {120, 60, 30};
        //const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM =    {100, 50, 20};
        const uint8_t RGBLED_SNAKE_INTERVALS[] PROGMEM =            {100, 50, 20};
        //const uint8_t RGBLED_KNIGHT_INTERVALS[] PROGMEM =           {100, 50, 20};

       /* These control which colors are selected for the gradient mode */
       // const uint16_t RGBLED_GRADIENT_RANGES[] PROGMEM =           {360, 240, 180, 120, 90};

        //const uint8_t RGBLED_TWINKLE_INTERVALS[] PROGMEM =          {50, 25, 10};

        //value is fixed to 500 in rgblight.c
        //#undef   RGBLIGHT_EFFECT_CHRISTMAS_INTERVAL
        //#define  RGBLIGHT_EFFECT_CHRISTMAS_INTERVAL                 1000

        //const uint16_t RGBLED_RGBTEST_INTERVALS[] PROGMEM =         {1024};
    #endif //RGBLIGHT_ANIMATIONS
#else   //rgb light not supported
        #define SET_COLOR( h ,s ,v )
        #define SET_LED_MODE( mod  )
        #define SET_LED_MODE_AND_STORE( mod , layer )

        #define RGB_INCR_HUE()
        #define RGB_DECR_HUE()

        //#define DEFAULT_LED_MODE
        //#define MIC_MUTED_EFFECT    RGBLIGHT_MODE_STATIC_LIGHT
        //#define MIC_UNMUTED_EFFECT  RGBLIGHT_MODE_STATIC_LIGHT

#endif //RGBLIGHT_ENABLE


#if defined RGBLIGHT_ENABLE && defined RGBLIGHT_ANIMATION_SUPPORT_ENABLED
    /** @ingroup jr_sinc_keymap
    *   @brief define storage type for led color info */
    typedef struct {
        uint8_t hue;    /**< HUE           */
        uint8_t sat;    /**< Saturation    */
        uint8_t val;    /**< Value         */
    } sHSV;

    /** @ingroup jr_sinc_keymap
    *   @brief define storage type to hold layer led color and modes
    *
    *   @details We include an explicit layer member to avoid implicit association of enumerated order to any table ordering
    *
    */
    typedef struct {
        kLayers   layer;    /**< layer this color and mode are applicable to  */
        sHSV      color;    /**< color for layer                              */
        uint16_t  mode;     /**< RGB mode for layer, see RGBLIGHT_ANIMATIONS  */
    } sLedInfo;

    /** @ingroup jr_sinc_keymap
    *   @brief Our primary container for associating a layer to its color and mode
    */
    sLedInfo g_led_info[ kLayer_NUM_LAYERS ] = {
        { .layer = kDefault_Layer,  .color = { MY_ORANGE }, .mode = DEFAULT_LED_MODE },
        { .layer = kRGB_Layer,      .color = { MY_PURPLE  }, .mode = DEFAULT_LED_MODE },
        { .layer = kDevLayer,       .color = { MY_GREEN   }, .mode = RGBLIGHT_MODE_BREATHING },
        #if 0
        { .layer = ??, .color = { MY_BLUE  }, .mode = RGBLIGHT_MODE_BREATHING },
        #endif
    };

    /** @ingroup jr_sinc_keymap
    *   @brief  Find layer led info within our led info table
    *
    *   @details Find layer led info within our led info table.
    *            Used primarily to locate the section of memory we which to change ( color and mode relative to layer )
    *            Table entries do not need to follow enumeration order.
    *            We do this to avoid implicit association of enumerated order to any table ordering
    *
    *   @param  layer  [in] :  layer to locate
    *
    *   /sa g_led_info
    *
    *   @return  index in table where layer info is stored
    */
    uint16_t find_layer_table_index( kLayers layer ){
        uint16_t idx;

        for( idx = 0 ; idx < kLayer_NUM_LAYERS ; ++idx ){
            if ( layer == g_led_info[ idx ].layer )
                return idx;
        }

        return kDefault_Layer;
    }

    #define RGB_LED_INFO_ENABLED
#endif //RGBLIGHT_ENABLE && defined RGBLIGHT_ANIMATION_SUPPORT_ENABLED


/** @ingroup jr_sinc_keymap
*   @brief  Manage application of RGB ( mode and color ) when transitioning to a new layer.
*
*   @param  new_layer  [in] :  new layer to move into
*
*   @return  --
*/
void move_to_layer( kLayers new_layer ){

#ifdef    RGB_LED_INFO_ENABLED
    sHSV *pcolor;
    sLedInfo * pInfo;
    uint16_t *pmode;

    pInfo = &g_led_info[ find_layer_table_index( new_layer ) ];
    pcolor = &pInfo->color;
    pmode = &pInfo->mode;
#endif

    SET_COLOR( pcolor->hue , pcolor->sat, pcolor->val);
    SET_LED_MODE( *pmode );

    layer_move(new_layer);
}

/** @ingroup jr_sinc_keymap
*   @brief  Manage simple state machine for advancing to next layer while allowing for rollover
*
*   @param  curr_layer  [in] :  current layer
*
*   @return  --
*/
void move_to_next_layer( void ){

   kLayers curr_layer = get_highest_layer(layer_state); //'layer_state' is global QMK bsp var
   kLayers next_layer = ++curr_layer;

   if( kLayer_NUM_LAYERS == next_layer ){
       next_layer = (kLayers)0;
   }

    move_to_layer( next_layer );
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  #if 0 // del and backspace on same row
      [kDefault_Layer] = LAYOUT_80_with_macro(
        RGB_MOD,                      KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_APP,    KC_MUTE,
        KC_PAUSE  ,   KC_HOME,        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC,   KC_INS,
        KC_PSCREEN,   KC_DEL,         KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,            KC_END,
        KC_SCLCK  ,   KC_NUMLOCK,     KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,             KC_PGUP,
        SEL_WORD  ,  SEL_LINE,        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,              KC_PGDN,
        COPY_IT   ,  PASTE_IT,       KC_LCTL, KC_LGUI, KC_LALT, MO(kDevLayer),   KC_SPC,  KC_SPC,           KC_SPC,  KC_SPC,  MO(kRGB_Layer),   KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
      ),
  #else //del on row above backspace
      [kDefault_Layer] = LAYOUT_80_with_macro(
        RGB_MOD    ,                  KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_INS,    KC_MUTE,
		KC_HOME    ,   KC_PAUSE,      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_BSPC,   KC_DEL,
        KC_END     ,   CNTR_MOUSE,    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,            KC_PGUP,
		kKC_MK_DEF ,  kKC_MK_LCOV,    KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,             KC_PGDN,
        SEL_WORD   ,  SEL_LINE,       KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,             kKC_CODE,
         COPY_IT   ,  PASTE_IT,       KC_LCTL, KC_LGUI, KC_LALT, MO(kDevLayer),   KC_SPC,  KC_SPC,           KC_SPC,  KC_SPC,  MO(kRGB_Layer),   KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
      ),
  #endif
  //original
  //KC_F9,       KC_F10,          KC_LCTL, KC_LALT, KC_LGUI, MO(1),   KC_SPC,  KC_SPC,           MO(1),  KC_SPC,   KC_RALT, KC_RCTL, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT
  [kRGB_Layer] = LAYOUT_80_with_macro(
    _______,                      _______, KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  _______,           _______,  _______,
    RGB_HUI,      RGB_HUD,        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,
    RGB_SAI,      RGB_SAD,        _______, _______, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    RGB_VAI,      RGB_VAD,        _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______,          _______,  _______,
    KC_GESC,      RGB_TOG,        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    RGB_MOD,      _______,        _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,  _______
  ),
  [kDevLayer] = LAYOUT_80_with_macro(
    _______,                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______,  _______,
    _______,      _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,
    _______,      _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
	kKC_MK_DEF,  kKC_MK_LCOV,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,  _______,
	kCYG_TREE,   KC_PWR,       _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
	kCYG_REPO,   kWIN_REPO,     _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,  _______
  ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if ( LEFT_ENCODER == index ) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if ( RIGHT_ENCODER == index ) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}

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

void keyboard_post_init_user(void) {

#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
	rgblight_sethsv_noeeprom( HSV_GREEN );
    rgblight_mode_noeeprom( (uint8_t) RGBLIGHT_MODE_KNIGHT ) ;
#endif
};
/** @} */
