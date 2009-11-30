using namespace std;

#include "sound.hpp"

Sound::~Sound() {
  cout << "deleting sound" << endl;
  
  // delete m_chunk;
}

Sound::Sound(char *path, ChunkPool *chunkpool) {
  cout << "opening file " << path << endl;

  SNDFILE *file;
  SF_INFO sfinfo;
  sample *sample_buffer;

  m_chunkpool = chunkpool;

  if (!(file = sf_open (path, SFM_READ, &sfinfo))) {
    cout << "Not able to open input file '" << path << "'" << endl;
  };

  m_channels = sfinfo.channels;
  m_frames   = sfinfo.frames;

  sample_buffer = new sample[CHUNK_SIZE * m_channels];

  int framechunks = (m_frames / CHUNK_SIZE) + 1;
  
  for (int i = 0 ; i < framechunks ; i++) {
    int read = sf_read_float(file, sample_buffer, CHUNK_SIZE);
    for (int c = 0 ; c < m_channels ; c++) {
      m_chunks[i * m_channels] = m_chunkpool->get_chunk();
      // TODO handle chunk unavailability
      for (int f = 0; f < read; f++) {
	m_chunks[i * m_channels][f] = sample_buffer[(f * m_channels) + c];
      }
    }
  }	

  cout << "file loaded: " << path        << endl
       << "frames: "      << m_frames    << endl
       << "channels: "    << m_channels  << endl
       << "framechunks: " << framechunks << endl;
}

sample Sound::frame(int channel, int index) {

  // cout << "m_frames " << m_frames << "\tindex " << index << endl
  //      << "index/chunkz " << index / CHUNK_SIZE << "\tindex%cz " << index % CHUNK_SIZE << endl;
  if (index < m_frames)
    return m_chunks[index / CHUNK_SIZE][index % CHUNK_SIZE];
  else
    return (sample)0;
}

int Sound::get_channels() {
  return m_channels;
}

int Sound::get_frames() {
  return m_frames;
}
