CC=g++
CXXFLAGS=-W -Wall
TARGET=riscv-sim
all: $(TARGET)
OBJS = icaproj1.o
# OBJS = main.o
riscv-sim: $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^
clean:
	rm -rf *.o riscv-sim