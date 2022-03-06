#pragma once

#include "config_common.h"

#ifdef RGB_MATRIX_ENABLE
    #ifdef DISABLE_RGB_MATRIX_BREATHING
        #undef DISABLE_RGB_MATRIX_BREATHING
    #endif
    
    #ifdef DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
        #undef DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #endif 
    
    #ifdef DISABLE_RGB_MATRIX_SOLID_REACTIVE
        #undef DISABLE_RGB_MATRIX_SOLID_REACTIVE
    #endif
#endif

#if OLE_ENABLED
#    define OLED_FONT_H "keyboards/ungodly/launch_pad/keymaps/jr/glcdfont.c"
#endif
