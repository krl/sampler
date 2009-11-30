#include "voicepool.hpp"

bool VoicePool::play(Sound *sound, jack_nframes_t when) {
  int start = m_next;
  Voice *voice;

  do {
    voice = m_voices[m_next++];

    if (m_next >= m_number)
      m_next = 0;

    if (m_next == start) { // did full round.
      cout << "error no free voice" << endl;
      return false;
    }
  } while (voice->get_status() != VOICE_IDLE);

  voice->play(sound, when);
  return true;
}

bool VoicePool::write(sample** buffers, jack_nframes_t frame_time) {
  for(int i = 0; i < m_number; i++) {
    switch (m_voices[i]->get_status()) {
    case VOICE_PLAYING:
    case VOICE_LOOPING:
      m_voices[i]->write(buffers, frame_time);
      break;
    }
  }	 
}

VoicePool::VoicePool(int number, int buffer_size, int samplerate) {
  m_number = number;
  m_next = 0;
  for (int i = 0 ; i < m_number; i++) {
    m_voices[i] = new Voice(2, buffer_size, samplerate);
  }
}

VoicePool::~VoicePool() {
  cout << "deleting voices" << endl;
  for (int i = 0 ; i < m_number; i++) {
    delete m_voices[i];
  }
}
