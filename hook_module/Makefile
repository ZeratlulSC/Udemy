#подключает файл конфига, использует переменные среды
#default.config
CONFIG = $(PWD)
include ${CONFIG}

#Возможно, имя компилируемого файла
TARGET=hooker 
SOURCES=hook_module.c log.c
OBJECTS=$(SOURCES:.c=.o)

ifeq ($(CONFIG_HOOK_FILTER), y)
KBUILD_CFLAGS+=-DCONFIG_HOOK_FILTER=1
SOURCES+=filter.c
endif
ifeq ($(CONFIG_HOOK_LOGGER), y)
KBUILD_CFLAGS+=-DCONFIG_HOOK_LOGGER=1
SOURCES+=logger.c
endif
ifeq ($(CONFIG_HOOK_INJECT), y)
KBUILD_CFLAGS+=-DCONFIG_HOOK_INJECT=1
SOURCES+=inject.c
endif

obj-m += $(TARGET).o # filter.o
$(info ########## $(CONFIG_HOOK_FILTER))

$(TARGET)-objs := $(OBJECTS)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
#	make -C /lib/modules/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
#	make -C /lib/modules/ M=$(PWD) clean
