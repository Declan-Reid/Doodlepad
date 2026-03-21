APP_NAME:=Doodlepad

SDK_DIR?=./sdk;

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4a_nofpueb-elf-gcc
AS_FLAGS:=

FUNCTION_FLAGS=-flto -ffat-lto-objects -ffunction-sections -fdata-sections -O2 -gdwarf-5

CC:=sh4a_nofpueb-elf-gcc
CC_FLAGS:=$(FUNCTION_FLAGS) -Wall -Wextra -I $(SDK_DIR)/include/

CXX:=sh4a_nofpueb-elf-g++
CXX_FLAGS:=$(FUNCTION_FLAGS) -Wall -Wextra -I $(SDK_DIR)/include/

LD:=sh4a_nofpueb-elf-g++
LD_FLAGS:=$(FUNCTION_FLAGS) -Wl,--gc-sections -L$(SDK_DIR)

READELF:=sh4a_nofpueb-elf-readelf
OBJCOPY:=sh4a_nofpueb-elf-objcopy
STRIP:=sh4a_nofpueb-elf-strip

AS_SOURCES:=$(wildcard *.S)
CC_SOURCES:=$(wildcard *.c)
CXX_SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(AS_SOURCES:.S=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

APP_ELF:=$(APP_NAME).elf
APP_OUT:=$(APP_NAME).hh3

elf: $(APP_ELF) Makefile
hh3: $(APP_OUT) Makefile
all: $(APP_ELF) $(APP_OUT) Makefile

.DEFAULT_GOAL := all

clean:
	rm -f $(OBJECTS) $(APP_ELF) $(APP_ELF).map $(APP_OUT)

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/libsdk.a
	$(LD) -Wl,-Map $@.map -o $@ $(LD_FLAGS) $(OBJECTS) -lsdk

$(APP_OUT): $(APP_ELF)
	$(STRIP) -o $@ $^