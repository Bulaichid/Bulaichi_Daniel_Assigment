INC =  -I.
CXXFLAGS =-std=c++14 -O0 -g -Wall -fmessage-length=0 $(INC)
CXX = c++
OBJS =	user_input_capture.o user_interface.o

LIBS = -pthread

TARGET = user_input_capture

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
