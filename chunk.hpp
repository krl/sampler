#include <jack/jack.h>
#include "def.hpp"

class Chunk {
public:
  Chunk(int frames, int channels, sample *data, Chunk *next);
  ~Chunk();

  sample *m_data;

  int     get_frames();
  sample* get_data();

private:
  int m_frames;
  int m_channels;
  Chunk *m_next;
};
