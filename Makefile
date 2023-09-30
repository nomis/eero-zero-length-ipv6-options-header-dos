.PHONY: clean

CXXFLAGS=-Wall -Werror -Wextra
LIBS=

send-packet: send-packet.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f send-packet
