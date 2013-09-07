
CFLAGS = g++ -g -O0 -Wall -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -fno-diagnostics-show-option 

LDFLAGS = -std=c++0x -g


programs = invertedindex


all : $(programs)

clean:
	rm -f $(programs) core *.o

.PHONY: clean all
