CC = mpicc
CPP = mpic++
LDC = mpicc
LD_FLAGS = -lm -fopenmp -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
FLAGS = -fopenmp
CPPFLAGS = -I/usr/include/opencv4 $(FLAGS)
PROG = PA3.x
RM = /bin/rm

OBJS = PA3.o
TOOLS = imageTools.o

all: $(PROG)

$(PROG): $(OBJS) $(TOOLS)
	$(LDC) $^ $(LD_FLAGS) -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	$(RM) -rf *.o *.x *.jpg
