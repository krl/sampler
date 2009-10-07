#include <iostream>
#include "voice.hpp"
#include "sound.hpp"

class VoicePool {
public:
  VoicePool(int number, int buffer_size, int samplerate);
  ~VoicePool();

  // control
  bool play(Sound *sound);
  bool write(sample** buffers);

private:
  int m_number;
  int m_next;
  Voice* m_voices[MAX_VOICES];

};
