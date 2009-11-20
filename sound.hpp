#ifndef _Sound
#define _Sound

using namespace std;

#include <string>
#include "def.hpp"
#include "chunk.hpp"

class Sound {
public:
  Sound(char *path);
  ~Sound();

  // the size of continuous sample data in memory at offset x
  int     get_channels();
  int     chunk_left(int offset);
  sample* get_data();

private:
  Chunk *m_chunk;

  int m_channels;
  int m_frames;
};

#endif
