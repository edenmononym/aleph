# List of available make goals:
#
# all                     Default target, builds the project
# clean                   Clean up the project
# rebuild                 Rebuild the project
#
# isp                     Use BatchISP instead of avr32program when interfacing
#                         the target device.
#
# chiperase               Perform a JTAG chip erase command
# cpuinfo                 Get CPU information
# debug                   Open a debug connection with the MCU
# erase                   Perform a flash erase
# halt                    Stop CPU execution
# program                 Program MCU memory from ELF output file
# reset                   Reset MCU
# readregs                Read CPU registers
# run                     Start CPU execution
# secureflash             Protect chip by setting security bit
#
# doc                     Build the documentation
# cleandoc                Clean up the documentation
# rebuilddoc              Rebuild the documentation


# Path to top level ASF directory relative to this project directory.
# our version-controlled dir
PRJ_PATH = ../../avr32_lib/asf-3.7.3

# Include the common Makefile, which will also include the project specific
# config.mk file.

MAKEFILE_PATH = $(PRJ_PATH)/avr32/utils/make/Makefile.avr32.in
include $(MAKEFILE_PATH)
