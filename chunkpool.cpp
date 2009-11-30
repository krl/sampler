#include "chunkpool.hpp"

ChunkPool::ChunkPool() {
  m_next = 0;

  for (int i = 0 ; i < CHUNK_POOL_SIZE ; i++) {
    m_chunks[i] = NULL;
  }
  this->replenish();
}

int 
ChunkPool::replenish() {
  // loop through buffer and allocate new memory for each
  // NULL pointer.
  int count = 0;

  cout << "repleneshing... ";

  for (int i = 0 ; i < CHUNK_POOL_SIZE ; i++) {
    if (!m_chunks[i]) {
      m_chunks[i] = new sample[CHUNK_SIZE];
      count++;
    }
  }

  cout << count << " chunks allocated" << endl;
  return count;
}

sample* 
ChunkPool::get_chunk() {
  // TODO, thred safety conciderations?
  sample *chunk;
  int start = m_next;
  int index;

  do {    
    index = m_next++;
    if (m_next >= CHUNK_POOL_SIZE)
      m_next = 0;

    chunk = m_chunks[index];

    if (m_next == start) { // did full round.
      cout << "error no free chunk" << endl;
      return NULL;
    }
  } while (chunk == NULL);
  
  m_chunks[index] = NULL; // free up this slot
  cout << "assigned chunk " << index << endl;
  return chunk;
}
