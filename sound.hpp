#ifndef _Sound
#define _Sound

using namespace std;

#include <string>
#include <jack/jack.h>
#include "def.hpp"
#include "chunkpool.hpp"

#include <iostream>
#include <sndfile.h>

class Sound {
public:
  Sound(char *path, ChunkPool *chunkpool);
  ~Sound();

  int     get_channels();
  sample* m_chunks[MAX_CHUNKS];


private:
  ChunkPool *m_chunkpool;

  int m_channels;
  int m_frames;
};

#endif
