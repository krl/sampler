SOURCES=$(shell ls *.cpp)
OBJECTS=${SOURCES:.cpp=.o}
HEADER=$(shell ls *.hpp)
EXECUTABLE=sampler
CXXFLAGS=-W -Iinclude -std=c++0x
LIBS=-ljack -lsndfile -loscpack -lrubberband
LFLAGS=$(LIBS) -W

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ -g $(OBJECTS) -o $@ $(LFLAGS)

%.o: %.c $(HEADER)
	g++ -g $(CXXFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
