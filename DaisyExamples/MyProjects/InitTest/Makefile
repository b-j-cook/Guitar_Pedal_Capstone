# Project Name
TARGET = InitTest

# Sources
CPP_SOURCES = InitTest.cpp
CPP_SOURCES += ../Effects/MyTremolo.cpp
CPP_SOURCES += ../lib/FIRTaps.cpp
CPP_SOURCES += ../Effects/MyChorus.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/
C_INCLUDES += -I../Effects
C_INCLUDES += -I../lib

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
