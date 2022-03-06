#include QMK_KEYBOARD_H

/** @defgroup jr_madnoodle_keymap */

/** @addtogroup jr_madnoodle_keymap
  * @brief Keymap
  *
  * @details
  *  Build :
  *     qmk compile -kb  themadnoodle/noodlepad -km jimr
  *  Clean :
  *     qmk compile -kb  themadnoodle/noodlepad -km clean
  *
  * \par Modification History
  *    Date     | Initials | Notes
  *    -------- | -------- | -----------------------------------------------------------------------
  *    08/01/20 |   JR     | Initial File
  *    08/07/20 |   JR     | Coupled rgb color to layer (i.e.g eahc layer has specific color )
  *    08/23/20 |   JR     | Added 4th layer and custom keycodes
  *    09/07/20 |   JR     | Add win cmd dir to MKS repo
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

#define MUTED       TRUE
#define UNMUTED     FALSE

// NOTES:
//   * To define as transparaent to base layer ( 0 ) use : _______
//   * To map key to nothing : ___X___
//   * If witnessing odd / unexplained behavior, try CLEAR EEPROM from QMK Toolbox and (re)flash
//     Also review QMK API to ensure you are not executing behaviors that implicitly work with eeprom. A code dive into the bsp may be required.
//     I've tried to structure the code to avoid this situation. Note that there are QMK behaviors that read/write into eeprom by default.
//     QMK documentation does not explicitly state this. It is only implied by the existence of implicit alternative
//     function calls that do not access eeprom.
//     e.g.  rgblight_enable() vs rgblight_enable_noeeprom()
//
//
//   Useful Refs :  https://matt3o.com/building-a-keyboard-firmware/
//                  eeprom - https://beta.docs.qmk.fm/using-qmk/guides/custom_quantum_functions

//(re)define some useful items
#ifndef _______
#define _______         KC_TRNS             /*transparent to layer*/
#endif
#define ___X___         KC_NO               /* do nothing */
#define CTL_SHFT( kc )  LCTL( LSFT(kc) )
#define ALT_SHFT( kc )  LALT( LSFT(kc) )

//MS TEAMS
//https://support.microsoft.com/en-us/office/keyboard-shortcuts-for-microsoft-teams-2e8e2a70-e8d8-4a19-949b-4c36dd5292d2
//Note: Windows Teams window must be active window!
//Should consider removing these and coupling behavior to custom Fx key + autohotkey or equiv
#define MST_TOG_VIDEO       CTL_SHFT( KC_O )
#define MST_TOG_MUTE        CTL_SHFT( KC_M )
#define MST_GOTO_SHARE      CTL_SHFT( KC_SPC )
#define MST_OPEN_TEAMS      LCTL( KC_3 )
#define MST_OPEN_CALNDR     LCTL( KC_4 )

#define MIC_DEFAULT_STATE   MUTED
#define TOGGLE(x)           if( TRUE == x ){x = FALSE;}else{x = TRUE;}
/* For reliable mic control in MsTeams we must leverage external tool(s) to avoid window context challenges that cannot be solved with qmk
 * We can ( attemp to ) use autohotkey to intercept these keys and handle muting/unmuting. YMMV
*/
#define MIC_MUTE_KEY        KC_F24
#define MIC_UNMUTE_KEY      KC_F23
#define PSCR_PASTE_KEY      KC_F20  /*Pass to autohotkey script : prntsrc, open irfanview and paste*/

/** @ingroup jr_madnoodle_keymap
*   @brief Our custom keycode enumeration.*/
enum custom_keycodes {
    kCUSTOME_KEYCODES = SAFE_RANGE,
    kCYGWIN_ROOT_TO_REPO,
    kCYGWIN_TREE,
    kWINCMD_ROOT_TO_REPO
};

/** @ingroup jr_madnoodle_keymap
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

        //MS TEAMS RGB settings
        #define MIC_MUTED_EFFECT    RGBLIGHT_MODE_STATIC_LIGHT
        #define MIC_UNMUTED_EFFECT  RGBLIGHT_MODE_SNAKE             /* visual alert when unmuted , recommend using somethin flashy */

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

/** @ingroup jr_madnoodle_keymap
*   @brief Our layer enumeration. Changing order will change layer order */
typedef enum {
    kLayer_EXT_KEYS = 0,
    kLayer_MEDIA,
    kLayer_RGB_CTRL,
    kLayer_MS_TEAMS,
    kLayer_UNIX,
    kLayer_NUM_LAYERS
} kLayers;

#if defined RGBLIGHT_ENABLE && defined RGBLIGHT_ANIMATION_SUPPORT_ENABLED
    /** @ingroup jr_madnoodle_keymap
    *   @brief define storage type for led color info */
    typedef struct {
        uint8_t hue;    /**< HUE           */
        uint8_t sat;    /**< Saturation    */
        uint8_t val;    /**< Value         */
    } sHSV;

    /** @ingroup jr_madnoodle_keymap
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

    /** @ingroup jr_madnoodle_keymap
    *   @brief Our primary container for associating a layer to its color and mode
    */
    sLedInfo g_led_info[ kLayer_NUM_LAYERS ] = {
        { .layer = kLayer_EXT_KEYS, .color = { MY_ORANGE }, .mode = DEFAULT_LED_MODE },
        { .layer = kLayer_MEDIA,    .color = { HSV_CYAN  }, .mode = DEFAULT_LED_MODE },
        { .layer = kLayer_RGB_CTRL, .color = { MY_BLUE   }, .mode = DEFAULT_LED_MODE },
        { .layer = kLayer_MS_TEAMS, .color = { MY_PURPLE }, .mode = DEFAULT_LED_MODE },
        { .layer = kLayer_UNIX,     .color = { MY_GREEN  }, .mode = RGBLIGHT_MODE_BREATHING },
    };

    /** @ingroup jr_madnoodle_keymap
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

        return kLayer_EXT_KEYS;
    }

    #define RGB_LED_INFO_ENABLED
#endif //RGBLIGHT_ENABLE && defined RGBLIGHT_ANIMATION_SUPPORT_ENABLED

#ifdef RGB_LED_INFO_ENABLED
    /** @ingroup jr_madnoodle_keymap
*   @brief  set led mode and store led information
*
*   @param  mode  [in] :  new rgb mode
*   @param  layer [in] :  layer to save rgb mode for

*   /sa g_led_info
*
*   @return  index in table where layer info is stored
*/
 void set_led_mode_and_store( uint16_t mode , kLayers layer ){
      SET_LED_MODE( mode ) ;
      g_led_info[ find_layer_table_index( layer ) ].mode = mode;
 }
 #endif

/** @ingroup jr_madnoodle_keymap
*   @brief Our tap dance enumeration
*/
enum {
    kTapDance_Encoder = 0,
    kTapDance_RGB_Layer_Select_1,
    kTapDance_RGB_Layer_Select_2,
    kTapDance_RGB_Layer_Select_3,
    kTapDance_RGB_Layer_Select_4,
    kTapDance_MicMute,
    kTapDance_MicUnmute
};

/** @ingroup jr_madnoodle_keymap
*   @brief  store muted/unmuted state of microphone so we can apply other behavours to this state
*/
static uint8_t g_bMicMuted = MIC_DEFAULT_STATE;

/** @ingroup jr_madnoodle_keymap
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


/** @ingroup jr_madnoodle_keymap
*   @brief  Manage simple state machine for advancing to next layer while allowing for rollover
*
*   @param  curr_layer  [in] :  current layer
*
*   @return  --
*/
void move_to_next_layer( kLayers curr_layer ){

   kLayers next_layer = ++curr_layer;

   if( next_layer == kLayer_NUM_LAYERS ){
       next_layer = (kLayers)0;
   }

    move_to_layer( next_layer );
};

/** @ingroup jr_madnoodle_keymap
*   @brief  Define common behavour for what a multitap of the encoder button will do for every layer
*
*   @details
*
*        Encoder mulit tap behavours:
*             4 taps  =  return RGB mode to default state
*             3 taps  =  change RGB mode to breathing state
*             2 taps  =  move to next layer
*
*   @param  taps        [in] :  number of taps
*   @param  curr_layer  [in] :  current layer
*
*   @return  --
*/
void encoder_multi_tap_action( uint8_t taps, kLayers curr_layer){

  if( 4 <= taps ){
#ifdef RGB_LED_INFO_ENABLED
        set_led_mode_and_store( DEFAULT_LED_MODE , curr_layer );
#endif
    } else if ( 3 == taps ) {
#ifdef RGB_LED_INFO_ENABLED
        set_led_mode_and_store( RGBLIGHT_MODE_BREATHING , curr_layer );
#endif
    } else if ( 2 == taps ) {
        move_to_next_layer( curr_layer );
    } else {
        //whoops! Shouldn't be here...
    }
}


/** @ingroup jr_madnoodle_keymap
*   @brief  Define common behavour for muting / unmuting the microphone
*
*   @return  --
*/
void mic_mute_state_action(void){
    if( TRUE == g_bMicMuted ){
#ifdef RGB_LED_INFO_ENABLED
        set_led_mode_and_store( MIC_MUTED_EFFECT , kLayer_MS_TEAMS );
#endif
        tap_code( MIC_MUTE_KEY );
    } else {
#ifdef RGB_LED_INFO_ENABLED
        set_led_mode_and_store( MIC_UNMUTED_EFFECT , kLayer_MS_TEAMS );
#endif
        tap_code( MIC_UNMUTE_KEY );
    }
}

/** @ingroup jr_madnoodle_keymap
*   @brief  qmk function for intercepting taps on the encoder
*
*   @details
*           First point of entry on handling key tap of the ecoder button
*
*           1   tap = execute specifc keycode dependant on layer ( similiar to mapping kecode directly in the 'keymaps' table )
*           2+ taps = custom handler , which at this point is common behavour regardless of layer
*
*   @param  state     [in] :  tap dance state ( count , , etc )
*   @param  user_data [in] :  user data ( not used )
*
*   @return  --
*/
void encoder_tap_decode(qk_tap_dance_state_t *state, void *user_data) {

 switch ( get_highest_layer(layer_state) ) {
    case kLayer_EXT_KEYS :
        if( 1 < state->count ) {
            encoder_multi_tap_action( state->count , kLayer_EXT_KEYS );
        } else { //single tap
            tap_code( KC_MUTE );
        }
        break;
    case kLayer_MEDIA :
        if( 1 < state->count ) {
            encoder_multi_tap_action( state->count , kLayer_MEDIA );
        } else { //single tap
            tap_code( KC_MUTE );
        }
        break;
    case kLayer_RGB_CTRL :
        if( 1 < state->count ) {
            encoder_multi_tap_action( state->count , kLayer_RGB_CTRL );
        } else { //single tap
            #ifdef RGBLIGHT_ENABLE
                rgblight_toggle_noeeprom();
            #endif
        }
        break;
    case kLayer_MS_TEAMS :
        if( 1 < state->count ) {
            encoder_multi_tap_action( state->count , kLayer_MS_TEAMS );
        } else { //single tap
            TOGGLE( g_bMicMuted );
            mic_mute_state_action();
        }
        break;
    case kLayer_UNIX :
        if( 1 < state->count ) {
            encoder_multi_tap_action( state->count , kLayer_UNIX );
        }else { //single tap
            tap_code( KC_MUTE );
        }
        break;
  }

  reset_tap_dance(state);
}

/// Create new handlers based on ACTION_TAP_DANCE_FN but modified to leverage user data field  ///
#define  ACTION_TAP_DANCE_FN_WITH_UDATA( usr_fn , usr_data ) \
                       { .fn = {NULL, usr_fn, NULL}, .user_data = (void*)usr_data, }

#define ACTION_TAP_DANCE_FN_ADVANCED_WITH_UDATA(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset , usr_data ) \
        { .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset}, ..user_data = (void*)usr_data }

#define ACTION_TAP_DANCE_FN_ADVANCED_TIME_WITH_UDATA(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, tap_specific_tapping_term) \
        { .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset , usr_data }, .user_data = (void*)usr_data, .custom_tapping_term = tap_specific_tapping_term, }
//////////////////////////////////////////////////////////////////////////////

/** @ingroup jr_madnoodle_keymap
*   @brief  Define storage container for rgb tap associations
*/
typedef struct {
    kLayers dest_layer;   /**< destination layer               */
    void (*rgb_fn)(void); /**< rgb 'action' to take if 1 tap   */
} sRgbTapCodeInfo;

/** @ingroup jr_madnoodle_keymap
*   @brief  Define association of layer to rgb function to call
*
*   @details Each index represents both a layer and RGB "keycode" action
*            We are purposefull avoiding the use of the enum labels in the layer defines
*            This will allow us to move layers ( via layer enum redef) without affecting tap behavours
*/
#ifdef    RGB_LED_INFO_ENABLED
const sRgbTapCodeInfo g_rgb_tap_info[] = {
    { .dest_layer = 0 , .rgb_fn = rgblight_increase_speed },
    { .dest_layer = 1 , .rgb_fn = rgblight_decrease_speed },
    { .dest_layer = 2 , .rgb_fn = rgblight_increase_val_noeeprom  },
    { .dest_layer = 3 , .rgb_fn = rgblight_decrease_val_noeeprom }
};
#else
    const sRgbTapCodeInfo g_rgb_tap_info[] = {
    { .dest_layer = 0 , .rgb_fn = NULL },
    { .dest_layer = 1 , .rgb_fn = NULL },
    { .dest_layer = 2 , .rgb_fn = NULL },
    { .dest_layer = 3 , .rgb_fn = NULL }
};
#endif

/** @ingroup jr_madnoodle_keymap
*   @brief  Define behavour of select keys in RGB layer
*
*   @details Led colors can be manipulated in RGB layer.
*            This mechanism will allow base level effects for single taps
*            while also creating method for applying that color to a a layer.
*
*            1. Single tap will invoke stndard keymap like behavour...
*               ..for RGB this means further manipulation of color, effects. etc.
*            2. Multitap will apply the led color to a layer. The layer is dependant on which key is tapped ( i.e one key assigned to one layer )
*
*   @param  state     [in] :  tap dance state ( count , , etc )
*   @param  user_data [in] :  user data (  we will use it to pass led information )
*
*    /sa g_rgb_tap_info
*
*   @return  --
*/
void rgb_tap_decode(qk_tap_dance_state_t *state, void *user_data) {

#ifdef    RGB_LED_INFO_ENABLED
    sRgbTapCodeInfo *p_tap_info = (sRgbTapCodeInfo*)user_data;

   //2+ tap = store color into layer as defined in g_rgb_tap_info[]
    if ( 2 <= state->count ) {

        sLedInfo * pInfo = &g_led_info[ p_tap_info->dest_layer ];
        sHSV *pcolor = &pInfo->color;

        pcolor->hue = rgblight_get_hue();
        pcolor->sat = rgblight_get_sat();
        pcolor->val = rgblight_get_val();

        //Send visual acknowledgement of storage
        //Note : +1  -> layer enum begins @ 0, i.e. layer 0 blink 1x, layer 1 blink 2x , etc
        for( unsigned blink = 0 ; blink < ( p_tap_info->dest_layer + 1 ) ; ++blink ) {
            rgblight_disable();
            _delay_ms( 250 );
            rgblight_enable();
            _delay_ms( 250 );
        }
    //1 tap to invoke RGB effect/color handling as defined in g_rgb_tap_info[]
    } else {
        p_tap_info->rgb_fn();
    }
#endif

  reset_tap_dance(state);
}

void unmute_tap_decode(qk_tap_dance_state_t *state, void *user_data) {
  g_bMicMuted = FALSE;
  mic_mute_state_action();
  reset_tap_dance(state);
}

void mute_tap_decode(qk_tap_dance_state_t *state, void *user_data) {
  g_bMicMuted = TRUE;
  mic_mute_state_action();
  reset_tap_dance(state);
}

/*! @brief primary container for defining tap dance actions */
qk_tap_dance_action_t tap_dance_actions[] = {
  [kTapDance_Encoder] = ACTION_TAP_DANCE_FN( encoder_tap_decode ),
  //[kTapDance_Encoder] = ACTION_TAP_DANCE_FN_ADVANCED_TIME( encode_tap_decode , NULL , NULL , 1000u )
  [kTapDance_RGB_Layer_Select_1] = ACTION_TAP_DANCE_FN_WITH_UDATA( rgb_tap_decode , &g_rgb_tap_info[ 0 ]),
  [kTapDance_RGB_Layer_Select_2] = ACTION_TAP_DANCE_FN_WITH_UDATA( rgb_tap_decode , &g_rgb_tap_info[ 1 ]),
  [kTapDance_RGB_Layer_Select_3] = ACTION_TAP_DANCE_FN_WITH_UDATA( rgb_tap_decode , &g_rgb_tap_info[ 2 ]),
  [kTapDance_RGB_Layer_Select_4] = ACTION_TAP_DANCE_FN_WITH_UDATA( rgb_tap_decode , &g_rgb_tap_info[ 3 ]),
  [kTapDance_MicUnmute] = ACTION_TAP_DANCE_FN( unmute_tap_decode ),
  [kTapDance_MicMute] = ACTION_TAP_DANCE_FN( mute_tap_decode )
};

/*! @brief primary container for defining key mapping. */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* LAYER 0 (ENCODER)
    * ,--------------------------.
    * | Print |   MUTE   | Pause |
    * | Screen| (Layer1) |       |  ENCODER : TAP - MUTE , DOUBLE TAP (goto Layer 1) / KNOB (VOLUME CONTROL)
    * |-------+----------+-------|
    * | Insert| Home     | Pg Up |
    * |-------+----------+-------|
    * | Del   | End      | Pg Dn |
    * `--------------------------'
    */
    [kLayer_EXT_KEYS] = LAYOUT_ortho_3x3(
        PSCR_PASTE_KEY, TD(kTapDance_Encoder),  KC_PSCR,
        KC_INS,         KC_HOME,                KC_PGUP,
        KC_DEL,         KC_END,                 KC_PGDN
    ),


    /* LAYER 1
     * ,--------------------------.
     * |       |   MUTE   |       |
     * |   <<  | (Layer2) |  >>   |  ENCODER : TAP - MUTE , DOUBLE TAP (goto Layer 1) / KNOB ( VOLUME CONTROL )
     * |-------+----------+-------|
     * |  STOP | PLAY     | MEDIA |
     * |-------+----------+-------|
     * | CALC  | MAIL     | PC/FN |
     * `--------------------------'
     */
    [kLayer_MEDIA] = LAYOUT_ortho_3x3(
      KC_MPRV,       TD(kTapDance_Encoder),  KC_MNXT,
      KC_MSTP,       KC_MPLY,                KC_MSEL,
      KC_MRWD,       KC_MFFD,                ___X___
    ),

    /* LAYER 2 - note if layer_state_set_user() with RGB is installed, effects will be nullified by layer change
     * ,--------------------------.
     * |       |   MUTE   |       |
     * | MODE+ | (Layer3) | MODE- |  ENCODER : TAP - MUTE , DOUBLE TAP (goto Layer 3) / KNOB (Hue Control)
     * |-------+----------+-------|
     * |  SPD- |  SPD+    |Bright |
     * |-------+----------+-------|
     * |  SAT+ |  SAT-    |       |
     * `--------------------------'
     */
    [kLayer_RGB_CTRL] = LAYOUT_ortho_3x3(
      RGB_MOD,                          TD(kTapDance_Encoder),            RGB_RMOD,
      TD(kTapDance_RGB_Layer_Select_1), TD(kTapDance_RGB_Layer_Select_2), TD(kTapDance_RGB_Layer_Select_3),
      TD(kTapDance_RGB_Layer_Select_4), RGB_SAI,                          RGB_SAD
    ),

    /* LAYER 3
     * ,--------------------------.
     * |       | MUTE MIC |  MUTE |
     * |       | (Layer0) |       |  ENCODER : TAP - MUTE , DOUBLE TAP (goto Layer 4) / KNOB ( VOLUME CONTROL )
     * |-------+----------+-------|
     * | TEAMS | SHARE    | CALNDR|
     * |-------+----------+-------|
     * | VIDEO |          |       |
     * `--------------------------'
     */

    [kLayer_MS_TEAMS] = LAYOUT_ortho_3x3(
      ___X___,          TD(kTapDance_Encoder),  KC_MUTE,
      MST_OPEN_TEAMS,   MST_GOTO_SHARE,         MST_OPEN_CALNDR,
      MST_TOG_VIDEO,    TD(kTapDance_MicMute),  TD(kTapDance_MicUnmute)
    ),


    /* LAYER 4
     * ,--------------------------.
     * |       |   MUTE   |       |
     * |       |          |       |  ENCODER : TAP - MUTE , DOUBLE TAP (goto Layer 1) / KNOB ( VOLUME CONTROL )
     * |-------+----------+-------|
     * |       |          |       |
     * |-------+----------+-------|
     * | CALC  | MAIL     | PC/FN |
     * `--------------------------'
     */
    [kLayer_UNIX] = LAYOUT_ortho_3x3(
      KC_PSCR,        TD(kTapDance_Encoder),  kCYGWIN_TREE,
      PSCR_PASTE_KEY, KC_MAIL,                KC_CALC,
      ___X___,        kWINCMD_ROOT_TO_REPO,   kCYGWIN_ROOT_TO_REPO
    )
};

#ifdef ENCODER_ENABLE
/** @ingroup jr_madnoodle_keymap
*   @brief  define behavour(s) for each layer when encoder is rotated
*
*   @details For all layers but RGB , rotating encoder effects system volume
*            For RGB, rotating causes a change to the leds HUE
*
*   @param  index     [in] :  encoder index number ( applicable if more than one )
*   @param  clockwise [in] :  true = clockwise , false = counter-clockwise
*
*   @return  --
*/
bool encoder_update_user(uint8_t index, bool clockwise) {

    switch ( get_highest_layer(layer_state) ) {
        case kLayer_RGB_CTRL:
            if (clockwise) {
              RGB_INCR_HUE();
            } else {
              RGB_DECR_HUE();
            }
            break;
        case kLayer_EXT_KEYS:
        case kLayer_MEDIA:
        case kLayer_MS_TEAMS:
        default:
            if (clockwise) {
              tap_code(KC_VOLU);
            } else {
              tap_code(KC_VOLD);
            }
            break;
    }

    return false;
}
#endif

/** @ingroup jr_madnoodle_keymap
*   @brief  Kickstart RGB effect
*
*   @return  --
*/
void keyboard_post_init_user(void) {

#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    //SET_COLOR(g_layer_colors[ kLayer_EXT_KEYS ].hue , g_layer_colors[ kLayer_EXT_KEYS ].sat , g_layer_colors[ kLayer_EXT_KEYS ].val);
    rgblight_sethsv_noeeprom( MY_ORANGE );
    rgblight_mode_noeeprom( DEFAULT_LED_MODE );
#endif

    //attempt an EEPROM corruption recovery
    /*
    if ( kLayer_EXT_KEYS != get_highest_layer(layer_state) ){
        eeconfig_init();
        set_single_persistent_default_layer( kLayer_EXT_KEYS );
    }
    */

    mic_mute_state_action();
};
 /** @} */

#if 0
#define ACTION_TAP_DANCE_FN_ADVANCED_TIME_WITH_UDATA(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, tap_specific_tapping_term) \
        { .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset , usr_data }, .user_data = (void*)usr_data, .custom_tapping_term = tap_specific_tapping_term, }

typedef enum {
	kDance_CLN_SCL,
	kDance_DQUOT_QUOT,
	kTotalDances
}kTapDances;

typedef struct {
	uint16_t single_tap_keycode;
	uint16_t single_hold_keycode;
	uint16_t double_tap_keycode;
	uint16_t double_single_tap_keycode;
} sTapKeycodes;


void on_dance(qk_tap_dance_state_t *state, void *user_data) {

	sTapKeycodes *p_codes = (sTapKeycodes*)user_data;

    if(state->count == 3) {
        tap_code16(p_codes->single_tap_keycode);
        tap_code16(p_codes->single_tap_keycode);
        tap_code16(p_codes->single_tap_keycode);
    }
    if(state->count > 3) {
        tap_code16(p_codes->single_tap_keycode);
    }
}
void dance_finished(qk_tap_dance_state_t *state, void *user_data) {

	sTapKeycodes *p_codes = (sTapKeycodes*)user_data;

    dance_state.step = dance_standard_double_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: register_code16(p_codes->single_tap_keycode); break;
        case SINGLE_HOLD: register_code16(p_codes->single_hold_keycode); break;
        case DOUBLE_TAP: register_code16(p_codes->double_tap_keycode); register_code16(p_codes->double_tap_keycode); break;
        case DOUBLE_SINGLE_TAP: tap_code16(p_codes->double_single_tap_keycode); register_code16(p_codes->double_single_tap_keycode);
    }
}

void dance_reset(qk_tap_dance_state_t *state, void *user_data) {

	sTapKeycodes *p_codes = (sTapKeycodes*)user_data;

    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP: unregister_code16( p_codes->single_tap_keycode ); break;
        case SINGLE_HOLD: unregister_code16( p_codes->single_hold_keycode ); break;
        case DOUBLE_TAP: unregister_code16( p_codes->double_tap_keycode ); break;
        case DOUBLE_SINGLE_TAP: unregister_code16( p_codes->double_single_tap_keycode ); break;
    }
    dance_state.step = 0;
}

sTapKeycodes tap_keycodes[ 	kTotalDances ] = {
		[kDance_CLN_SCL] =
			{ .single_tap_keycode = KC_COLN ,
			  .single_hold_keycode = KC_SCLN ,
			  .double_tap_keycode = KC_COLN ,
			  .double_single_tap_keycode = KC_SCLN
			},
		[kDance_DQUOT_QUOT] =
					{ .single_tap_keycode = KC_DQUOT ,
					  .single_hold_keycode = KC_QUOT ,
					  .double_tap_keycode = KC_DQUOT ,
					  .double_single_tap_keycode = KC_QUOT
					}
};

qk_tap_dance_action_t tap_dance_actions[ kTotalDances ] = {
        [ kDance_CLN_SCL ]    = ACTION_TAP_DANCE_FN_ADVANCED_TIME_WITH_UDATA( on_dance, dance_finished, dance_reset , &tap_keycodes[ kDance_CLN_SCL ]),
		[ kDance_DQUOT_QUOT ] = ACTION_TAP_DANCE_FN_ADVANCED_TIME_WITH_UDATA( on_dance, dance_finished, dance_reset , &tap_keycodes[ kDance_DQUOT_QUOT ])
};
#endif
