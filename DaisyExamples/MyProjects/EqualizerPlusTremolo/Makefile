# Project Name
TARGET = EqualizerPlusTremolo

# Sources
CPP_SOURCES = EqualizerPlusTremolo.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

EXTRA_INCLUDES += ../../lib
EXTRA_INCLUDES += ../../Effects
EXTRA_SRCS += ../../lib/biquad.cpp
EXTRA_SRCS += ../../lib/FIRTaps.cpp
EXTRA_SRCS += ../../Effects/Tremolo.cpp

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
