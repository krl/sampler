#include "soundbank.hpp"

SoundBank::SoundBank(ChunkPool *chunkpool) {
  m_chunkpool = chunkpool;
}

bool 
SoundBank::load(char* path) {
  if (m_sounds[path] == NULL) {
    m_sounds[path] = new Sound(path, m_chunkpool);
    cout << "loaded " << path << " at " << m_sounds[path] << endl;
  }
}

SoundBank::~SoundBank() {
  cout << "deleting sounds" << endl;
  // TODO
}

Sound*
SoundBank::get(char* path) {
  cout << "requesting " << path << " at " << m_sounds[path] << endl;
  return m_sounds[path];
}
