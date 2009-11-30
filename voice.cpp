using namespace std;

#include "voice.hpp"

Voice::Voice(int channels, int buffer_size, int samplerate) {
  m_buffer_size = buffer_size;
  m_samplerate = samplerate;
  m_channels = channels;
  m_offset = 0;
  m_status = VOICE_IDLE;

  // m_rubber = new RubberBand::RubberBandStretcher(m_samplerate,
  // 						 m_channels,
  // 						 RubberBand::RubberBandStretcher::OptionProcessRealTime);

  // cout << "voice init, buffer size: " << m_buffer_size << endl;
}

Voice::~Voice() {
  // delete m_rubber;
}

int Voice::play(Sound *sound, jack_nframes_t when) {
  cout << "voice::play @ frame: " << when << endl;
  m_offset = 0;
  m_status = VOICE_PLAYING;
  m_sound  = sound;
  m_when   = when;
}

int Voice::reset() {
  m_status = VOICE_IDLE;
  m_sound  = NULL;
}

VoiceStatus Voice::get_status() {
  return m_status;
}

bool Voice::write(sample** buffers, jack_nframes_t frame_time) {

  int offset = frame_time + m_buffer_size - m_when;

  // cout << "voice::write offset " << offset << endl;

  if (offset >= 0) {
    if (offset > m_buffer_size) offset = 0;
    // starts in this cycle    
    for (int c = 0 ; c < 2; c++) { // output channels
      for (int f = offset; f < m_buffer_size; f++) {
	buffers[c][f] += m_sound->frame(c, m_offset + (f - offset));
      }
    }
    m_offset += m_buffer_size - offset;
    if (m_offset > m_sound->get_frames())
      m_status = VOICE_IDLE;
  }

  // for (int c = 0 ; c < 2; c++)
  //   for (int f = 0 ; (f < data_left && f < m_buffer_size); f++)
  //     buffers[c][f] += data[m_offset + f];

  // int required = m_rubber->getSamplesRequired();  

  //cout << ((const float* const*)data)[100] <<endl;

  // m_rubber->process(((float* const*)data), 1, false);

  // m_offset += m_buffer_size;
  // if (m_offset >= data_left) {
  //   m_status = VOICE_IDLE;
  //   return false;
  // }
  return true;
}
