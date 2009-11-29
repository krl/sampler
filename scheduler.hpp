#include "def.hpp"

#include "soundbank.hpp"
#include "voicepool.hpp"

#include <jack/jack.h>
#include <sys/time.h>
#include <iostream>
#include <cstring>
#include <cmath>

#include <oscpack/osc/OscReceivedElements.h>
#include <oscpack/osc/OscTypes.h>
#include <oscpack/ip/PacketListener.h>
#include <oscpack/ip/UdpSocket.h>

using namespace std;
using namespace osc;

class Scheduler : public PacketListener {

public:
  Scheduler(SoundBank *soundbank, VoicePool* voicepool, int samplerate);
  void run();
  
  void           set_time_offset(jack_nframes_t start);
  uint64         get_time_offset();
  jack_nframes_t in_frames(uint64 timetag);

private:
  uint64         m_offset_timestamp;
  jack_nframes_t m_offset_frames;
  int            m_samplerate;
  SoundBank*     m_soundbank;
  VoicePool*     m_voicepool;

protected:
  virtual void ProcessBundle(const ReceivedBundle& b,
			     const IpEndpointName& remoteEndpoint );

  virtual void ProcessMessage(const ReceivedMessage& m,
  			      const IpEndpointName& remoteEndpoint,
			      uint64 timetag);
    
public:
  virtual void ProcessPacket(const char *data, int size, 
			     const IpEndpointName& remoteEndpoint)
  {
    ReceivedPacket p(data, size);
    if(p.IsBundle())
      ProcessBundle(ReceivedBundle(p), remoteEndpoint);
    else
      ProcessMessage(ReceivedMessage(p), remoteEndpoint, NULL);
  }
};
