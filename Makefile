OBJS=main.o
BIN=simlit
CXXFLAGS ?= -std=c++11 -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -Wall

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm $(OBJS) $(BIN)

