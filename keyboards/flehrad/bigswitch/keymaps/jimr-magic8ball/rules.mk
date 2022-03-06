VIA_ENABLE = no

#------ OLED --------------
#OLED_ENABLE = no

OLED_ENABLE = yes
OLED_DRIVER = SSD1306
#---------------------------

#----- AUDIO --------------
AUDIO_ENABLE = no
#JR::AUDIO ( on avr) appears to incompatible with oled! Corrupts display.
#Se also : https://docs.qmk.fm/#/audio_driver
#
#AUDIO_ENABLE  = yes
#pwm_hardware , see platforms/avr/drivers/audio_pwm_hardware.c ( default? )
#AUDIO_DRIVER = pwm_hardware
#MIDI_ENABLE = yes           # MIDI controls
#---------------------------

#---- Code space recovery ----
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend
TAP_DANCE_ENABLE = no
# we are not using 'actions' so lets gets some codespace back
LTO_ENABLE = yes
