KVERSION = $(shell uname -r)

TARGET = pgen

obj-m := ${TARGET}.o

${TARGET}-objs += ./src/pgen.o \
		  ./src/pgen_base64.o \
		  ./src/pgen_file_operations.o

ccflags-y += -I ${PWD}/include

all: ${TARGET}.ko

${TARGET}.ko:
	make -C /lib/modules/${KVERSION}/build M=${PWD} modules

.PHONY: clean
clean:
	make -C /lib/modules/${KVERSION}/build M=${PWD} clean

.PHONY: install
install: ${TARGET}.ko
	insmod $<

