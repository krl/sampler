#include "scheduler.hpp"

void Scheduler::ProcessPacket(const char *data, int size, 
			      const IpEndpointName& remoteEndpoint)
{
  cout << osc::ReceivedPacket( data, size );
}

