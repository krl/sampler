#include <iostream>

#include <oscpack/osc/OscReceivedElements.h>
#include <oscpack/osc/OscPrintReceivedElements.h>

#include <oscpack/ip/UdpSocket.h>
#include <oscpack/ip/PacketListener.h>

using namespace std;

class Scheduler : public PacketListener {
public:
  void ProcessPacket(const char *data, int size, const IpEndpointName& remoteEndpoint );
};
