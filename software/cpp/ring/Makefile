CC=g++
CFLAGS=
LDFLAGS=-lm
TARGET=main
LIB = ring_buffer.h ring_buffer.cpp


$(TARGET): $(TARGET).cpp
	@echo "COMPILING CODE"
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LIB) $(LDFLAGS)
	@echo "EXECUTING CODE"
	@./$(TARGET)
	@echo "DELETING EXECUTABLE"
	@rm $(TARGET)