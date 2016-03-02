

TARGET=serial_trans

all: $(TARGET)

$(TARGET): serial_trans.o
	cc -o $(TARGET) $^

clean:
	-rm $(TARGET)
	-rm *.o

