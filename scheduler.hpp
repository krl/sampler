#include "def.hpp"
#include <jack/jack.h>
#include <sys/time.h>
#include <iostream>

#include <oscpack/osc/OscReceivedElements.h>
#include <oscpack/ip/PacketListener.h>
#include <oscpack/ip/UdpSocket.h>

using namespace std;

class Scheduler : public PacketListener {

public:
  Scheduler();
  void run();
  
  void set_time_offset();
  osc::uint64 m_offset;

protected:
  virtual void ProcessBundle(const osc::ReceivedBundle& b,
			     const IpEndpointName& remoteEndpoint );

  virtual void ProcessMessage(const osc::ReceivedMessage& m,
  			      const IpEndpointName& remoteEndpoint);
    
public:
  virtual void ProcessPacket(const char *data, int size, 
			     const IpEndpointName& remoteEndpoint)
  {
    osc::ReceivedPacket p(data, size);
    if(p.IsBundle())
      ProcessBundle(osc::ReceivedBundle(p), remoteEndpoint);
    else
      ProcessMessage(osc::ReceivedMessage(p), remoteEndpoint);
  }
};
