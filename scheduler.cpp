#include "scheduler.hpp"

void
Scheduler::ProcessBundle(const ReceivedBundle& b,
			      const IpEndpointName& remoteEndpoint) {
  for( ReceivedBundle::const_iterator i = b.ElementsBegin(); i != b.ElementsEnd(); ++i ) {
    if( i->IsBundle() )
      ProcessBundle( ReceivedBundle(*i), remoteEndpoint );
    else
      ProcessMessage( ReceivedMessage(*i), remoteEndpoint, b.TimeTag());
  }
}

void
Scheduler::ProcessMessage(const ReceivedMessage& m,
			       const IpEndpointName& remoteEndpoint,
			       uint64 timetag) {
  ReceivedMessageArgumentStream args = m.ArgumentStream();

  const char *path;
  Sound* sound;

  if (strcmp(m.AddressPattern(), "/samp_load") == 0) {
    // s:path
    args >> path;
    cout << "message samp_load path: " << path << endl;

    m_soundbank->load((char*)path);

  } else if (strcmp(m.AddressPattern(), "/samp_play") == 0) {
    // s:path
    args >> path;
    cout << "message samp_play path: " << path << endl; 

    sound = m_soundbank->get((char*)path);

    if (sound != NULL) {
      m_voicepool->play(sound, this->in_frames(timetag));
    } else {
      cout << path << " not loaded" << endl;
    }
  }
}

Scheduler::Scheduler(SoundBank *soundbank, VoicePool* voicepool, int samplerate) {
  m_soundbank  = soundbank;
  m_voicepool  = voicepool;
  m_samplerate = samplerate;
}

jack_nframes_t
Scheduler::in_frames(uint64 timestamp) {
  uint64 diff = timestamp - m_offset_timestamp;

  // FIXME - bit twiddling headache...
  double seconds = (double)(diff >> 32) +
    ((double)((diff << 32) >> 32) / (double)pow(2, 32));
  
  return m_offset_frames + (jack_nframes_t)(seconds * (double)m_samplerate);
}

void
Scheduler::set_time_offset(jack_nframes_t start) {
  m_offset_timestamp = this->get_time_offset();
  m_offset_frames    = start;

  cout << "frame offset: " << m_offset_frames << endl;
}

uint64
Scheduler::get_time_offset() {
  timeval tv;
  gettimeofday(&tv, 0);

  // time conversion algorithm taken from Open Mash under
  // a GPL compatible license http://www.openmash.org/lxr/source/LICENSE
  uint32 sec  = (uint32)tv.tv_sec + GETTIMEOFDAY_TO_NTP_OFFSET;
  uint32 usec = ((tv.tv_usec << 12) + (tv.tv_usec << 8) - (tv.tv_usec * 1825) >> 5);
  return ((uint64)sec << 32) + usec;
}

void
Scheduler::run() {
  UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, OSC_PORT),
			      this);
  s.RunUntilSigInt();
}
