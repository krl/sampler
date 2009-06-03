SOURCES=$(shell ls *.cpp)
OBJECTS=${SOURCES:.cpp=.o}
#HEADER=$(shell ls *.hpp)
EXECUTABLE=sampler
# CFLAGS=-Wuninitialized -O -g
CFLAGS=-W -Iinclude
LIBS=-ljack -lsndfile
LFLAGS=$(LIBS) -W

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ -g $(OBJECTS) -o $@ $(LFLAGS)

%.o: %.c $(HEADER)
	g++ -g $(CFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
