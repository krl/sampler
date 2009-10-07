SOURCES=$(shell ls *.cpp)
OBJECTS=${SOURCES:.cpp=.o}
HEADER=$(shell ls *.hpp)
EXECUTABLE=sampler
CFLAGS=-W -Iinclude
LIBS=-ljack -lsndfile -lrubberband -loscpack
LFLAGS=$(LIBS) -W

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ -g $(OBJECTS) -o $@ $(LFLAGS)

%.o: %.c $(HEADER)
	g++ -g $(CFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
