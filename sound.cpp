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

  int framechunks = (m_frames / CHUNK_SIZE) + (m_frames % CHUNK_SIZE) != 0 ? 1 : 0;
  
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

  cout << "file loaded: " << path       << endl
       << "frames: "      << m_frames   << endl
       << "channels: "    << m_channels << endl;
}

// sample* Sound::get_data() {
//   // return m_chunk->get_data();
// }

int Sound::get_channels() {
  return m_channels;
}
