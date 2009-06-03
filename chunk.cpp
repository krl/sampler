using namespace std;

#include <iostream>
#include <string>

#include <jack/jack.h>

#include "chunk.hpp"

Chunk::Chunk(int frames, jack_default_audio_sample_t *data, Chunk *next) {
  m_frames = frames;
  m_data = data;
  m_next = next;
}
