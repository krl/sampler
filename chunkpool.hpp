#ifndef _ChunkPool
#define _ChunkPool

#include <iostream>
#include <jack/jack.h>
#include "def.hpp"

using namespace std;

class ChunkPool {
public:
  ChunkPool();
  ~ChunkPool();

  sample *get_chunk();
  int     replenish();

private:  
  sample *m_chunks[CHUNK_POOL_SIZE];
  int m_next;
};

#endif
