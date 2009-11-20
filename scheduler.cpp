#include "scheduler.hpp"

void Scheduler::ProcessBundle(const osc::ReceivedBundle& b,
			      const IpEndpointName& remoteEndpoint) {
  cout << b.TimeTag() << endl;
}

void Scheduler::ProcessMessage(const osc::ReceivedMessage& m,
			       const IpEndpointName& remoteEndpoint) {
}

Scheduler::Scheduler() {
}

void Scheduler::set_time_offset() {
  timeval tv;
  gettimeofday(&tv, 0);

  // time conversion algorithms taken from Open Mash and copyrighted
  // under a GPL compatible license http://www.openmash.org/lxr/source/LICENSE
  u_int sec  = (u_int)tv.tv_sec + GETTIMEOFDAY_TO_NTP_OFFSET;
  u_int usec = ((tv.tv_usec << 12) + (tv.tv_usec << 8) - (tv.tv_usec * 1825) >> 5);

  m_offset = ((unsigned long)sec << 32) + usec;  
}

void Scheduler::run() {
  UdpListeningReceiveSocket s(IpEndpointName( IpEndpointName::ANY_ADDRESS, OSC_PORT),
			      this);
  s.RunUntilSigInt();
}
