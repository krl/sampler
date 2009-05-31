using namespace std;

#include <string>

#include "chunk.hpp"
#define MAX_CHANNELS 2

class Sound {
public:
  Sound(char *path);

private:
  int channels;
  Chunk *chunk[MAX_CHANNELS];
};
