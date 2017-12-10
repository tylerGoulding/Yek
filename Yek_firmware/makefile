#
#             LUFA Library
#     Copyright (C) Dean Camera, 2017.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USER
F_CPU        = 8000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = Yek
SRC          = Descriptors.c Lib/uLib/u8x8_8x8.c Lib/uLib/u8x8_display.c Lib/uLib/u8x8_byte.c Lib/uLib/u8x8_cad.c Lib/uLib/u8x8_fonts.c Lib/uLib/u8x8_debounce.c Lib/uLib/u8x8_string.c Lib/uLib/u8x8_d_ssd1306_128x32.c Lib/uLib/u8x8_gpio.c Lib/uLib/u8x8_setup.c Lib/millis/millis.c Lib/fram.c Lib/fps_c.cpp Lib/utility/Adafruit_FIFO.cpp Lib/WString.cpp Lib/Print.cpp Lib/Stream.cpp Lib/Adafruit_ATParser.cpp Lib/Adafruit_BLE.cpp Lib/Adafruit_BLEMIDI.cpp Lib/Adafruit_BluefruitLE_SPI.cpp Lib/ble_c.cpp Lib/FPS_GT511C3.cpp $(TARGET).cpp $(LUFA_SRC_SERIAL) $(LUFA_SRC_TWI) $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) 
LUFA_PATH    = ../../../../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
CPP_FLAGS    = -lstdc++ -felide-constructors -std=c++0x
LD_FLAGS     =

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA

include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS


# we are using CPP so we need this build module
include $(LUFA_PATH)/Build/lufa_build.mk

# include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
# include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
# include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
