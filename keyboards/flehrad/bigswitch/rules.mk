# MCU name
MCU = atmega32u4

# Bootloader selection
#BOOTLOADER = caterina

#JR my magic8ball Big switch uses Elite-C
BOOTLOADER = atmel-dfu

# Build Options
#   comment out to disable the options.
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE  = no	# Mouse keys
EXTRAKEY_ENABLE  = no	# Audio control and System control
CONSOLE_ENABLE  = no	# Console for debug
COMMAND_ENABLE  = no    # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE  = no  # Custom backlighting code is used, so this should not be enabled
RGBLIGHT_ENABLE  = yes # This can be enabled if a ws2812 strip is connected to the expansion port.