.PHONY=all
	
all:
	g++ -fPIC -shared injlib.cpp -o injlib.so -Wl,-init,init_func

clean:
	rm -f injlib.so
