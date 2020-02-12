.PHONY=all

all:
	cd src && $(MAKE) 
	cd dummy-proc && $(MAKE)
	cd dummy-lib && $(MAKE)
