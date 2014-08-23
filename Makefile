#
#            DMBS Build System
#     Released into the public domain.
#
#  dean [at] fourwalledcubicle [dot] com
#        www.fourwalledcubicle.com
#

# Run "make help" for target help.

MCU          = atmega328p
ARCH         = AVR8
F_CPU        = 16000000
OPTIMIZATION = s
TARGET       = avr-enm
#test/test_basic.c test/test_uart.c test/test_ringbuff.c
SRC          =  \
				drivers/uart.c \
               	communication/comm.c \
				communication/commands.c \
               	util/ringbuffer.c \
               	main.c
               
INCLUDES     = -I . \
               -I ./test \
               -I ./drivers \
               -I ./communication \
               -I ./util
               
CC_FLAGS     = $(INCLUDES)
LD_FLAGS     =

#Programmer
AVRDUDE_PROGRAMMER = arduino
#AVRDUDE_PORT       = /dev/ttyACM0
AVRDUDE_PORT       = /dev/tty.usbmodem1411

# Default target
all:

# Include DMBS build script makefiles
DMBS_PATH   ?= ./DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
