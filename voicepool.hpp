#ifndef _VoicePool
#define _VoicePool

#include <iostream>
#include "voice.hpp"
#include "sound.hpp"

class VoicePool {
public:
  VoicePool(int number, int buffer_size, int samplerate);
  ~VoicePool();

  // control
  bool play(Sound *sound, jack_nframes_t when);
  bool write(sample** buffers, jack_nframes_t frame_time);

private:
  int m_number;
  int m_next;
  Voice* m_voices[MAX_VOICES];

};

#endif
