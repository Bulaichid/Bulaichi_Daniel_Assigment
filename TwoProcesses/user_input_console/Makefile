INC =  -I.
CXXFLAGS =-std=c++14 -O0 -g -Wall -fmessage-length=0 $(INC)
CXX = c++
OBJS =	user_console.o

LIBS = -pthread

TARGET = user_console

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
