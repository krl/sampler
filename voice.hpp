#include <jack/jack.h>

#include "def.hpp"
#include "sound.hpp"

enum VoiceStatus { VOICE_IDLE, VOICE_PLAYING, VOICE_LOOPING };

class Voice {
public:
  Voice(int channels, int buffer_size, int samplerate);
  ~Voice();

  // control
  int play(Sound *sound);
  int reset();
  int advance();
  bool write(sample** buffers);
  VoiceStatus get_status();

private:
  Sound *m_sound;

  int m_samplerate;
  int m_buffer_size;
  int m_channels;
  int m_offset;
  VoiceStatus m_status;

  //RubberBand::RubberBandStretcher *m_rubber;

  // here be envelopes etc, etc, etc...
};
