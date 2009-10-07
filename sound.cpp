using namespace std;

#include <string>
#include <iostream>

#include <sndfile.h>
#include <jack/jack.h>
#include "sound.hpp"

Sound::~Sound() {
  cout << "deleting sound" << endl;
  
  delete m_chunk;
}

Sound::Sound(char *path) {
  cout << "opening file " << path << endl;

  SNDFILE *file;
  SF_INFO sfinfo;
  sample *sample_buffer;

  if (!(file = sf_open (path, SFM_READ, &sfinfo))) {
    cout << "Not able to open input file '" << path << "'" << endl;
  };

  m_channels = sfinfo.channels;
  m_frames = sfinfo.frames;

  sample_buffer = new sample[m_frames * m_channels];
  sf_read_float(file, sample_buffer, m_frames);

  m_chunk = new Chunk(m_frames, m_channels, sample_buffer, NULL);

  printf("file loaded: %s\nframes: %i\nchannels: %i\n", path, m_frames, m_channels);
}

sample* Sound::get_data() {
  return m_chunk->get_data();
}

// returns how much data can be directly read
int Sound::chunk_left(int offset) {
  return m_chunk->get_frames() - offset;
}

int Sound::get_channels() {
  return m_channels;
}
