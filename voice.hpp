#ifndef _Voice
#define _Voice

#include <jack/jack.h>
#include <iostream>
#include <math.h>
//#include <rubberband/RubberBandStretcher.h>
#include <oscpack/osc/OscTypes.h>

#include "def.hpp"
#include "sound.hpp"

using namespace osc;

enum VoiceStatus {VOICE_IDLE, VOICE_PLAYING, VOICE_LOOPING};

class Voice {
public:
  Voice(int channels, int buffer_size, int samplerate);
  ~Voice();
  
  // control
  int play(Sound *sound, jack_nframes_t when);
  int reset();
  int advance();
  bool write(sample** buffers, jack_nframes_t frame_time);
  VoiceStatus get_status();

private:
  Sound *m_sound;

  uint64 start_timestamp;

  int m_samplerate;
  int m_buffer_size;
  int m_channels;
  int m_offset;
  jack_nframes_t m_when;
  VoiceStatus m_status;

  // RubberBand::RubberBandStretcher *m_rubber;

  // here be envelopes etc...
};

#endif
