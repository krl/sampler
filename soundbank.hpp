#ifndef _SoundBank
#define _SoundBank

#include <iostream>
#include <unordered_map>
#include <cstring>
#include "sound.hpp"
#include "chunkpool.hpp"

struct eqstr {
  bool operator()(const char* s1, const char* s2) const {
    return strcmp(s1, s2) == 0;
  }
};

class SoundBank {
public:
  SoundBank(ChunkPool *chunkpool);
  ~SoundBank();

  bool   load(char* path);
  Sound* get(char* path);

private:

  ChunkPool *m_chunkpool;
  unordered_map<string, Sound*> m_sounds;
};

#endif
