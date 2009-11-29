#include "soundbank.hpp"

SoundBank::SoundBank(ChunkPool *chunkpool) {
  m_chunkpool = chunkpool;
}

bool 
SoundBank::load(char* path) {
  Sound* tmp;
  tmp = new Sound(path, m_chunkpool);
  m_sounds[path] = tmp;
  //delete tmp;
}

SoundBank::~SoundBank() {
  cout << "deleting sounds" << endl;
  // TODO
}

Sound*
SoundBank::get(char* path) {
  return m_sounds[path];
}
