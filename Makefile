

TARGET=serial_trans

all: $(TARGET)

$(TARGET): serial_trans.o read_config.o
	cc -Wall -o $(TARGET) $^ 

read_config.o: read_config.c
	cc -c read_config.c

serial_trans.o: serial_trans.c
	cc -c serial_trans.c

.PHONY:clean
clean:
	-rm $(TARGET)
	-rm *.o

