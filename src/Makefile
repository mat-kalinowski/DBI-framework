.PHONY=all

CFLAGS := ""

ifeq ($(arch), ARM)
	CFLAGS := __ARM32__ARCH	
else ifeq ($(arch),x86)
	CFLAGS := __X86__ARCH
else
	exit 1
endif

all:
	g++ injector.cpp utils.cpp main.cpp parser.cpp -o injector -D $(CFLAGS)

clean:
	rm -f injector
