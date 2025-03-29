CC=g++
CXXFLAGS=-W -Wall
TARGET=riscv-sim
all: $(TARGET)
OBJS = diff.o
riscv-sim: $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^
clean:
	rm -rf *.o riscv-sim