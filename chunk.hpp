#include <jack/jack.h>

class Chunk {
 public:
  Chunk(int frames, jack_default_audio_sample_t *data, Chunk *next);

 private:
  int m_frames;
  jack_default_audio_sample_t *m_data;
  Chunk *m_next;
};
