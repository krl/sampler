using namespace std;

#include <iostream>
#include <string>

#include <jack/jack.h>

#include "chunk.hpp"

Chunk::~Chunk() {
  delete m_data;
  if (m_next) delete m_next;
  cout << "deleting chunk" << endl;
}

Chunk::Chunk(int frames, int channels, sample *data, Chunk *next) {
  m_frames = frames;
  m_channels = channels;
  m_data = data;
  m_next = next;
}

int Chunk::get_frames() {
  return m_frames;
}

sample* Chunk::get_data() {
  return m_data;
}
